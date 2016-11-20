/*! @file BIPL_LIB_LEXIAL.h
@brief �����񑀍�C�����͌��J����Ȃ��C�C�C�͂��D
@author Kazumasa HORIE
@date 2015/12/1~
*/

#pragma once
#include <string>
#include <vector>
namespace bipl
{
	/*!
	@brief �����񑀍�֐��Q
	@author Kazumasa HORIE
	@date 2015/12/1~
	@version ��1.0
	*/
	namespace lexial
	{
		/*! @brief ���������蕶���ŕ�������vector�ɓ����
		@param[out] out_vector �o��
		@param[in] material ����������������
		@param[in] divide_charactor ��������
		@return �Ȃ��D
		*/
		void Split(std::vector<std::string> &out_vector, const std::string &material, const char divide_charactor);

		/*! @brief ���蕶���̊��S�폜
		@param[in,out] material �Ώە�����
		@param[in] delete_charactor ������������
		@return �Ȃ��D
		*/
		void DeleteCharactor(std::string &material, const std::string delete_charactor);

		/*! @brief ���蕶���̃J�E���g
		@param[in] material �Ώە�����
		@param[in] count_charactor ������������
		@return ���蕶���̐��D
		*/
		int CountCharactor(const std::string material, const std::string count_charactor);
	}
}

