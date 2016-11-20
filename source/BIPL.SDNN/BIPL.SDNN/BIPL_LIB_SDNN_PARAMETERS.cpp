#include "BIPL_LIB_SDNN_PARAMETERS.h"
#include <BIPL_LIB_PARAMETERS.h>
#include <BIPL_LIB_LEXIAL.h>
#include <fstream>
#include <omp.h>
using namespace bipl::sdnn::parameter_property;

//入力素子数
PARAM_PROPERTY_SET PARAM_INPUT_NUMBER::property_("SDNN\\input_number", "2", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int>>(2));

//SD対応関係定義手法
const std::string PARAM_SD_METHOD::CNT_SD_METHOD::mutual_("mutual");
const std::string PARAM_SD_METHOD::CNT_SD_METHOD::file_("file");
const std::string PARAM_SD_METHOD::CNT_SD_METHOD::saved_("saved");
const std::vector<std::string> PARAM_SD_METHOD::CNT_SD_METHOD::list_({mutual_, file_, saved_});
PARAM_PROPERTY_SET PARAM_SD_METHOD::property_("SDNN\\SD\\method", CNT_SD_METHOD::mutual_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_SD_METHOD::list_));

//SD定義，ファイル指定時のファイル名
PARAM_PROPERTY_SET PARAM_SD_METHOD::PARAM_FILE::property_("SDNN\\SD\\filename","",std::make_unique<bipl::parameters::judgement::JUDGEMENT_FILE_EXIST>());

//SD定義，saved指定時の文字列名
PARAM_PROPERTY_SET PARAM_SD_METHOD::PARAM_LINE::property_("SDNN\\SD\\line", "[(0,1),(1,0)]", nullptr);

//パターンコーディングのn
PARAM_PROPERTY_SET PARAM_SD_PC_N::property_("SDNN\\SD\\PC\\n", "100", std::make_unique<PARAM_SD_PC_N::JUDGEMENT_N_>());
bool PARAM_SD_PC_N::JUDGEMENT_N_::judgement(bipl::parameters::PARAMETERS *parameters, std::string parameter_name, std::string default_parameter_content)const
{
	int n;
	parameters->ReadParameter(n, parameter_name, default_parameter_content);
	return (n >= 2 && (n % 2 == 0));
}

PARAM_PROPERTY_SET PARAM_SD_PC_TYPE::property_("SDNN\\SD\\PC\\type", "[]", std::make_unique<bipl::sdnn::parameter_property::PARAM_SD_PC_TYPE::JUDGEMENT_PC_TYPE_>(0,0));
const std::string PARAM_SD_PC_TYPE::CNT_INPUT_TYPE_::numerical_("NUMERICAL");
const std::string PARAM_SD_PC_TYPE::CNT_INPUT_TYPE_::symbol_("SYMBOL");
const std::string PARAM_SD_PC_TYPE::CNT_PC_METHOD_::random_inverse_("RANDOM_INVERSE");
const std::string PARAM_SD_PC_TYPE::CNT_PC_METHOD_::interpolation_("INTERPOLATION");
const std::string PARAM_SD_PC_TYPE::CNT_PC_METHOD_::correlation_tree_("CORRELATION_TREE");


bool bipl::sdnn::parameter_property::PARAM_SD_PC_TYPE::JUDGEMENT_PC_TYPE_::judgement(bipl::parameters::PARAMETERS *parameters, std::string parameter_name, std::string default_parameter_content)const
{
	std::vector<std::string> pc_type;
	parameters->ReadParameter(pc_type, parameter_name, default_parameter_content);
	if (input_number_ != pc_type.size())
		return 0;
	else
	{
		std::vector<std::string> type;
		for (int i = 0; i < pc_type.size(); i++)
		{
			bipl::lexial::Split(type, pc_type[i], '(');
			if (type.size() != 3)
				return 0;

			if (type[0] != CNT_INPUT_TYPE_::numerical_ && type[0] != CNT_INPUT_TYPE_::symbol_)
				return 0;


			type[2].erase(--type[2].end());
			type[2].erase(--type[2].end());

			if (type[0] == CNT_INPUT_TYPE_::numerical_)
			{
				std::vector<std::string> q_make;
				bipl::lexial::Split(q_make, type[1], ',');
				try
				{
					int q = stoi(q_make[0]);
					if (q <= 0)
						return 0;
				}
				catch (...)
				{
					return 0;
				}

				if (q_make[1] == CNT_PC_METHOD_::random_inverse_)
				{
					int r = std::stoi(type[2]);
					if (r <= 0 || r >= n_ / 2)
						return 0;
				}
				else if (q_make[1] == CNT_PC_METHOD_::interpolation_)
				{
					int s = std::stoi(type[2]);
					if (s <= 0 || n_ % s != 0)
						return 0;
				}
				else
					return 0;
			}
			else if (type[0] == CNT_INPUT_TYPE_::symbol_)
			{
				std::vector<std::string> q_make;
				bipl::lexial::Split(q_make, type[1], ',');
				switch (q_make.size())
				{
				case 1:
					if (q_make[0] == CNT_PC_METHOD_::correlation_tree_)
					{
						std::ifstream file_buffer(type[2]);
						if (!file_buffer.is_open())
							return 0;
						file_buffer.close();
					}
					break;
				case 2:
					if (q_make[1] == CNT_PC_METHOD_::random_inverse_)
					{
						try
						{
							int q = stoi(q_make[0]);
							if (q <= 0)
								return 0;
						}
						catch (...)
						{
							return 0;
						}
						int r = std::stoi(type[2]);
						if (r <= 0 || r >= n_ / 2)
							return 0;
					}
					break;
				default:
					return 0;
				}
			}
			else
				return 0;
		}
	}
	return 1;
}

