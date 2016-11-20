#pragma once
#include "BIPL_LIB_SDNN_NN_VIRTUAL.h"
#include <vector>
namespace bipl
{
	namespace parameters
	{
		class PARAMETERS;
	}
	namespace sdnn
	{
		namespace base
		{
			namespace nn
			{
				class SDNN_NN_PP :public SDNN_NN_VIRTUAL
				{
				private:
					//! �j���[�����̓����d�ʂ��Ⴂ�ق���I������֐��N���X
					class CHOOSE_LESS_POTENTIAL;

					//! output = a_*(1_neuron_number) + b_;
					double a_;
					double b_;
				
					/*! @brief �o�͌���֐��̒��g(�P�ƃX���b�h)
					@param[in] adress_data_list �A�h���X�E�f�[�^���X�g
					@return ���肵���o��
					*/
					double DetermineOutputAstarisk(int *adress_data_list);

					/*! @brief �o�͌���֐��̒��g(OpenMP�ɂ��}���`�R�A)
					@param[in] adress_data_list �A�h���X�E�f�[�^���X�g
					@return ���肵���o��
					*/
					double DetermineOutputAstarisk_MultiThread(int *adress_data_list);

					/*! @brief �o�͌���֐��̒��g(C++11thread�ɂ��}���`�R�A)
					@param[in] adress_data_list �A�h���X�E�f�[�^���X�g
					@return ���肵���o��
					@warning ���܂������Ă��Ȃ��D�vthread�ɂ��Ă̊w�K
					*/
//					double DetermineOutputAstarisk_MultiThread_C11thread(int *adress_data_list);

					/*! @brief �w�K�֐��̒��g(�P�ƃX���b�h)
					@param[in] adress_data_list �A�h���X�E�f�[�^���X�g
					@param[in] target �ڕW�l
					@return �Ȃ�
					*/
					void TrainAstarisk(int *adress_data_list, double target);

					/*! @brief �w�K�֐��̒��g(OpenMP�ɂ��}���`�R�A)
					@param[in] adress_data_list �A�h���X�E�f�[�^���X�g
					@param[in] target �ڕW�l
					@return �Ȃ�
					*/
					void TrainAstarisk_MultiThread(int *adress_data_list, double target);
				public:

					//! �R���X�g���N�^
					SDNN_NN_PP();
					//! �f�R���X�g���N�^
					~SDNN_NN_PP() {}

					/*! @brief NN�̏�����
					@param[in,out] sdnn_parameter SDNN�̃p�����[�^
					@param[in] input_pattern_length ���̓p�^�[���� �� ���͂̕s������p�^�[���̒���
					@return �Ȃ��D�G���[��throw
					*/
					void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);
				};
			}
		}
	}
}