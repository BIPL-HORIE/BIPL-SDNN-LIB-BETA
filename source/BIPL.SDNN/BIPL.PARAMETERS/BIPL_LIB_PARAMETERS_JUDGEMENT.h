#pragma once
#include <vector>
#include <string>

namespace bipl
{
	namespace parameters
	{
		class PARAMETERS;

		/*!
		@brief �p�����[�^�������Ɉ�v���邩�ǂ����̔��莮�N���X�̖��O���
		@author Kazumasa HORIE
		@date 2015/1/26~
		*/
		namespace judgement
		{
			/*!
			@brief ���莮�N���X�̏������z�֐�
			@author Kazumasa HORIE
			@date 2015/1/26~
			*/
			class JUDGEMENT
			{
			private:
				//! ����𖞂����p�����[�^�̃��X�g�C����
				std::string list_;
			public:
				//! �R���X�g���N�^
				JUDGEMENT(const std::string &list) :list_(list) {}
				JUDGEMENT(const std::vector<std::string> &list) {
					for (unsigned int i = 0; i < list.size(); i++)
						list_ += list[i] + "\n";
				}
				//! �f�R���X�g���N�^
				virtual ~JUDGEMENT() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return �G���[�D���̊֐��͌Ăяo����Ȃ��͂��D
				*/
				virtual bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
				void coutList(void)const;
				std::string GetList(void)const { return list_; };
			};

			/*!
			@brief �p�����[�^�͓���͈͓��ɓ��邩�ۂ�
			parameter >= min && parameter <= max
			����������̂Œ���
			@author Kazumasa HORIE
			*/
			template<typename content_type>
			class JUDGEMENT_IS_IN_RANGE :public JUDGEMENT
			{
			private:
				//! �ŏ��l
				content_type min_;
				//! �ő�l
				content_type max_;
			public:
				//! �R���X�g���N�^
				JUDGEMENT_IS_IN_RANGE(content_type min, content_type max);
				//! �f�R���X�g���N�^
				~JUDGEMENT_IS_IN_RANGE() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	(parameter >= min && parameter <= max)
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief �p�����[�^�͋K��l�ȏォ�ۂ�
			parameter >= min
			@author Kazumasa HORIE
			*/
			template<typename content_type>
			class JUDGEMENT_IS_BIGGER_THAN :public JUDGEMENT
			{
			private:
				//! �ŏ��l
				content_type min_;
			public:
				//! �R���X�g���N�^
				JUDGEMENT_IS_BIGGER_THAN(content_type min);
				//! �f�R���X�g���N�^
				~JUDGEMENT_IS_BIGGER_THAN() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	(parameter >= min)
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief �p�����[�^���͈͂��Ӗ�����x�N�g�����ۂ�
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_IS_RANGE :public JUDGEMENT
			{
			public:
				//! �R���X�g���N�^
				JUDGEMENT_IS_RANGE();
				//! �f�R���X�g���N�^
				~JUDGEMENT_IS_RANGE() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	�i2�v�f�x�N�g���H�j&&�i���v�f�����v�f�H�j
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief �p�����[�^�Ɠ����̃t�@�C�������݂��邩�ǂ���
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_FILE_EXIST :public JUDGEMENT
			{
			public:
				//! �R���X�g���N�^
				JUDGEMENT_FILE_EXIST();
				//! �f�R���X�g���N�^
				~JUDGEMENT_FILE_EXIST() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	�i�t�@�C�������݂����1�C���Ȃ����0�j
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief �p�����[�^���C�K��̒l�̂ǂꂩ�ł��邩�ǂ���
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_STRING_LIST :public JUDGEMENT
			{
			private:
				//! �p�����[�^�����ׂ��l���X�g
				std::vector<std::string> judgement_list_;
			public:
				//! �R���X�g���N�^
				JUDGEMENT_STRING_LIST(const std::vector<std::string> judgement_list);
				//! �f�R���X�g���N�^
				~JUDGEMENT_STRING_LIST() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	�i�t�@�C�������݂����1�C���Ȃ����0�j
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief �p�����[�^���C"Y","N"�̂ǂ��炩�ł��邩�ǂ���
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_YN :public JUDGEMENT
			{
			public:
				//! �R���X�g���N�^
				JUDGEMENT_YN();
				//! �f�R���X�g���N�^
				~JUDGEMENT_YN() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	�i�t�@�C�������݂����1�C���Ȃ����0�j
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief �p�����[�^���C�x�N�g���ł����܂����v�f�����ǂ���
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_VECTOR_SIZE :public JUDGEMENT
			{
			private:
				//! �x�N�g���̎��ׂ��v�f��
				int vector_size_;
			public:
				//! �R���X�g���N�^
				JUDGEMENT_VECTOR_SIZE(int vector_size);
				//! �f�R���X�g���N�^
				~JUDGEMENT_VECTOR_SIZE() {}
				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	�i�p�����[�^�̓x�N�g���H�j&&�i�v�f�����K��l�H�j
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;

			};
			/*!
			@brief �p�����[�^�������̃V�[�h�l���ǂ���
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_RANDOMSEED :public JUDGEMENT
			{
			public:
				//! �R���X�g���N�^
				JUDGEMENT_RANDOMSEED() :JUDGEMENT("String \"random_device\", or a unsigned integer array ([uint1, uint2,,,])") {}
				//! �f�R���X�g���N�^
				~JUDGEMENT_RANDOMSEED() {}

				/*! @brief ���莮
				@param[in] parameters ���肷��p�����[�^���X�g
				@param[in] parameter_name �p�����[�^��
				@param[in] default_parameter_content �p�����[�^�K��l
				@return	�p�����[�^��random_device���Cunsigned int�x�N�g���Ȃ�true
				*/
				bool judgement(parameters::PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};
		}
	}
}

