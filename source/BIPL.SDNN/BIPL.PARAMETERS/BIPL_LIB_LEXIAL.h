/*! @file BIPL_LIB_LEXIAL.h
@brief 文字列操作，ここは公開されない，，，はず．
@author Kazumasa HORIE
@date 2015/12/1~
*/

#pragma once
#include <string>
#include <vector>
namespace bipl
{
	/*!
	@brief 文字列操作関数群
	@author Kazumasa HORIE
	@date 2015/12/1~
	@version α1.0
	*/
	namespace lexial
	{
		/*! @brief 文字列を特定文字で分割してvectorに入れる
		@param[out] out_vector 出力
		@param[in] material 分割したい文字列
		@param[in] divide_charactor 分割文字
		@return なし．
		*/
		void Split(std::vector<std::string> &out_vector, const std::string &material, const char divide_charactor);

		/*! @brief 特定文字の完全削除
		@param[in,out] material 対象文字列
		@param[in] delete_charactor 消したい文字
		@return なし．
		*/
		void DeleteCharactor(std::string &material, const std::string delete_charactor);

		/*! @brief 特定文字のカウント
		@param[in] material 対象文字列
		@param[in] count_charactor 数えたい文字
		@return 特定文字の数．
		*/
		int CountCharactor(const std::string material, const std::string count_charactor);
	}
}

