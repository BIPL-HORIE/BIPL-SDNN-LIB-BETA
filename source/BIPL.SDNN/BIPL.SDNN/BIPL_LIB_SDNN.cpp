#include "BIPL_LIB_SDNN.h"
#include "BIPL_LIB_SDNN_NN_VIRTUAL.h"
#include "BIPL_LIB_SDNN_SD_VIRTUAL.h"
#include "BIPL_LIB_SDNN_FACTORY.h"
#include "BIPL_LIB_SDNN_PARAMETERS.h"
#include "BIPL_LIB_SDNN_PC.h"

#include "BIPL_LIB_PARAMETERS.h"
#include "BIPL_LIB_LEXIAL.h"
#include "BIPL_LIB_UTILITY.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>

namespace bipl
{
	namespace sdnn
	{
		std::string CheckParameterFile(std::string parameter_filename)
		{
			bipl::parameters::PARAMETERS check_parameter;
			check_parameter.LoadFile(parameter_filename);
			std::string result_buffer;

			//���͐��̃`�F�b�N
			if ((result_buffer = parameter_property::PARAM_INPUT_NUMBER::property_.Check(check_parameter)) != "")
				return result_buffer+ "\n";

			//multi_thread�̃`�F�b�N
			if ((result_buffer = parameter_property::PARAM_OPTION_MULTI_THREAD_USE::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			//SD���@�̃`�F�b�N
			if ((result_buffer = parameter_property::PARAM_SD_METHOD::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			std::string method;
			parameter_property::PARAM_SD_METHOD::property_.Read(method, check_parameter);
			if (method == parameter_property::PARAM_SD_METHOD::CNT_SD_METHOD::file_)
			{
				if ((result_buffer = parameter_property::PARAM_SD_METHOD::PARAM_FILE::property_.Check(check_parameter)) != "")
					return result_buffer + "\n";
			}

			//PC�̃`�F�b�N
			if ((result_buffer = parameter_property::PARAM_SD_PC_N::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			int input_number, n;
			parameter_property::PARAM_INPUT_NUMBER::property_.Read(input_number, check_parameter);
			parameter_property::PARAM_SD_PC_N::property_.Read(n, check_parameter);
			auto judgement_buffer = std::make_unique<bipl::sdnn::parameter_property::PARAM_SD_PC_TYPE::JUDGEMENT_PC_TYPE_>(input_number, n);
			parameter_property::PARAM_SD_PC_TYPE::property_.judgement_ = std::move(judgement_buffer);

			if ((result_buffer = parameter_property::PARAM_SD_PC_TYPE::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			if ((result_buffer = parameter_property::PARAM_SDNN_TYPE::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			std::string issue_type;
			parameter_property::PARAM_SDNN_TYPE::property_.Read(issue_type,check_parameter);

			if ((result_buffer = parameter_property::PARAM_NN_INITIAL_VALUE_RANGE::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			if (issue_type == parameter_property::PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::function_approximation_)
			{
				if ((result_buffer = parameter_property::PARAM_NN_PP_OUTPUT_RANGE::property_.Check(check_parameter)) != "")
					return result_buffer + "\n";

				std::vector<double> output_range;
				parameter_property::PARAM_NN_PP_OUTPUT_RANGE::property_.Read(output_range, check_parameter);
				auto judgement_buffer = std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<double>>(0.000001, (output_range[1] - output_range[0]));
				parameter_property::PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE::property_.judgement_ = std::move(judgement_buffer);
				if ((result_buffer = parameter_property::PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE::property_.Check(check_parameter)) != "")
					return result_buffer + "\n";
			}
			else if(issue_type == parameter_property::PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::pattern_recognition_)
			{
				if ((result_buffer = parameter_property::PARAM_NN_SP_CATEGORY_NUMBER::property_.Check(check_parameter)) != "")
					return result_buffer + "\n";

				if ((result_buffer = parameter_property::PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::property_.Check(check_parameter)) != "")
					return result_buffer + "\n";
			}
			else
			{
				bipl::utility::error::BugFound(0x206);
			}
			if ((result_buffer = parameter_property::PARAM_NN_TRAINING_END::property_.Check(check_parameter)) != "")
				return result_buffer + "\n";

			return "OK";
		}
	}
}

class bipl::sdnn::SDNN::SDNN_PIMPL
{
private:
	std::unique_ptr<bipl::sdnn::base::SDNN_SD_VIRTUAL> sd_;
	std::unique_ptr<bipl::sdnn::base::SDNN_NN_VIRTUAL> nn_;

	std::vector<int> adress_data_list_;
	std::vector<std::string> train_method_;

	enum STATEMENT
	{
		instance,
		inited,
	};
	STATEMENT sdnn_statement_;

	/*! @brief ���o�̓T���v����1��w�K
	@param[in] input ���̓��X�g
	@param[in] target �ڕW�l���X�g
	@return�@�Ȃ��C�d������throw���Ȃ��D
	*/
	void TrainOnce(const std::vector<std::vector<double>>input, const std::vector<double> target)
	{
		const std::vector<double> *input_buffer = input.data();
		const double *target_buffer = target.data();
		for (unsigned int sample_counter = 0; sample_counter < input.size(); sample_counter++)
		{
			sd_->SelectiveDesensitization(adress_data_list_.data(), input_buffer++->data());
			nn_->Train(adress_data_list_.data(), *target_buffer++);
		}
	}

	//�w�K�̓r���o�ߕ\���n
	void (SDNN_PIMPL::*OutProgression)(double input1, double input2);
	void OutProgressionNon(double input1, double input2) {}
	void OutProgressionDefault(double input1, double input2) { std::cout << "\t\t\t\r" << input1 << "/" << input2 << "\r"; }

	//! �R�s�[�R���X�g���N�^�͋֎~
	SDNN_PIMPL(const SDNN_PIMPL&) = delete;
	//! ������Z���֎~
	void operator=(const SDNN_PIMPL&) = delete;
	
public:
	SDNN_PIMPL() :sd_(nullptr), nn_(nullptr), sdnn_statement_(instance){ OutProgression = (&SDNN_PIMPL::OutProgressionDefault); }
	~SDNN_PIMPL() {}

	parameters::PARAMETERS sdnn_parameter_;

	/*! @brief SDNN�̏�����
	@param[in] parameter_stream BPL�̏����ꂽstringstream
	@return �Ȃ��D�G���[��throw
	@memo ��{��std::string�@stringstream��Load�֐���p
	*/
	template<typename parameter>
	void InitSDNN(parameter &parameter_filename)
	{
		if (sdnn_statement_ != instance)
			utility::error::BugFound(0x10105);
		try
		{
			sdnn_parameter_.LoadFile(parameter_filename);

			base::FactorySD(sd_, sdnn_parameter_);
			base::FactoryNN(nn_, sdnn_parameter_);

			sd_->InitSD(sdnn_parameter_);
			nn_->InitNN(sdnn_parameter_, sd_->GetSDPatternNeuronNumber());

			adress_data_list_.resize(sd_->GetSDPatternNeuronNumber() + 2);

			std::string train_method_buffer;
			parameter_property::PARAM_NN_TRAINING_END::property_.Read(train_method_buffer,sdnn_parameter_);
			lexial::Split(train_method_, train_method_buffer, '+');

			std::string print_progression;
			parameter_property::PARAM_OPTION_PRINT_PROGRESSION::property_.Read(print_progression, sdnn_parameter_);
			if (print_progression == parameter_property::PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::n_)
			{
				OutProgression = (&SDNN_PIMPL::OutProgressionNon);
			}
			sdnn_statement_ = inited;

			std::string use_multi_thread;
			parameter_property::PARAM_OPTION_MULTI_THREAD_USE::property_.Read(use_multi_thread, sdnn_parameter_);
			if(use_multi_thread == parameter_property::PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::y_)
			{
#ifdef _OPENMP
				//�X���b�h���̊m�F�ƃZ�b�g�D
				unsigned int thread_number;
				parameter_property::PARAM_OPTION_MULTI_THREAD_USE::PARAM_OPTION_MULTI_THREAD_NUMBER::property_.Read(thread_number, sdnn_parameter_);
				omp_set_num_threads(thread_number);
#else
				std::cout << "please compile with -openmp"<< std::endl;
#endif

			}
		}
		catch (...)
		{
			utility::error::BugFound(0x1ff04);
		}
	}

	/*! @brief �o�͂̌���
	@param[in] input ����
	@return ���肵���o��
	*/
	double Estimate(const std::vector<double> &input)
	{
		sd_->SelectiveDesensitization(adress_data_list_.data(), input.data());
		return nn_->DetermineOutput(adress_data_list_.data());
	}

	/*! @brief �����d�ʎ擾
	@param[in] out_potential �����d��
	@param[in] input ����
	@return ���肵���o��
	*/
	void GetPotential(std::vector<int> &out_potential, const std::vector<double> input)
	{
		sd_->SelectiveDesensitization(adress_data_list_.data(), input.data());
		nn_->GetPotential(out_potential, adress_data_list_.data());
	}

	/*! @brief 1�T���v���̊w�K
	@param[in] input ����
	@param[in] target �ڕW�l
	@return�@�Ȃ�
	@memo �@�B�w�K�ł̗��p��z��
	*/
	void TrainOneSample(const std::vector<double> &input, const double target)
	{
		if (sdnn_statement_ == instance)
		{
			utility::error::BugFound(0x10106);
		}
		sd_->SelectiveDesensitization(adress_data_list_.data(), input.data());
		nn_->Train(adress_data_list_.data(), target);
	}

	/*! @brief �w�K���@���X�g�ɏ]���Ċw�K�D
	@param[in] input ���̓��X�g
	@param[in] target �ڕW�l���X�g
	@return�@�Ȃ��C�d������throw���Ȃ��D
	*/
	void Train(const std::vector<std::vector<double>>input, const std::vector<double> target)
	{
		if (input.size() != target.size())
		{
			utility::error::BugFound(0x10100);
		}
		if (sdnn_statement_ == instance)
		{
			utility::error::BugFound(0x10106);
		}

		for (std::vector<std::string>::iterator train_method_itr = train_method_.begin(); train_method_itr != train_method_.end(); train_method_itr++)
		{
			std::vector<std::string> train_method_buffer;
			lexial::Split(train_method_buffer, *train_method_itr, '(');

			if (train_method_buffer[0] == "iteration")
			{
				train_method_buffer[1].erase(--train_method_buffer[1].end());
				int count = stoi(train_method_buffer[1]);
				for (int i = 1; i <= count; i++)
				{
					(this->*OutProgression)(i, count);
					TrainOnce(input, target);
				}
			}
			else if (train_method_buffer[0] == "rmse")
			{
				train_method_buffer[1].erase(--train_method_buffer[1].end());
				std::vector<std::string> argument;
				lexial::Split(argument, train_method_buffer[1], ',');

				double rmse = stof(argument[0]);
				int count = stoi(argument[1]);
				double rmse_buffer;
				for (int i = count; i > 0; i--)
				{
					rmse_buffer = 0;
					TrainOnce(input, target);
					for (unsigned int l_counter = 0; l_counter < input.size(); l_counter++)
					{
						double result = Estimate(input[l_counter]);
						rmse_buffer += (result - target[l_counter]) * (result - target[l_counter]);
					}
					rmse_buffer /= input.size();
					rmse_buffer = sqrt(rmse_buffer);
					(this->*OutProgression)(rmse_buffer, rmse);
					if (rmse_buffer <= rmse)
						break;
				}
			}
			else
			{
				utility::error::BugFound(0x1ff05);
			}
		}

		std::string save_filename;
		parameter_property::PARAM_OPTION_SAVE_FILENAME::property_.Read(save_filename, sdnn_parameter_);
		Save(save_filename);
	}

	/*! @brief �w�K���ʂ̕ۑ�
	@param[in] filename �ۑ���t�@�C������
	@return�@�Ȃ��C
	*/
	void Save(const std::string &filename)
	{
		if (sdnn_statement_ == instance)
		{
			utility::error::BugFound(0x10107);
		}
		try
		{
			std::string method;
			//SD�g�ݍ��킹�w���i�̃I�[�o�[���C�g�Csd_file�s�v��
			parameter_property::PARAM_SD_METHOD::property_.Read(method, sdnn_parameter_);
			if(method != parameter_property::PARAM_SD_METHOD::CNT_SD_METHOD::saved_)
			{
				sdnn_parameter_.OverwriteParameter(parameter_property::PARAM_SD_METHOD::property_.name_, parameter_property::PARAM_SD_METHOD::CNT_SD_METHOD::saved_);
				sdnn_parameter_.OverwriteParameter(parameter_property::PARAM_SD_METHOD::PARAM_LINE::property_.name_, sd_->GetDesensitizationList());
			}
			
			//CORRELATION_MATRIX�w��̃I�[�o�[���C�g�CCMfile�s�v��
			std::vector<std::string> pc_type_list;
			parameter_property::PARAM_SD_PC_TYPE::property_.Read(pc_type_list, sdnn_parameter_);
			for (auto ptl_itr = pc_type_list.begin(); ptl_itr != pc_type_list.end(); ptl_itr++)
			{
				if (ptl_itr->find(parameter_property::PARAM_SD_PC_TYPE::CNT_PC_METHOD_::correlation_matrix_) != std::string::npos)
				{
					std::vector<std::string> setting_buffer_vector;
					bipl::lexial::Split(setting_buffer_vector, *ptl_itr, '(');
					setting_buffer_vector[2].erase(--setting_buffer_vector[2].end());
					setting_buffer_vector[2].erase(--setting_buffer_vector[2].end());

					std::vector<std::string> f_b_i_p;
					bipl::lexial::Split(f_b_i_p,setting_buffer_vector[2], ',');

					std::string line_buffer = bipl::sdnn::base::pc::CorrelationMatrixFile2Strings(f_b_i_p[0]);
					*ptl_itr = setting_buffer_vector[0] + "(" + parameter_property::PARAM_SD_PC_TYPE::CNT_PC_METHOD_::saved_ + "(" + line_buffer + "," + f_b_i_p[1] + "," + f_b_i_p[2] + "," + f_b_i_p[3] + "))";
				}
			}
			sdnn_parameter_.OverwriteParameter(parameter_property::PARAM_SD_PC_TYPE::property_.name_, pc_type_list);

			std::ofstream parameter_file;
			utility::OpenFile(parameter_file, filename, std::ios::binary);
			std::stringstream buffer_stream;
			buffer_stream << sdnn_parameter_;
			size_t parameter_length = buffer_stream.str().size();

			parameter_file.write((char*)&parameter_length, sizeof(int));
			parameter_file.write((char*)buffer_stream.str().c_str(), sizeof(char)*parameter_length);

			std::vector<int> w;
			nn_->GetW(w);

			for (unsigned int i = 0; i < w.size(); i++)
			{
				parameter_file.write((const char*)&w[i], sizeof(int));
			}
		}
		catch (...)
		{
			utility::error::BugFound(0x1ff06);
		}
	}

	/*! @brief �w�K���ʂ̃��[�h
	@param[in] filename �w�K���ʃt�@�C������
	@return�@�Ȃ��C
	*/
	void Load(const std::string &filename)
	{
		if (sdnn_statement_ != instance)
		{
			utility::error::BugFound(0x10108);
		}
		try
		{
			std::ifstream parameter_file;
			utility::OpenFile(parameter_file, filename, std::ios::binary);
			int parameter_length;
			parameter_file.read((char*)&parameter_length, sizeof(int));
			char* test = new char[parameter_length + 1];
			parameter_file.read(test, sizeof(char)*parameter_length);
			std::stringstream str;
			test[parameter_length] = '\0';
			str << test;
			InitSDNN(str);

			std::vector<int> w;
			nn_->GetW(w);

			int w_buffer;
			std::vector<int> w_vector;
			w_vector.clear();
			for (int i = 0; i < nn_->GetWNumber(); i++)
			{
				parameter_file.read((char*)&w_buffer, sizeof(w_buffer));
				if (parameter_file.bad())
					throw "file error";
				w_vector.push_back(w_buffer);
			}
			nn_->LoadW(w_vector);
		}
		catch (...)
		{
			utility::error::BugFound(0x1ff07);
		}
	}

	/*! @brief ���Z�b�g�DInit�����蒼��
	@return�@�Ȃ��C
	*/
	void Reset(void)
	{
		sdnn_statement_ = instance;
		sd_.reset();
		nn_.reset();
	}

	void GetParameter(std::string &out_parameter, const std::string &parameter_name)
	{
		sdnn_parameter_.ReadParameter(out_parameter, parameter_name,"");
	}

};

template void bipl::sdnn::SDNN::SDNN_PIMPL::InitSDNN(const std::string& filename);
template void bipl::sdnn::SDNN::SDNN_PIMPL::InitSDNN(std::stringstream& filename);

//SDNN�N���X����

bipl::sdnn::SDNN::SDNN():pimpl_(new SDNN_PIMPL)
{}

bipl::sdnn::SDNN::~SDNN()
{}

void bipl::sdnn::SDNN::InitSDNN(const std::string &parameter_filename)
{
	pimpl_->InitSDNN(parameter_filename);
}

double bipl::sdnn::SDNN::Estimate(const std::vector<double> &input)
{
	return pimpl_->Estimate(input);
}

double bipl::sdnn::SDNN::Estimate4Matlab(double *input)
{
	unsigned int input_number = 0;
	parameter_property::PARAM_INPUT_NUMBER::property_.Read(input_number, pimpl_->sdnn_parameter_);
	std::vector<double> input_matrix;
	for (unsigned int i_counter = 0; i_counter < input_number; i_counter++)
		input_matrix.push_back(*input++);
	return pimpl_->Estimate(input_matrix);
}

void bipl::sdnn::SDNN::GetPotential(std::vector<int> &out_potential, const std::vector<double> input)
{
	pimpl_->GetPotential(out_potential, input);
}

void bipl::sdnn::SDNN::TrainOneSample(const std::vector<double> &input, const double target)
{
	pimpl_->TrainOneSample(input, target);
}

void bipl::sdnn::SDNN::Train(const std::vector<std::vector<double>>input, const std::vector<double> target)
{
	pimpl_->Train(input, target);
}

void bipl::sdnn::SDNN::Train4Matlab(double *input, double *target, int target_number)
{
	std::vector<std::vector<double>> input_matrix(target_number);
	std::vector<double> target_matrix;
	unsigned int input_number = 0;
	parameter_property::PARAM_INPUT_NUMBER::property_.Read(input_number,pimpl_->sdnn_parameter_);

	for (int t_counter = 0; t_counter < target_number; t_counter++)
	{
		target_matrix.push_back(*target++);
		for (unsigned int i_counter = 0; i_counter < input_number; i_counter++)
		{
			input_matrix[t_counter].push_back(*input++);
		}
	}
	pimpl_->Train(input_matrix, target_matrix);
}

void bipl::sdnn::SDNN::Save(const std::string &filename)
{
	pimpl_->Save(filename);
}
void bipl::sdnn::SDNN::Load(const std::string &filename)
{
	pimpl_->Load(filename);
}

void bipl::sdnn::SDNN::Reset(void)
{
	pimpl_->Reset();
}

template<typename content_type>
void bipl::sdnn::SDNN::GetParameter(content_type &out_parameter, const std::string &parameter_name)
{
	pimpl_->sdnn_parameter_.ReadParameter(out_parameter, parameter_name,"");
}

template void bipl::sdnn::SDNN::GetParameter(std::string &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(int &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(double &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(unsigned int &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(std::vector<std::string> &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(std::vector<int> &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(std::vector<double> &out_parameter, const std::string &parameter_name);
template void bipl::sdnn::SDNN::GetParameter(std::vector<unsigned int> &out_parameter, const std::string &parameter_name);
