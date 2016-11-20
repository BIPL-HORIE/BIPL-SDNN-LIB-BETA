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

				/*! @brief ��Ԗ@�ɂ��p�^�[���쐬
				@param[in] r ��\�p�^�[����
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void Interpolation(const int q, const int r, std::mt19937 &mt);

				/*! @brief �V���{���c���[�ɂ��p�^�[���쐬
				@param[in] tree_filename�@�c���[�̃t�@�C����
				@param[in] mt �����Z���k�c�C�X�^
				@return �Ȃ��D
				*/
				void SymbolTree(const std::string &tree_filename, std::mt19937 &mt);

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
				void InitPC(const int n, const std::string &pattern_type, std::mt19937 &mt);

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