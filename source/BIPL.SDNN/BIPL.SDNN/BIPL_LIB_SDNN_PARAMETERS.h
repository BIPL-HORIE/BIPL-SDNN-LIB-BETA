#pragma once

#include<string>
#include<vector>
#include<memory>
#include<BIPL_LIB_PARAMETERS_JUDGEMENT.h>
#include<BIPL_LIB_PARAMETERS.h>

namespace bipl
{
	namespace sdnn
	{
		namespace parameter_property
		{
			class PARAM_PROPERTY_SET
			{
			public:
				PARAM_PROPERTY_SET(std::string name, std::string def, std::unique_ptr<parameters::judgement::JUDGEMENT> judgement) :name_(name), default_(def), judgement_(std::move(judgement)) {}
				const std::string name_;
				const std::string default_;
				std::unique_ptr<parameters::judgement::JUDGEMENT> judgement_;
				template<typename content_type>
				void Read(content_type &output, parameters::PARAMETERS &parameter)
				{
					parameter.ReadParameter(output, name_, default_, judgement_.get());
				}
				std::string Check(parameters::PARAMETERS &parameter)
				{
					return (!judgement_->judgement(&parameter, name_, default_)) ? "Parameter<" + name_ + "> is incorrect.\nYou can set the following parameters.\n" + judgement_->GetList() : "";
				}
			};

			class PARAM_INPUT_NUMBER/**/
			{
			private:
				PARAM_INPUT_NUMBER() = delete;
			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_SD_METHOD
			{
			private:
				PARAM_SD_METHOD() = delete;
			public:
				static PARAM_PROPERTY_SET property_;
				class CNT_SD_METHOD
				{
				private:
					CNT_SD_METHOD() = delete;
				public:
					static const std::string mutual_;
					static const std::string file_;
					static const std::string saved_;
					static const std::vector<std::string> list_;
				};

				class PARAM_FILE //file_
				{
				private:
					PARAM_FILE() = delete;
				public:
					static PARAM_PROPERTY_SET property_;
				};
				
				class PARAM_LINE //saved
				{
				private:
					PARAM_LINE() = delete;
				public:
					static PARAM_PROPERTY_SET property_;
				};
			};

			class PARAM_SD_PC_N/**/
			{
			private:
				PARAM_SD_PC_N() = delete;

			public:
				static PARAM_PROPERTY_SET property_;

				/*!
				@brief �p�����[�^��2�ȏ�̋������ǂ�������
				@author Kazumasa HORIE
				*/
				class JUDGEMENT_N_ :public bipl::parameters::judgement::JUDGEMENT
				{
				public:
					//! �R���X�g���N�^
					JUDGEMENT_N_() :bipl::parameters::judgement::JUDGEMENT("2�ȏ�̋����C�v�Z�ʂ̋�������傫�����邱�Ƃ𐄏�") {}
					//! �f�R���X�g���N�^
					~JUDGEMENT_N_() {}
					/*! @brief ���莮
					@param[in] parameters ���肷��p�����[�^���X�g
					@param[in] parameter_name �p�����[�^��
					@param[in] default_parameter_content �p�����[�^�K��l
					@return	(parameter%2 == 0) && (parameter >= 2)
					*/
					bool judgement(bipl::parameters::PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
				};
			};

			class PARAM_SD_PC_TYPE/**/
			{
			private:
				PARAM_SD_PC_TYPE() = delete;

			public:
				class CNT_INPUT_TYPE_
				{
				private:
					CNT_INPUT_TYPE_() = delete;
				public:
					static const std::string numerical_;
					static const std::string symbol_;
				};

				class CNT_PC_METHOD_
				{
				private:
					CNT_PC_METHOD_() = delete;
				public:
					static const std::string random_inverse_;
					static const std::string interpolation_;
					static const std::string correlation_tree_;
				};

