#include "BIPL_LIB_SDNN_PARAMETERS.h"
#include <BIPL_LIB_PARAMETERS.h>
#include <BIPL_LIB_LEXIAL.h>
#include <fstream>
#include <omp.h>
using namespace bipl::sdnn::parameter_property;

//問題の種類
const std::string PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::pattern_recognition_("pattern_recognition");
const std::string PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::function_approximation_("function_approximation");
const std::vector<std::string> PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::list_({pattern_recognition_,function_approximation_});
PARAM_PROPERTY_SET PARAM_SDNN_TYPE::property_("ISSUE\\type", CNT_SDNN_TYPE_::function_approximation_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_SDNN_TYPE_::list_));

//FAの場合
//SDNNの出力幅
PARAM_PROPERTY_SET PARAM_NN_PP_OUTPUT_RANGE::property_("ISSUE\\FA\\output_range", "[0, 1]", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_RANGE>());
//SDNNの出力刻み
PARAM_PROPERTY_SET PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE::property_("ISSUE\\FA\\required_step_size", "0,01", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<double>>(0.0001, 1));

//PRの場合
//カテゴリ数
PARAM_PROPERTY_SET PARAM_NN_SP_CATEGORY_NUMBER::property_("ISSUE\\PR\\category_number", "2", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int>>(2));

//多クラス近似手法
const std::string PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovo_("1v1");
const std::string PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovr_("1vR");
const std::vector<std::string> PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::list_({ ovo_,ovr_ });
PARAM_PROPERTY_SET PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::property_("ISSUE\\PR\\multi_class_recognition", CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovo_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::list_));

//入力素子数
PARAM_PROPERTY_SET PARAM_INPUT_NUMBER::property_("ISSUE\\input_number", "2", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int>>(2));

//PC
//パターンコーディングのn
PARAM_PROPERTY_SET PARAM_SD_PC_N::property_("SDNN\\PC\\n", "128", std::make_unique<PARAM_SD_PC_N::JUDGEMENT_N_>());
bool PARAM_SD_PC_N::JUDGEMENT_N_::judgement(bipl::parameters::PARAMETERS *parameters, std::string parameter_name, std::string default_parameter_content)const
{
	int n;
	parameters->ReadParameter(n, parameter_name, default_parameter_content);
	return (n >= 2 && (n % 2 == 0));
}

PARAM_PROPERTY_SET PARAM_SD_PC_TYPE::property_("SDNN\\PC\\input_type_and_creation_method", "[]", std::make_unique<bipl::sdnn::parameter_property::PARAM_SD_PC_TYPE::JUDGEMENT_PC_TYPE_>(0,0));
const std::string PARAM_SD_PC_TYPE::CNT_INPUT_TYPE_::numerical_("NUMERICAL");
const std::string PARAM_SD_PC_TYPE::CNT_INPUT_TYPE_::symbol_("SYMBOLIC");
const std::string PARAM_SD_PC_TYPE::CNT_PC_METHOD_::random_inverse_("RANDOM_INVERSE");
const std::string PARAM_SD_PC_TYPE::CNT_PC_METHOD_::correlation_matrix_("CORRELATION_MATRIX");
const std::string PARAM_SD_PC_TYPE::CNT_PC_METHOD_::saved_("SAVED");

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

			if (type[1] == CNT_PC_METHOD_::random_inverse_)
			{
				std::vector<std::string> q_r;
				bipl::lexial::Split(q_r, type[2], ',');
				try
				{
					int q = stoi(q_r[0]);
					if (q <= 0)
						return 0;
				}
				catch (...)
				{
					return 0;
				}
				try
				{
					int r = std::stoi(q_r[1]);
					if (r <= 0 || r >= n_ / 2)
						return 0;
				}
				catch (...)
				{
					return 0;
				}
			}
			else if (type[1] == CNT_PC_METHOD_::correlation_matrix_)
			{
				std::vector<std::string> f_b_i_p;
				bipl::lexial::Split(f_b_i_p, type[2], ',');

				try
				{
					int b = stoi(f_b_i_p[1]);
					if (b <= 0)
						return 0;
					int i = stoi(f_b_i_p[2]);
					if (i <= 0)
						return 0;
					double p = stof(f_b_i_p[3]);
					if (p <= 0)
						return 0;
				}
				catch (...)
				{
					return 0;
				}
			}
			else if (type[1] == CNT_PC_METHOD_::saved_)
			{
				std::vector<std::string> f_b_i_p;
				bipl::lexial::Split(f_b_i_p, type[2], ',');

				try
				{
					int b = stoi(f_b_i_p[1]);
					if (b <= 0)
						return 0;
					int i = stoi(f_b_i_p[2]);
					if (i <= 0)
						return 0;
					double p = stof(f_b_i_p[3]);
					if (p <= 0)
						return 0;
				}
				catch (...)
				{
					return 0;
				}
			}
			else
				return 0;
		}
	}
	return 1;
}

