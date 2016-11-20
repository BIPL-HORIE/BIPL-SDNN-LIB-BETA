#pragma once
#include "BIPL_LIB_SDNN_NN_VIRTUAL.h"
#include <vector>
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
			namespace nn
			{
				class SDNN_NN_PP :public SDNN_NN_VIRTUAL
				{
				private:
					//! ニューロンの内部電位が低いほうを選択する関数クラス
					class CHOOSE_LESS_POTENTIAL;

					//! output = a_*(1_neuron_number) + b_;
					double a_;
					double b_;
				
					/*! @brief 出力決定関数の中身(単独スレッド)
					@param[in] adress_data_list アドレス・データリスト
					@return 決定した出力
					*/
					double DetermineOutputAstarisk(int *adress_data_list);

					/*! @brief 出力決定関数の中身(OpenMPによるマルチコア)
					@param[in] adress_data_list アドレス・データリスト
					@return 決定した出力
					*/
					double DetermineOutputAstarisk_MultiThread(int *adress_data_list);

					/*! @brief 出力決定関数の中身(C++11threadによるマルチコア)
					@param[in] adress_data_list アドレス・データリスト
					@return 決定した出力
					@warning うまく動いていない．要threadについての学習
					*/
//					double DetermineOutputAstarisk_MultiThread_C11thread(int *adress_data_list);

					/*! @brief 学習関数の中身(単独スレッド)
					@param[in] adress_data_list アドレス・データリスト
					@param[in] target 目標値
					@return なし
					*/
					void TrainAstarisk(int *adress_data_list, double target);

					/*! @brief 学習関数の中身(OpenMPによるマルチコア)
					@param[in] adress_data_list アドレス・データリスト
					@param[in] target 目標値
					@return なし
					*/
					void TrainAstarisk_MultiThread(int *adress_data_list, double target);
				public:

					//! コンストラクタ
					SDNN_NN_PP();
					//! デコンストラクタ
					~SDNN_NN_PP() {}

					/*! @brief NNの初期化
					@param[in,out] sdnn_parameter SDNNのパラメータ
					@param[in] input_pattern_length 入力パターン長 ＝ 入力の不感化後パターンの長さ
					@return なし．エラー時throw
					*/
					void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);
				};
			}
		}
	}
}