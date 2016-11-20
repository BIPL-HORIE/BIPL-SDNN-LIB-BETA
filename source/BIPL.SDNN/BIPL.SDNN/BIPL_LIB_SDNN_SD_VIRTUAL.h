#pragma once
#include <string>
#include <memory>
#include <BIPL_LIB_PARAMETERS_JUDGEMENT.h>

namespace bipl
{
	namespace parameters
	{
		//! �p�����[�^�̑O���錾
		class PARAMETERS;
	}
	namespace sdnn
	{
		namespace base
		{
			//! �p�^�[���̑O���錾
			class SDNN_PC;

			/*!
			@brief SDNN��SD�����CPC�̊Ǘ��N���X�����˂�D
			@note ���͉��񏑂������Ă���̂��낤���D�D�D
			@author Kazumasa HORIE
			@date 2015/12/15~
			@version ��1.0�i12/26�F�u���b�N�{�b�N�X�e�X�g�I���j
			*/
			class SDNN_SD_VIRTUAL
			{
			private:
				//! �R�s�[�R���X�g���N�^�͋֎~
				SDNN_SD_VIRTUAL(const SDNN_SD_VIRTUAL&) = delete;
				//! ������Z���֎~
				void operator=(const SDNN_SD_VIRTUAL&) = delete;

			protected:
				//! ���͐�
				unsigned int input_number_;
				//! input��ʎq����������
				std::vector<int> quantized_input_;
				//! input�̎��
				enum INPUT_TYPE_
				{
					NUMERICAL = 0,
					SYMBOL = 1,
				};
				std::vector<INPUT_TYPE_> input_type_;
				std::vector<int> q_;

				//! �s�����̌��ʓ�����s�����p�^�[���̑f�q�̐�
				int desensitized_pattern_neuron_number_;

				//! �s������
				unsigned int desensitize_number_;

				//! �s���������̋�̓I�Ȓ��g�D�֐��|�C���^���X�g���g���ė~�����Ȃ����߂ɂ킴��virtual�ɂ��Ă��Ȃ��D
				void SelectiveDesensitizationAstarisk(int* out_adress_data_list, const double *input);
				//! �s���������̊֐��|�C���^
				void (SDNN_SD_VIRTUAL::*SelectiveDesensitizationPointer)(int* out_adress_data_list, const double *input);

			public:
				//! �R���X�g���N�^
				SDNN_SD_VIRTUAL(void);
				//! �f�R���X�g���N�^
				virtual ~SDNN_SD_VIRTUAL(void);

				/*! @brief SD�̏�����
				�ݒ肷�ׂ��p�����[�^
				SDNN\\input_number:���͐�
				SDNN\\SD\\method(mutual|file):�s�����̑g�ݍ��킹�̌�����@
				SDNN\\SD\\file(method == file�̏ꍇ):�Q�Ƃ��ׂ��t�@�C����
				@param[in,out] sdnn_parameter �p�����[�^
				@return �Ȃ��D
				*/
				virtual void InitSD(parameters::PARAMETERS &sdnn_parameter);

				/*! @brief �I��I�s�����̎��s
				@param[out] out_adress_data_list �o�̓A�h���X�E�f�[�^���X�g
				@param[in] input SDNN�ւ̓���
				@return �Ȃ��D
				*/
				void SelectiveDesensitization(int* out_adress_data_list, const double* input) { (this->*SelectiveDesensitizationPointer)(out_adress_data_list, input); }

				/*! @brief �s�����ς݃p�^�[���̑f�q��
				@return �s�����ς݃p�^�[���̑f�q��=desensitized_pattern_neuron_number�D
				*/
				int GetSDPatternNeuronNumber(void);

				/*! @brief �s�������X�g�̕ԋp
				@return �s�������X�g [(S1,C1),(S2,C2),,,,]
				*/
				virtual std::string GetDesensitizationList(void);
			};

			//! �s���������̊֐��|�C���^�Ɍp�����ꂽ�N���X����A�N�Z�X�ł���悤�Ɍ^��`�D
			typedef void(SDNN_SD_VIRTUAL::*SelectiveDesensitization_callback)(int* out_adress_data_list, const double *input);
		}
	}
}