#pragma once
#include "BIPL_LIB_SDNN_SD_VIRTUAL.h"
#include <vector>

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
			@version ��1.0�i12/6�F�u���b�N�{�b�N�X�e�X�g�I���j
			*/
			namespace sd
			{
				class SDNN_SD_MULTI : public SDNN_SD_VIRTUAL
				{
				private:
					//! �R�s�[�R���X�g���N�^�͋֎~
					SDNN_SD_MULTI(const SDNN_SD_MULTI&) = delete;
					//! ������Z���֎~
					void operator=(const SDNN_SD_MULTI&) = delete;

					//! ��s�����p�^�[���Ɠ��͂̑Ή�
					unsigned int *s_list_;
					//! �s�����p�^�[���Ɠ��͂̑Ή�
					std::vector<unsigned int> *c_list_;

					//! ��s�����p�^�[���̃��X�g
					SDNN_PC *pattern_s_;
					//! �s�����p�^�[���̃��X�g�C�����͒P�w�s����
					SDNN_PC **pattern_c_;

					//! 1�p�^�[���̑f�q��
					int n_;
					//! 1�p�^�[���̕�����
					int q_;
					//! 1�p�^�[����\������unsigned int�i32bit�j��
					int n32_;

					//! �J�X�^���C�t�@�C������s�����̑g�ݍ��킹�����[�h����ꍇ
					void MakePatternAndListWithFile(std::ifstream &sd_file);

					/*! @brief double���͂�s�����ς݃p�^�[���̃A�h���X�f�[�^���X�g�ɕϊ�
					@param[out] out_adress_data_list �o�̓A�h���X�f�[�^���X�g
					@param[in] input ����
					@note desensitized_number_ < 44�i���������j�ʂ��ƁC�������̂ق��������i�l��PC�ł́j
					@return �Ȃ��D
					*/
					void SelectiveDesensitizationAstarisk(int* out_adress_data_list, const double* input);

				public:
					//! �R���X�g���N�^
					SDNN_SD_MULTI(void);
					//! �f�R���X�g���N�^
					~SDNN_SD_MULTI(void);

					/*! @brief SD�̏�����
					�ݒ肷�ׂ��p�����[�^
					-SDNN\input_number:���͐�
					-SDNN\SD\method(mutual|file):�s�����̑g�ݍ��킹�̌�����@
					-SDNN\SD\file(method == file�̏ꍇ):�Q�Ƃ��ׂ��t�@�C����
					@param[in,out] sdnn_parameter �p�����[�^
					@return �Ȃ��D
					*/
					void InitSD(parameters::PARAMETERS &sdnn_parameter);
				};
			}
		}
	}
}