#pragma once

#include<string>
#include<random>
#include"BIPL_LIB_PARAMETERS.h"
#include<vector>
#include<fstream>

namespace bipl
{
	namespace utility
	{
		/*! @brief �t�@�C�����J���C�J���Ȃ������ꍇ�R���\�[������ē��͂����߂�D
		@param[out] file_stream �t�@�C���X�g���[��
		@param[in] file_name�@�t�@�C����
		@return �I�[�v�������t�@�C���̖��O
		*/
//		template<typename file_type>
//		std::string OpenFile(file_type &file_stream, const std::string &file_name);

		template<typename file_type>
		std::string OpenFile(file_type &file_stream, const std::string &file_name, const std::ios::openmode mode = (std::ios::openmode)0);

		/*! @brief �������m�ہDbool int double unsigned_int�Ή�
		@param[out] out_memory �m�ۂ����������̃A�h���X
		@param[in] row_size �s���Cmemory�̑��v�f
		@param[in] column_size �񐔁Cmemory�̑��v�f  out_memory[row_size][column_size]
		@return�@�Ȃ�
		*/
		template<typename memory_type>
		void AllocateMemory(memory_type** &out_memory, int row_size, int column_size);

		/*! @brief �������m�ہDbool int double unsigned_int�Ή�
		@param[out] out_memory �m�ۂ����������̃A�h���X
		@param[in] row_size �s���Cmemory�̑��v�f
		@param[in] column_size �񐔁Cmemory�̑��v�f  out_memory[row_size][column_size]
		@return�@�Ȃ�
		*/
		template<typename memory_type>
		void AllocateMemory(std::vector<memory_type*> &out_memory, int row_size, int column_size);

		/*! @brief �v���O�����̈ꎞ���f
		@return�@�Ȃ�
		*/
		void Wait(void);

		/*! @brief �����Z���k�c�C�X�^�̃C�j�b�g
		�V�[�h�l���x�N�g���������ꍇ���̂܂܁Crandom_device�������ꍇ�n�[�h������10�������V�[�h�Ƃ��ė��p�D
		�g�����V�[�h�l�Ńp�����[�^���㏑������D
		@param[out] out_mt ���������������Z���k�c�C�X�^�o��
		@param[in,out] parameter �����Z���k�c�C�X�^�̃V�[�h�l���܂܂ꂽ�p�����[�^���X�g
		@param[in] seed_name �V�[�h�l�̃p�����[�^��
		@return�@�Ȃ�
		*/
		void InitMt(std::mt19937 &out_mt, bipl::parameters::PARAMETERS &parameter, const std::string &seed_name, const std::string &default_content);

		namespace error
		{
			void BugFound(int place);
			void AllocationError(int place);
		}
	}
}
