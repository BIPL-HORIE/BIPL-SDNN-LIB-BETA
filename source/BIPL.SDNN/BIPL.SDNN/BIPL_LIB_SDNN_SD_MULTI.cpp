#include "BIPL_LIB_SDNN_SD_MULTI.h"

#include <BIPL_LIB_PARAMETERS.h>
#include <BIPL_LIB_PARAMETERS_JUDGEMENT.h>
#include <BIPL_LIB_LEXIAL.h>
#include <BIPL_LIB_UTILITY.h>
#pragma comment(lib,"BIPL.PARAMETERS.lib")

#include "BIPL_LIB_SDNN_PC.h"

#include <iostream>
#include <fstream>


bipl::sdnn::base::sd::SDNN_SD_MULTI::SDNN_SD_MULTI() :pattern_c_(nullptr), pattern_s_(nullptr), s_list_(nullptr), c_list_(nullptr), n_(0), q_(0), n32_(0)
{
	SDNN_SD_VIRTUAL::SelectiveDesensitizationPointer = static_cast<SelectiveDesensitization_callback>(&SDNN_SD_MULTI::SelectiveDesensitizationAstarisk);
}

bipl::sdnn::base::sd::SDNN_SD_MULTI::~SDNN_SD_MULTI()
{
	if (nullptr != pattern_s_)
		delete[]pattern_s_;
	if (nullptr != pattern_c_)
	{
		for (unsigned int i = 0; i < desensitize_number_; i++)
			delete[]pattern_c_[i];
		delete[]pattern_c_;
	}
	if (nullptr != s_list_)
		delete[]s_list_;
	if (nullptr != c_list_)
	{
		delete[]c_list_;
	}
}


void bipl::sdnn::base::sd::SDNN_SD_MULTI::InitSD(parameters::PARAMETERS &sdnn_parameter)
{
	parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int> judgement_input_number(2);
	sdnn_parameter.ReadParameterWithJudgement(input_number_, "SDNN\\input_number", judgement_input_number, "2");

	JUDGEMENT_N_ judgement_n;
	sdnn_parameter.ReadParameterWithJudgement(n_, "SDNN\\SD\\PC\\n", judgement_n, "32");

	parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<int> judgement_q(1);
	sdnn_parameter.ReadParameterWithJudgement(q_, "SDNN\\SD\\PC\\q", judgement_q, "1");

	n32_ = ((n_ - 1) >> 5) + 1;

	quantized_input_ = std::make_unique<int[]>(input_number_);

	//methodごとにPatternとlistを作成．
	std::string method;
	std::vector<std::string> list_method;
	list_method.push_back("file");
	parameters::judgement::JUDGEMENT_STRING_LIST judgement_method(list_method);

	sdnn_parameter.ReadParameterWithJudgement(method, "SDNN\\SD\\method", judgement_method, list_method[0]);
	if (method == list_method[0])
	{
		std::ifstream sd_file;
		std::string sd_filename;
		parameters::judgement::JUDGEMENT_FILE_EXIST judgement_file_exist;
		sdnn_parameter.ReadParameterWithJudgement(sd_filename, "SDNN\\SD\\filename", judgement_file_exist);
		sd_file.open(sd_filename);
		MakePatternAndListWithFile(sd_file);
	}
	else
	{
		utility::error::BugFound(0x9832);
	}

	//パターンの初期化
	std::mt19937 mt;
	bipl::utility::InitMt(mt, sdnn_parameter, "SDNN\\SD\\PC\\random_seed");

	std::vector<std::string> init_type;
	sdnn_parameter.ReadParameter(init_type, "SDNN\\SD\\PC\\type", "[]");

	std::vector<unsigned int> *c_list_buffer = c_list_;
	unsigned int *s_list_buffer = s_list_;
	bipl::sdnn::base::SDNN_PC **pattern_c_buffer = pattern_c_;
	bipl::sdnn::base::SDNN_PC *pattern_s_buffer = pattern_s_;

SDNN_LIB_LABEL_SDNN_SD_MULTIInitSD:
	try {
		if (init_type.size() < input_number_)
		{
			if (init_type[0] == "")
				init_type[0] = "RANDOM_INVERSE(1)";

			std::cout << "typeが足りません．RANDOM_INVERSE(1)で仮定します" << std::endl;
			while (init_type.size() < input_number_)
			{
				init_type.push_back("RANDOM_INVERSE(1)");
			}
			sdnn_parameter.OverwriteParameter("SDNN\\SD\\PC\\type", init_type);
		}
		else if (init_type.size() > input_number_)
			throw "ベクトルの要素数と入力数が一致しません．";

		s_list_buffer = s_list_;
		pattern_s_buffer = pattern_s_;
		for (unsigned int d_counter = 0; d_counter < desensitize_number_; d_counter++)
		{
			for (unsigned int c_counter = 0; c_counter < c_list_[d_counter].size(); c_counter++)
				pattern_c_buffer[d_counter][c_counter].InitPC(n_,q_,init_type[c_list_[d_counter][c_counter]], mt);
			pattern_s_buffer++->InitPC(n_, q_, init_type[*s_list_buffer++], mt);
		}
	}
	catch (const char* str)
	{
		std::cout << "typeの設定に誤りがあります．" << std::endl;
		std::cout << str << std::endl;
		std::cout << "再設定をしてください " << std::endl;
		std::string buffer;
		std::cin >> buffer;
		std::cin.ignore();
		sdnn_parameter.OverwriteParameter("SDNN\\SD\\PC\\type", buffer);
		sdnn_parameter.ReadParameter(init_type, "SDNN\\SD\\PC\\type", "[]");
		goto SDNN_LIB_LABEL_SDNN_SD_MULTIInitSD;
	}

	pattern_s_buffer = pattern_s_;
	desensitized_pattern_neuron_number_ = 0;
	for (unsigned int d_counter = 0; d_counter < desensitize_number_; d_counter++)
	{
		desensitized_pattern_neuron_number_ += pattern_s_buffer++->GetN();
	}
}