				class JUDGEMENT_PC_TYPE_ :public bipl::parameters::judgement::JUDGEMENT
				{
				private:
					int input_number_;
					int n_;
				public:
					//! �R���X�g���N�^
					JUDGEMENT_PC_TYPE_(int input_number, int n) :bipl::parameters::judgement::JUDGEMENT(
						"String vector whose element is NUMERICAL(RANDOM_INVERSE(r)) (1 =< r < n/2), NUMERICAL(INTERPOLATION(r)) (r > 2 && n % r == 0), SYMBOL(RANDOM_INVERSE(r)) (1 =< r < n/2),or SYMBOL(CORRELATION_TREE(filaname)),\n the number of elements have to equal to the number of inputs.") {
						input_number_ = input_number; n_ = n;
					}
					//! �f�R���X�g���N�^
					~JUDGEMENT_PC_TYPE_() {}
					/*! @brief ���莮
					@param[in] parameters ���肷��p�����[�^���X�g
					@param[in] parameter_name �p�����[�^��
					@param[in] default_parameter_content �p�����[�^�K��l
					@return	(parameter%2 == 0) && (parameter >= 2)
					*/
					bool judgement(bipl::parameters::PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
				};
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_SD_PC_RANDOM_SEED
			{
			private:
				PARAM_SD_PC_RANDOM_SEED() = delete;

			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_TYPE/**/
			{
			private:
				PARAM_NN_TYPE() = delete;

			public:
				class CNT_NN_TYPE_
				{
				private:
					CNT_NN_TYPE_() = delete;
				public:
					static const std::string pp_;
					static const std::string sp_;
					static const std::vector<std::string> list_;
				};
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_TRAINING_END/**/
			{
			private:
				PARAM_NN_TRAINING_END() = delete;
			public:
				class JUDGEMENT_TRAIN_METHOD_ :public bipl::parameters::judgement::JUDGEMENT
				{
				public:
					JUDGEMENT_TRAIN_METHOD_() :JUDGEMENT("example:\nrmse(rmse_threashold, max number of iterations)\nfor(specic number of iterations)\nrmse(0.01,100)+for(10)") {}
					~JUDGEMENT_TRAIN_METHOD_() {}
					bool judgement(bipl::parameters::PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
				};
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_PP_OUTPUT_RANGE/**/
			{
			private:
				PARAM_NN_PP_OUTPUT_RANGE() = delete;
			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE/**/
			{
			private:
				PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE() = delete;
			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD/**/
			{
			private:
				PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD() = delete;

			public:
				class CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_
				{
				private:
					CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_() = delete;
				public:
					static const std::string ovo_;
					static const std::string ovr_;
					static const std::vector<std::string> list_;
				};
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_SP_CATEGORY_NUMBER/**/
			{
			private:
				PARAM_NN_SP_CATEGORY_NUMBER() = delete;
			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_RANDOM_SEED
			{
			private:
				PARAM_NN_RANDOM_SEED() = delete;

			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_NN_INITIAL_VALUE_RANGE/**/
			{
			private:
				PARAM_NN_INITIAL_VALUE_RANGE() = delete;

			public:
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_OPTION_PRINT_PROGRESSION/**/
			{
			private:
				PARAM_OPTION_PRINT_PROGRESSION() = delete;
			public:
				class CNT_OPTION_PRINT_PROGRESSION //YN
				{
				private:
					CNT_OPTION_PRINT_PROGRESSION() = delete;
				public:
					static const std::string y_;
					static const std::string n_;
					static const std::vector<std::string> list_;
				};
				static PARAM_PROPERTY_SET property_;
			};

			class PARAM_OPTION_MULTI_THREAD_USE/**/
			{
			private:
				PARAM_OPTION_MULTI_THREAD_USE() = delete;
			public:
				class CNT_OPTION_MULTI_THREAD_USE //YN
				{
				private:
					CNT_OPTION_MULTI_THREAD_USE() = delete;
				public:
					static const std::string y_;
					static const std::string n_;
					static const std::vector<std::string> list_;
				};
				static PARAM_PROPERTY_SET property_;

				class PARAM_OPTION_MULTI_THREAD_NUMBER
				{
				private:
					PARAM_OPTION_MULTI_THREAD_NUMBER() = delete;
				public:
					static PARAM_PROPERTY_SET property_;
				};
			};

			class PARAM_OPTION_SAVE_FILENAME
			{
			private:
				PARAM_OPTION_SAVE_FILENAME() = delete;
			public:
				static PARAM_PROPERTY_SET property_;
			};
		}
	}
}