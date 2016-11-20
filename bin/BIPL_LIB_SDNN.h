#pragma once
#include <string>
#include <vector>
#include <memory>

namespace bipl
{
	namespace sdnn
	{
		/*! @brief SDNN�̃p�����[�^�t�@�C���̃`�F�b�N�C��{�I��MATLAB�p�D
		@param[in] parameter_filename SDNN�̃p�����[�^�t�@�C����
		@return ���Ȃ���΁hOK�h�C��肪����΃G���[������ԋp�D
		@todo random_seed�̃`�F�b�N�������Ή��D
		*/
		std::string CheckParameterFile(std::string parameter_filename);
			
		/*! @brief SDNN�̃N���X
		*/
		class SDNN
		{
		private:
			class SDNN_PIMPL;
			//! SDNN��PIMPL
			std::unique_ptr<SDNN_PIMPL> pimpl_;

			//! �R�s�[�R���X�g���N�^�͋֎~
			SDNN(const SDNN&) = delete;
			//! ������Z���֎~
			void operator=(const SDNN&) = delete;

		public:
			//! �R���X�g���N�^
			SDNN();
			//! �f�R���X�g���N�^
			~SDNN();

			/*! @brief SDNN�̏�����
			@param[in] parameter_filename SDNN�̃p�����[�^�t�@�C����
			@return �Ȃ��D�G���[��throw
			*/
			void InitSDNN(const std::string &parameter_filename);

			/*! @brief 1�T���v���̊w�K
			@param[in] input ����
			@param[in] target �ڕW�l
			@return�@�Ȃ�
			@memo �@�B�w�K�ł̗��p��z��
			*/
			void TrainOneSample(const std::vector<double> &input, const double target);

			/*! @brief �w�K���@���X�g�ɏ]���Ċw�K�D
			@param[in] input ���̓��X�g
			@param[in] target �ڕW�l���X�g
			@return�@�Ȃ��C�d������throw���Ȃ��D
			*/
			void Train(const std::vector<std::vector<double>>input, const std::vector<double> target);

			/*! @brief �o�͂̌���
			@param[in] input ����
			@return ���肵���o��
			*/
			double Estimate(const std::vector<double> &input);

			/*! @brief �����d�ʎ擾
			@param[in] out_potential �����d��
			@param[in] input ����
			@return ���肵���o��
			*/
			void GetPotential(std::vector<int> &out_potential, const std::vector<double> input);

			/*! @brief �w�K���ʂ̕ۑ�
			@param[in] filename �ۑ���t�@�C������
			@return�@�Ȃ��C
			*/
			void Save(const std::string &filename);

			/*! @brief �w�K���ʂ̃��[�h
			@param[in] filename �w�K���ʃt�@�C������
			@return�@�Ȃ��C
			*/
			void Load(const std::string &filename);

			/*! @brief ���Z�b�g�DInit�����蒼��
			@return�@�Ȃ��C
			*/
			void Reset(void);

			/*! @brief sdnn�̃p�����[�^�̌Ăяo��
			@param[out] out_parameter �p�����[�^�̓��e
			@param[in] parameter_name �p�����[�^�̖��O
			@return�@�Ȃ��C
			*/
			template <typename content_type>
			void GetParameter(content_type &out_parameter, const std::string &parameter_name);

			/*! @brief matlab���C�u�����p�֐�
			*/
			void Train4Matlab(double *input, double *target, int target_num);
			double Estimate4Matlab(double *input);
		};
	}
}