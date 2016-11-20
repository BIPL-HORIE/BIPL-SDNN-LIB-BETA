#pragma once
#include <memory>

#include "BIPL_LIB_SDNN_SD_VIRTUAL.h"


namespace bipl
{
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
			namespace sd
			{
				class SDNN_SD_SINGLE : public SDNN_SD_VIRTUAL
				{
				private:
					//! �R�s�[�R���X�g���N�^�͋֎~
					SDNN_SD_SINGLE(const SDNN_SD_SINGLE&) = delete;
					//! ������Z���֎~
					void operator=(const SDNN_SD_SINGLE&) = delete;

					//! ��s�����p�^�[���Ɠ��͂̑Ή�
					std::vector<unsigned int> s_list_;
					//! �s�����p�^�[���Ɠ��͂̑Ή�
					std::vector<unsigned int> c_list_;

					//! ��s�����p�^�[���̃��X�g
					std::vector<SDNN_PC> pattern_s_;
					//! �s�����p�^�[���̃��X�g�C�����͒P�w�s����
					std::vector<SDNN_PC> pattern_c_;

					//! 1�p�^�[���̑f�q��
					int n_;
					//! 1�p�^�[����\������unsigned int�i32bit�j��
					int n32_;

					//! �o�͂����A�h���X�f�[�^���X�g��/2 ���s������p�^�[����0�łȂ��l�����f�q��
					int adress_data_list_length_per2;

					//! ���ݕs�����݂̂̏ꍇ
					void MakePatternAndListWithMutual(void);
					//! �J�X�^���C�t�@�C������s�����̑g�ݍ��킹�����[�h����ꍇ
					void MakePatternAndListWithFile(std::ifstream &sd_file);
					//! Save�t�@�C���p�C1�s�w�胉�C���ǂݍ���
					void MakePatternAndListWithLine(std::vector<std::string> &line);

					/*! @brief double���͂�s�����ς݃p�^�[���̃A�h���X�f�[�^���X�g�ɕϊ�
					@param[out] out_adress_data_list �o�̓A�h���X�f�[�^���X�g
					@param[in] input ����
					@note desensitized_number_ < 44�i���������j�ʂ��ƁC�������̂ق��������i�l��PC�ł́j
					@return �Ȃ��D
					*/
					void SelectiveDesensitizationAstarisk(int* out_adress_data_list, const double* input);

					/*! @brief double���͂�s�����ς݃p�^�[���̃A�h���X�f�[�^���X�g�ɕϊ�
					@param[out] out_adress_data_list �o�̓A�h���X�f�[�^���X�g
					@param[in] input ����
					@note desensitized_number_ > 44�i���������j�ʂ��ƁC�������̂ق�������
					@return �Ȃ��D
					*/
					void SelectiveDesensitizationAstarisk_MultiThread(int* out_adress_data_list, const double* input);

				public:
					//! �R���X�g���N�^
					SDNN_SD_SINGLE(void);
					//! �f�R���X�g���N�^
					~SDNN_SD_SINGLE(void);

					/*! @brief SD�̏�����
					�ݒ肷�ׂ��p�����[�^
					-SDNN\input_number:���͐�
					-SDNN\SD\method(mutual|file):�s�����̑g�ݍ��킹�̌�����@
					-SDNN\SD\file(method == file�̏ꍇ):�Q�Ƃ��ׂ��t�@�C����
					@param[in,out] sdnn_parameter �p�����[�^
					@return �Ȃ��D
					*/
					void InitSD(parameters::PARAMETERS &sdnn_parameter);

					std::string GetDesensitizationList(void);
				};
			}
		}
	}
}