PARAM_PROPERTY_SET PARAM_SD_PC_RANDOM_SEED::property_("SDNN\\SD\\PC\\random_seed", RANDOM_DEVICE, std::make_unique<bipl::parameters::judgement::JUDGEMENT_RANDOMSEED>());

const std::string PARAM_NN_TYPE::CNT_NN_TYPE_::pp_("PP");
const std::string PARAM_NN_TYPE::CNT_NN_TYPE_::sp_("SP");
const std::vector<std::string> PARAM_NN_TYPE::CNT_NN_TYPE_::list_({ pp_,sp_ });
PARAM_PROPERTY_SET PARAM_NN_TYPE::property_("SDNN\\NN\\type", "", std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_NN_TYPE_::list_));

bool PARAM_NN_TRAINING_END::JUDGEMENT_TRAIN_METHOD_::judgement(bipl::parameters::PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::string train_method;
	std::vector<std::string> train_method_splitter;
	parameters->ReadParameter(train_method, parameter_name, default_parameter_content);
	bipl::lexial::Split(train_method_splitter, train_method, '+');
	
	for (unsigned int t_counter = 0; t_counter < train_method_splitter.size(); t_counter++)
	{
		std::vector<std::string> train_method_buffer;
		lexial::Split(train_method_buffer, train_method_splitter[t_counter], '(');
	
		if (train_method_buffer.size() != 2)
			return false;
		if (train_method_buffer[0] == "for")
		{
			train_method_buffer[1].erase(--train_method_buffer[1].end());
			try
			{
				unsigned int count = stoul(train_method_buffer[1]);
			}
			catch (...)
			{
				return false;
			}
		}
		else if (train_method_buffer[0] == "rmse")
		{
			train_method_buffer[1].erase(--train_method_buffer[1].end());
			std::vector<std::string> argument;
			lexial::Split(argument, train_method_buffer[1], ',');
			if (argument.size() != 2)
				return false;
			try
			{
				double rmse = stof(argument[0]);
				unsigned int count = stoul(argument[1]);
				if (rmse <= 0)
					return false;
			}
			catch (...)
			{
				return false;
			}
		}
		else
			return false;
	}
	return true;
}

//NNの学習方法
PARAM_PROPERTY_SET PARAM_NN_TRAINING_END::property_("SDNN\\NN\\train_method", "for(10)",std::make_unique<PARAM_NN_TRAINING_END::JUDGEMENT_TRAIN_METHOD_>());

//PPの出力幅
PARAM_PROPERTY_SET PARAM_NN_PP_OUTPUT_RANGE::property_("SDNN\\NN\\output_range", "[-5,5]", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_RANGE>());

PARAM_PROPERTY_SET PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE::property_("SDNN\\NN\\output_quantization_step_size","0,01",std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<double>>(0.0001,1));

const std::string PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovo_("1v1");
const std::string PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovr_("1vR");
const std::vector<std::string> PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::list_({ ovo_,ovr_ });

PARAM_PROPERTY_SET PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::property_("SDNN\\NN\\multi_class_recognition_method", CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovo_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::list_));

PARAM_PROPERTY_SET PARAM_NN_SP_CATEGORY_NUMBER::property_("SDNN\\NN\\category_number", "2", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int>>(2));

PARAM_PROPERTY_SET PARAM_NN_RANDOM_SEED::property_("SDNN\\NN\\random_seed", RANDOM_DEVICE, std::make_unique<bipl::parameters::judgement::JUDGEMENT_RANDOMSEED>());

PARAM_PROPERTY_SET PARAM_NN_INITIAL_VALUE_RANGE::property_("SDNN\\NN\\initial_value_range", "[-5,5]", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_RANGE>());

//OPTION 進行状況表示
const std::string PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::y_("Y");
const std::string PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::n_("N");
const std::vector<std::string> PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::list_({y_,n_});
PARAM_PROPERTY_SET PARAM_OPTION_PRINT_PROGRESSION::property_("SDNN\\OPTION\\print_progression", CNT_OPTION_PRINT_PROGRESSION::y_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_OPTION_PRINT_PROGRESSION::list_));

//OPTION OPENMP利用
const std::string PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::y_("Y");
const std::string PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::n_("N");
const std::vector<std::string> PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::list_({ y_,n_ });
PARAM_PROPERTY_SET PARAM_OPTION_MULTI_THREAD_USE::property_("SDNN\\OPTION\\MULTI_THREAD\\use", CNT_OPTION_MULTI_THREAD_USE::y_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_OPTION_MULTI_THREAD_USE::list_));

//thread number
PARAM_PROPERTY_SET PARAM_OPTION_MULTI_THREAD_USE::PARAM_OPTION_MULTI_THREAD_NUMBER::property_("SDNN\\OPTION\\MULTI_THREAD\\thread_number", "1", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<unsigned int>>(1, omp_get_num_procs()));

PARAM_PROPERTY_SET PARAM_OPTION_SAVE_FILENAME::property_("SDNN\\OPTION\\save_filename", "autosave.bin", nullptr);