void bipl::sdnn::base::sd::SDNN_SD_MULTI::MakePatternAndListWithFile(std::ifstream &sd_file)
{
	std::string line_buffer;

	desensitize_number_ = 0;

	while (getline(sd_file, line_buffer))
	{
		if (line_buffer != "")
			desensitize_number_++;
	}

	try
	{
		s_list_ = new unsigned int[desensitize_number_];
		c_list_ = new std::vector<unsigned int>[desensitize_number_];
		pattern_s_ = new sdnn::base::SDNN_PC[desensitize_number_];
		pattern_c_ = new sdnn::base::SDNN_PC*[desensitize_number_];
	}
	catch (std::bad_alloc)
	{
		utility::error::AllocationError(0x12312454);
	}

	sd_file.clear();
	sd_file.seekg(0);

	std::vector<std::string> sd_buffer;
	int counter = 0;
	while (getline(sd_file, line_buffer))
	{
		if (line_buffer == "")
			continue;
		sd_buffer.clear();
		parameters::lexial::Split(sd_buffer, line_buffer, ",");
		if (sd_buffer.size() <= 1)
			throw "sd define error";

		s_list_[counter] = std::stoul(sd_buffer[0]);
		for (unsigned int i = 1; i < sd_buffer.size(); i++)
			c_list_[counter].push_back(std::stoul(sd_buffer[i]));
		pattern_c_[counter] = new sdnn::base::SDNN_PC[c_list_[counter].size()];
		counter++;
	}
}

void bipl::sdnn::base::sd::SDNN_SD_MULTI::SelectiveDesensitizationAstarisk(int* out_adress_data_list, const double* input)
{
/*	{
		const double *input_buffer = input;
		int * quantized_input_buffer = quantized_input_;
		for (unsigned int i_counter = 0; i_counter < input_number_; i_counter++)
		{
			//ここたぶんもっと早くできる．
			*quantized_input_buffer = static_cast<int>(q_**input_buffer++);
			*quantized_input_buffer++ = (*quantized_input_buffer >= q_) ? q_ - 1 : (*quantized_input_buffer < 0) ? 0 : *quantized_input_buffer;
		}
	}
	
	unsigned int *s_list_buffer = s_list_;
	std::vector<unsigned int> *c_list_buffer = c_list_;

	int *out_adress_data_list_buffer = out_adress_data_list + 1;

	SDNN_PC *pattern_s_buffer = pattern_s_;
	SDNN_PC **pattern_c_buffer = pattern_c_;

	unsigned int *pattern_s_uint_buffer;
	unsigned int *pattern_c_uint_buffer;
	unsigned int pattern_s_content;
	unsigned int pattern_c_content;

	*out_adress_data_list_buffer = 1;
	for (unsigned int d_counter = 0; d_counter < desensitize_number_; d_counter++)
	{
		pattern_s_uint_buffer = pattern_s_buffer++->GetPatternBinary(quantized_input_[*s_list_buffer++]);


		for (int n32_counter = 0; n32_counter < n32_ - 1; n32_counter++)
		{
			pattern_s_content = *pattern_s_uint_buffer++;
			pattern_c_content = *pattern_c_uint_buffer++;
			for (int counter32 = 0; counter32 < 32; counter32++)
			{
				if (pattern_c_content & 0x1)
				{
					*++out_adress_data_list_buffer = 2 * (pattern_s_content & 0x1) - 1;
					*++out_adress_data_list_buffer = 1;
				}
				else
				{
					(*out_adress_data_list_buffer)++;
				}
				pattern_s_content >>= 1;
				pattern_c_content >>= 1;
			}
		}
		pattern_s_content = *pattern_s_uint_buffer++;
		pattern_c_content = *pattern_c_uint_buffer++;
		for (int remain_counter = 0; remain_counter < n_ - (n32_ - 1) * 32; remain_counter++)
		{
			if (pattern_c_content & 0x1)
			{
				*++out_adress_data_list_buffer = 2 * (pattern_s_content & 0x1) - 1;
				*++out_adress_data_list_buffer = 1;
			}
			else
			{
				(*out_adress_data_list_buffer)++;
			}
			pattern_s_content >>= 1;
			pattern_c_content >>= 1;
		}
	}
	*out_adress_data_list = adress_data_list_length_per2;*/
}
