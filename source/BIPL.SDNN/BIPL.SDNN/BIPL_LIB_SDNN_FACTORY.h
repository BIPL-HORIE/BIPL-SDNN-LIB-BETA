#pragma once

#include<memory>
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
			//! SD�����̑O���錾
			class SDNN_SD_VIRTUAL;

			/*! @brief SD�����̃t�@�N�g�����\�b�h�@�p�����[�^����K����SD�����̉��C�ԋp
			����SINGLE_SD�̂�
			@param[out] out_sd_pointer SD�̃|�C���^�C�����Ŏ��̉����ĕԋp
			@param[in,out] sdnn_parameter SDNN�̃p�����[�^
			@return	(parameter >= min && parameter <= max)
			*/
			void FactorySD(std::unique_ptr<SDNN_SD_VIRTUAL> &out_sd_pointer, parameters::PARAMETERS &sdnn_parameter);

			//! NN�����̑O���錾
			class SDNN_NN_VIRTUAL;

			/*! @brief NN�����̃t�@�N�g�����\�b�h�@�p�����[�^����K����NN�����̉��C�ԋp
			����PP��SP�̂݁DSP�̓\�t�g�E�F�A�e�X�g�����ĂȂ��D
			@param[out] out_nn_pointer NN�̃|�C���^�C�����Ŏ��̉����ĕԋp
			@param[in,out] sdnn_parameter SDNN�̃p�����[�^
			@return	(parameter >= min && parameter <= max)
			*/
			void FactoryNN(std::unique_ptr<SDNN_NN_VIRTUAL> &out_nn_pointer, parameters::PARAMETERS &sdnn_parameter);
		}
	}
}