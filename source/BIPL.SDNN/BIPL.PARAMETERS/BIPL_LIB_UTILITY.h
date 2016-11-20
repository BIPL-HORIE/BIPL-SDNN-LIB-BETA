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
		/*! @brief ファイルを開く，開けなかった場合コンソールから再入力を求める．
		@param[out] file_stream ファイルストリーム
		@param[in] file_name　ファイル名
		@return オープンしたファイルの名前
		*/
//		template<typename file_type>
//		std::string OpenFile(file_type &file_stream, const std::string &file_name);

		template<typename file_type>
		std::string OpenFile(file_type &file_stream, const std::string &file_name, const std::ios::openmode mode = (std::ios::openmode)0);

		/*! @brief メモリ確保．bool int double unsigned_int対応
		@param[out] out_memory 確保したメモリのアドレス
		@param[in] row_size 行数，memoryの第一要素
		@param[in] column_size 列数，memoryの第二要素  out_memory[row_size][column_size]
		@return　なし
		*/
		template<typename memory_type>
		void AllocateMemory(memory_type** &out_memory, int row_size, int column_size);

		/*! @brief メモリ確保．bool int double unsigned_int対応
		@param[out] out_memory 確保したメモリのアドレス
		@param[in] row_size 行数，memoryの第一要素
		@param[in] column_size 列数，memoryの第二要素  out_memory[row_size][column_size]
		@return　なし
		*/
		template<typename memory_type>
		void AllocateMemory(std::vector<memory_type*> &out_memory, int row_size, int column_size);

		/*! @brief プログラムの一時中断
		@return　なし
		*/
		void Wait(void);

		/*! @brief メルセンヌツイスタのイニット
		シード値がベクトルだった場合そのまま，random_deviceだった場合ハード乱数を10個準備しシードとして利用．
		使ったシード値でパラメータを上書きする．
		@param[out] out_mt 初期化したメルセンヌツイスタ出力
		@param[in,out] parameter メルセンヌツイスタのシード値が含まれたパラメータリスト
		@param[in] seed_name シード値のパラメータ名
		@return　なし
		*/
		void InitMt(std::mt19937 &out_mt, bipl::parameters::PARAMETERS &parameter, const std::string &seed_name, const std::string &default_content);

		namespace error
		{
			void BugFound(int place);
			void AllocationError(int place);
		}
	}
}
