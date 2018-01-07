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

				/*! @brief bool表現のパターン2つの相関値を計算．
				@param[in] pattern1	1つめ
				@param[in] pattern2	2つめ
				@return pattern1とpattern2の相関値．
				*/
				double CalcCorrelation(const std::vector<bool> & pattern1, const std::vector<bool> & pattern2);

				/*! @brief 相関係数ファイルを開き，改行を\\，"，"を$に置き換え，文字列として出力
				@param[in] filename 相関係数ファイル名
				@return 変換後文字列
				*/
				std::string CorrelationMatrixFile2Strings(const std::string &filename);
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

				/*! @brief 相関行列によるパターン作成
				@param[in] correlation_matrix 相関行列
				@param[in] batch_n 分割した小さいパターンのサイズ
				@param[in] iteration 最大反復回数，これを超えても精度が満たされない場合，全てやり直す．
				　　　　　　　　　　 難度か試し，それでも達成できなければ精度を悪くし，実行する．
				@param[in] precision 求める精度
				@param[in] mt メルセンヌツイスタ
				@return なし．
				*/
				void CorrelationMatrix(const std::vector<std::vector<double>> &correlation_matrix, const int batch_n, const int max_iteration, const double precision, std::mt19937 &mt, const bool multi_core);

				/*! @brief 相関行列によるパターン作成
				@param[in] correlation_matrix_filename 相関行列ファイル名
				@param[in] batch_n 分割した小さいパターンのサイズ
				@param[in] iteration 最大反復回数，これを超えても精度が満たされない場合，全てやり直す．
				　　　　　　　　　　 難度か試し，それでも達成できなければ精度を悪くし，実行する．
						   @param[in] precision 求める精度
						   @param[in] mt メルセンヌツイスタ
						   @return なし．
						   */
				void CorrelationMatrix(const std::string &correlation_matrix_filename, const int batch_n, const int max_iteration, const double precision, std::mt19937 &mt, const bool multi_core);

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
				void InitPC(const int n, const std::string &pattern_type, std::mt19937 &mt, const bool multi_core = false);

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