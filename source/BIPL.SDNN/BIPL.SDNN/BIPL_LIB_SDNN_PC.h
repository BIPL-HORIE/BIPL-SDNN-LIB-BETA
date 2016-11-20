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
				/*! @brief 1と0が半分ずつのパターンのランダム作成
				@param[out] out_pattern 出力先配列，（n_以上分確保されていることを前提）
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void MakeRandomPattern(std::vector<bool> &out_pattern, std::mt19937 &mt);

				/*! @brief materialから反転数rのパターンをランダム作成
				@param[out] out_pattern 出力先配列
				@param[in] material 原材料パターン
				@param[in] r 反転数
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void MakeRandomPattern(std::vector<bool> &out_pattern, const std::vector<bool> &material, const int r, std::mt19937 &mt);

				/*! @brief bool表現のパターンをunsigned intに変換して保存．
				@param[in] binary	保存先 
				@param[in] pattern bool表現のパターン
				@return なし．
				*/
				void SavePattern(unsigned int* binary, const std::vector<bool> &pattern);
			}
			class SDNN_PC
			{
			private:
				//! パターン素子数
				int n_;
				//! パターン素子を32bitintで表現したときに必要になるintの数
				int n32_;
				//! 32bit表現されたバイナリパターン
				std::vector<unsigned int*> binary_pattern_;

				//パターンの作成手法
				//関数ポインタの利用を考えたために入力が少し不自然．変更したほうがいいかもしれない．

				/*! @brief ランダム反転によるパターン作成
				@param[in] q 分割数
				@param[in] r 反転数
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void RandomInverse(const int q, const int r, std::mt19937 &mt);

				/*! @brief 補間法によるパターン作成
				@param[in] r 代表パターン数
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void Interpolation(const int q, const int r, std::mt19937 &mt);

				/*! @brief シンボルツリーによるパターン作成
				@param[in] tree_filename　ツリーのファイル名
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void SymbolTree(const std::string &tree_filename, std::mt19937 &mt);

				public:		
				//! コンストラクタ
				SDNN_PC(void);
				//! デコンストラクタ
				~SDNN_PC(void);

				/*! @brief 初期化
				@param[in] n 素子数
				@param[in] pattern_type (入力パターンのタイプ，数値-シンボル，分割数，作成方法等)
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void InitPC(const int n, const std::string &pattern_type, std::mt19937 &mt);

				/*! @brief inputに対応するバイナリを返す．
				@param[in] input 何番目のバイナリを返すか
				@warning ここのinputは範囲チェックをしない（速度重視）ので，上位で問題ないことを確認すべき．
				@return なし．
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