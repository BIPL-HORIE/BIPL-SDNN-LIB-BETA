#pragma once
#include <string>
#include <vector>
#include <random>
#include <memory>
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
			//! �j���[�����̓����d�ʁ{�ꏊ
			class NEURON_OUTPUT
			{
			public:
				unsigned int index;
				int potential;
			};

			//! @todo POTENTIAL�N���X�̍쐬�Dinner_potential�̌�����֐����D��10%�ȏ�v�Z���Ԃ������D�v�Z���Ԃƃt�@�C���T�C�Y�̂ǂ����D�悷�邩�l�����D
			class SDNN_NN_VIRTUAL
			{
			private:
				//! �R�s�[�R���X�g���N�^�͋֎~
				SDNN_NN_VIRTUAL(const SDNN_NN_VIRTUAL&) = delete;
				//! ������Z���֎~
				void operator=(const SDNN_NN_VIRTUAL&) = delete;

				//! �o�͌���֐��̒��g
				double DetermineOutputAstarisk(int *adress_data_list);

				//! �w�K�֐��̒��g
				void TrainAstarisk(int *adress_data_list, double target);

			protected:
				//! �d�݂̔z��
				std::unique_ptr<int[]> w_;

				//! 1�o�̓j���[����������̏d�ݐ��ih�܂ށj
				int w_number_per_neuron_;
				//! �o�̓j���[������
				int output_neuron_number_;

				//! �o�͌���֐��̊֐��|�C���^
				double (SDNN_NN_VIRTUAL::*DetermineOutputPointer)(int *adress_data_list);
				//! �w�K�֐��̊֐��|�C���^
				void (SDNN_NN_VIRTUAL::*TrainPointer)(int* adress_data_list, double target);

				//! �d�ݏ����l�̍ŏ��l
				int initial_value_min;
				//! �d�ݏ����l�̍ő�l
				int initial_value_max;

			public:
				//! �R���X�g���N�^
				SDNN_NN_VIRTUAL();
				//! �f�R���X�g���N�^
				virtual ~SDNN_NN_VIRTUAL();

				/*! @brief NN�̏�����
				@param[in,out] sdnn_parameter SDNN�̃p�����[�^
				@param[in] input_pattern_length ���̓p�^�[���� �� ���͂̕s������p�^�[���̒���
				@return �Ȃ��D
				*/
				virtual void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);

				/*! @brief �o�͂̌���
				@param[in] adress_data_list �A�h���X�f�[�^���X�g
				@note OPENMP���p
				@return ���肵���o��
				*/
				double DetermineOutput(int *adress_data_list) { return (this->*DetermineOutputPointer)(adress_data_list); }

				/*! @brief �P���o�̓T���v���̊w�K
				@param[in] adress_data_list �A�h���X�f�[�^���X�g
				@param[in] target �ڕW�l
				@note OPENMP���p
				@return�@�Ȃ��C�d������throw���Ȃ��D
				*/
				void Train(int* adress_data_list, double target) { return (this->*TrainPointer)(adress_data_list, target); }

				/*! @brief �d�݂̃��Z�b�g
				@param[in] mt �����Z���k�c�C�X�^
				@return�@�Ȃ��C�d������throw���Ȃ��D
				*/
				void Reset(std::mt19937 &mt);

				/*! @brief �d�݂̃��[�h
				@return�@�Ȃ��Cthorow����D
				*/
				void LoadW(std::vector<int> w);

				/*! @brief w_�̒��ڎQ�ƁD���܂��肽���Ȃ����d���Ȃ��D
				@return�@�Ȃ��C
				*/
				void GetW(std::vector<int> &w);

				//! w_�̒�����ԓ�
				int GetWNumber(void);

				void GetPotential(std::vector<int> &out_potential, int* adress_data_list);
			};

			//! �w�K�֐��̌^��`
			typedef void(SDNN_NN_VIRTUAL::*Train_callback)(int *adress_data_list, double target);
			//! �o�͌���֐��̌^��`
			typedef double(SDNN_NN_VIRTUAL::*DetermineOutput_callback)(int *adress_data_list);
		}
	}
}