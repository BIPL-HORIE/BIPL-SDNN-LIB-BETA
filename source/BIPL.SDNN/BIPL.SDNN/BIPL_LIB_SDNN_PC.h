#pragma once

#include <string>
#include <memory>
#include <random>
namespace bipl
{
	namespace sdnn
	{
		namespace base
		{
			namespace pc
			{
				/*! @brief 1��0���������̃p�^�[���̃����_���쐬
				@param[out] out_pattern �o�͐�z��C�in_�ȏ㕪�m�ۂ���Ă��邱�Ƃ�O��j
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void MakeRandomPattern(std::vector<bool> &out_pattern, std::mt19937 &mt);

				/*! @brief material���甽�]��r�̃p�^�[���������_���쐬
				@param[out] out_pattern �o�͐�z��
				@param[in] material ���ޗ��p�^�[��
				@param[in] r ���]��
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void MakeRandomPattern(std::vector<bool> &out_pattern, const std::vector<bool> &material, const int r, std::mt19937 &mt);

				/*! @brief bool�\���̃p�^�[����unsigned int�ɕϊ����ĕۑ��D
				@param[in] binary	�ۑ��� 
				@param[in] pattern bool�\���̃p�^�[��
				@return �Ȃ��D
				*/
				void SavePattern(unsigned int* binary, const std::vector<bool> &pattern);

				/*! @brief bool�\���̃p�^�[��2�̑��֒l���v�Z�D
				@param[in] pattern1	1��
				@param[in] pattern2	2��
				@return pattern1��pattern2�̑��֒l�D
				*/
				double CalcCorrelation(const std::vector<bool> & pattern1, const std::vector<bool> & pattern2);

				/*! @brief ���֌W���t�@�C�����J���C���s��\\�C"�C"��$�ɒu�������C������Ƃ��ďo��
				@param[in] filename ���֌W���t�@�C����
				@return �ϊ��㕶����
				*/
				std::string CorrelationMatrixFile2Strings(const std::string &filename);
			}
			class SDNN_PC
			{
			private:
				//! �p�^�[���f�q��
				int n_;
				//! �p�^�[���f�q��32bitint�ŕ\�������Ƃ��ɕK�v�ɂȂ�int�̐�
				int n32_;
				//! 32bit�\�����ꂽ�o�C�i���p�^�[��
				std::vector<unsigned int*> binary_pattern_;

				//�p�^�[���̍쐬��@
				//�֐��|�C���^�̗��p���l�������߂ɓ��͂������s���R�D�ύX�����ق���������������Ȃ��D

				/*! @brief �����_�����]�ɂ��p�^�[���쐬
				@param[in] q ������
				@param[in] r ���]��
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void RandomInverse(const int q, const int r, std::mt19937 &mt);

				/*! @brief ���֍s��ɂ��p�^�[���쐬
				@param[in] correlation_matrix ���֍s��
				@param[in] batch_n ���������������p�^�[���̃T�C�Y
				@param[in] iteration �ő唽���񐔁C����𒴂��Ă����x����������Ȃ��ꍇ�C�S�Ă�蒼���D
				�@�@�@�@�@�@�@�@�@�@ ��x�������C����ł��B���ł��Ȃ���ΐ��x���������C���s����D
				@param[in] precision ���߂鐸�x
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void CorrelationMatrix(const std::vector<std::vector<double>> &correlation_matrix, const int batch_n, const int max_iteration, const double precision, std::mt19937 &mt, const bool multi_core);

				/*! @brief ���֍s��ɂ��p�^�[���쐬
				@param[in] correlation_matrix_filename ���֍s��t�@�C����
				@param[in] batch_n ���������������p�^�[���̃T�C�Y
				@param[in] iteration �ő唽���񐔁C����𒴂��Ă����x����������Ȃ��ꍇ�C�S�Ă�蒼���D
				�@�@�@�@�@�@�@�@�@�@ ��x�������C����ł��B���ł��Ȃ���ΐ��x���������C���s����D
						   @param[in] precision ���߂鐸�x
						   @param[in] mt �����Z���k�c�C�X�^
						   @return �Ȃ��D
						   */
				void CorrelationMatrix(const std::string &correlation_matrix_filename, const int batch_n, const int max_iteration, const double precision, std::mt19937 &mt, const bool multi_core);

			public:		
				//! �R���X�g���N�^
				SDNN_PC(void);
				//! �f�R���X�g���N�^
				~SDNN_PC(void);

				/*! @brief ������
				@param[in] n �f�q��
				@param[in] pattern_type (���̓p�^�[���̃^�C�v�C���l-�V���{���C�������C�쐬���@��)
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void InitPC(const int n, const std::string &pattern_type, std::mt19937 &mt, const bool multi_core = false);

				/*! @brief input�ɑΉ�����o�C�i����Ԃ��D
				@param[in] input ���Ԗڂ̃o�C�i����Ԃ���
				@warning ������input�͔͈̓`�F�b�N�����Ȃ��i���x�d���j�̂ŁC��ʂŖ��Ȃ����Ƃ��m�F���ׂ��D
				@return �Ȃ��D
				*/
				unsigned int* GetPatternBinary(const int input) { return binary_pattern_[input]; }

				//! return n_
				int GetN(void) { return n_; }
				//! return pattern_number_
				int GetPatternNumber(void) { return static_cast<int>(binary_pattern_.size()); }
			};
		}
	}
}