PARAM_PROPERTY_SET PARAM_SD_PC_RANDOM_SEED::property_("SDNN\\PC\\random_seed", HARDWARE_ENTROPY, std::make_unique<bipl::parameters::judgement::JUDGEMENT_RANDOMSEED>());

const std::string bipl::sdnn::parameter_property::PARAM_NN_TRAINING_END::CNT_NN_TRAINING_END_::itr_("iteration");
const std::string bipl::sdnn::parameter_property::PARAM_NN_TRAINING_END::CNT_NN_TRAINING_END_::rmse_("rmse");

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
		if (train_method_buffer[0] == CNT_NN_TRAINING_END_::itr_)
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
		else if (train_method_buffer[0] == CNT_NN_TRAINING_END_::rmse_)
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

//SD
//SD対応関係定義手法
const std::string PARAM_SD_METHOD::CNT_SD_METHOD::mutual_("mutual");
const std::string PARAM_SD_METHOD::CNT_SD_METHOD::file_("file");
const std::string PARAM_SD_METHOD::CNT_SD_METHOD::saved_("saved");
const std::vector<std::string> PARAM_SD_METHOD::CNT_SD_METHOD::list_({ mutual_, file_, saved_ });
PARAM_PROPERTY_SET PARAM_SD_METHOD::property_("SDNN\\SD\\combination_setting", CNT_SD_METHOD::mutual_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_SD_METHOD::list_));

//SD定義，ファイル指定時のファイル名
PARAM_PROPERTY_SET PARAM_SD_METHOD::PARAM_FILE::property_("SDNN\\SD\\filename", "", std::make_unique<bipl::parameters::judgement::JUDGEMENT_FILE_EXIST>());

//SD定義，saved指定時の文字列名
PARAM_PROPERTY_SET PARAM_SD_METHOD::PARAM_LINE::property_("SDNN\\SD\\line", "[(0,1),(1,0)]", nullptr);




//NNの学習方法
PARAM_PROPERTY_SET PARAM_NN_TRAINING_END::property_("SDNN\\NN\\completion_condition", "for(10)",std::make_unique<PARAM_NN_TRAINING_END::JUDGEMENT_TRAIN_METHOD_>());


PARAM_PROPERTY_SET PARAM_NN_RANDOM_SEED::property_("SDNN\\NN\\random_seed", HARDWARE_ENTROPY, std::make_unique<bipl::parameters::judgement::JUDGEMENT_RANDOMSEED>());

PARAM_PROPERTY_SET PARAM_NN_INITIAL_VALUE_RANGE::property_("SDNN\\NN\\initial_value_range", "[-5,5]", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_RANGE>());

//OPTION 進行状況表示
const std::string PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::y_("Y");
const std::string PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::n_("N");
const std::vector<std::string> PARAM_OPTION_PRINT_PROGRESSION::CNT_OPTION_PRINT_PROGRESSION::list_({y_,n_});
PARAM_PROPERTY_SET PARAM_OPTION_PRINT_PROGRESSION::property_("APP\\print_progression", CNT_OPTION_PRINT_PROGRESSION::y_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_OPTION_PRINT_PROGRESSION::list_));

//OPTION OPENMP利用
const std::string PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::y_("Y");
const std::string PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::n_("N");
const std::vector<std::string> PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::list_({ y_,n_ });
PARAM_PROPERTY_SET PARAM_OPTION_MULTI_THREAD_USE::property_("APP\\multi_thread", CNT_OPTION_MULTI_THREAD_USE::n_, std::make_unique<bipl::parameters::judgement::JUDGEMENT_STRING_LIST>(CNT_OPTION_MULTI_THREAD_USE::list_));

//thread number
PARAM_PROPERTY_SET PARAM_OPTION_MULTI_THREAD_USE::PARAM_OPTION_MULTI_THREAD_NUMBER::property_("APP\\thread_number", "1", std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<unsigned int>>(1, omp_get_num_procs()));

PARAM_PROPERTY_SET PARAM_OPTION_SAVE_FILENAME::property_("APP\\training_result_filename", "autosave.bin", nullptr);