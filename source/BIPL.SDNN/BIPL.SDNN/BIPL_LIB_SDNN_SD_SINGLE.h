#pragma once
#include <memory>

#include "BIPL_LIB_SDNN_SD_VIRTUAL.h"


namespace bipl
{
	namespace sdnn
	{
		namespace base
		{
			//! パターンの前方宣言
			class SDNN_PC;

			/*!
			@brief SDNNのSD部分，PCの管理クラスを兼ねる．
			@note 俺は何回書き直しているのだろうか．．．
			@author Kazumasa HORIE
			@date 2015/12/15~
			@version β1.0（12/26：ブラックボックステスト終了）
			*/
			namespace sd
			{
				class SDNN_SD_SINGLE : public SDNN_SD_VIRTUAL
				{
				private:
					//! コピーコンストラクタは禁止
					SDNN_SD_SINGLE(const SDNN_SD_SINGLE&) = delete;
					//! 代入演算も禁止
					void operator=(const SDNN_SD_SINGLE&) = delete;

					//! 被不感化パターンと入力の対応
					std::vector<unsigned int> s_list_;
					//! 不感化パターンと入力の対応
					std::vector<unsigned int> c_list_;

					//! 被不感化パターンのリスト
					std::vector<SDNN_PC> pattern_s_;
					//! 不感化パターンのリスト，ここは単層不感化
					std::vector<SDNN_PC> pattern_c_;

					//! 1パターンの素子数
					int n_;
					//! 1パターンを表現するunsigned int（32bit）数
					int n32_;

					//! 出力されるアドレスデータリスト長/2 ＝不感化後パターンの0でない値を持つ素子数
					int adress_data_list_length_per2;

					//! 相互不感化のみの場合
					void MakePatternAndListWithMutual(void);
					//! カスタム，ファイルから不感化の組み合わせをロードする場合
					void MakePatternAndListWithFile(std::ifstream &sd_file);
					//! Saveファイル用，1行指定ライン読み込み
					void MakePatternAndListWithLine(std::vector<std::string> &line);

					/*! @brief double入力を不感化済みパターンのアドレスデータリストに変換
					@param[out] out_adress_data_list 出力アドレスデータリスト
					@param[in] input 入力
					@note desensitized_number_ < 44（だいたい）位だと，こっちのほうが速い（僕のPCでは）
					@return なし．
					*/
					void SelectiveDesensitizationAstarisk(int* out_adress_data_list, const double* input);

					/*! @brief double入力を不感化済みパターンのアドレスデータリストに変換
					@param[out] out_adress_data_list 出力アドレスデータリスト
					@param[in] input 入力
					@note desensitized_number_ > 44（だいたい）位だと，こっちのほうが速い
					@return なし．
					*/
					void SelectiveDesensitizationAstarisk_MultiThread(int* out_adress_data_list, const double* input);

				public:
					//! コンストラクタ
					SDNN_SD_SINGLE(void);
					//! デコンストラクタ
					~SDNN_SD_SINGLE(void);

					/*! @brief SDの初期化
					設定すべきパラメータ
					-SDNN\input_number:入力数
					-SDNN\SD\method(mutual|file):不感化の組み合わせの決定方法
					-SDNN\SD\file(method == fileの場合):参照すべきファイル名
					@param[in,out] sdnn_parameter パラメータ
					@return なし．
					*/
					void InitSD(parameters::PARAMETERS &sdnn_parameter);

					std::string GetDesensitizationList(void);
				};
			}
		}
	}
}