#pragma once
#include <string>
#include <vector>
#include <random>
#include <memory>
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
			//! ニューロンの内部電位＋場所
			class NEURON_OUTPUT
			{
			public:
				unsigned int index;
				int potential;
			};

			//! @todo POTENTIALクラスの作成．inner_potentialの決定を関数化．←10%以上計算時間が増加．計算時間とファイルサイズのどちらを優先するか考え中．
			class SDNN_NN_VIRTUAL
			{
			private:
				//! コピーコンストラクタは禁止
				SDNN_NN_VIRTUAL(const SDNN_NN_VIRTUAL&) = delete;
				//! 代入演算も禁止
				void operator=(const SDNN_NN_VIRTUAL&) = delete;

				//! 出力決定関数の中身
				double DetermineOutputAstarisk(int *adress_data_list);

				//! 学習関数の中身
				void TrainAstarisk(int *adress_data_list, double target);

			protected:
				//! 重みの配列
				std::unique_ptr<int[]> w_;

				//! 1出力ニューロンあたりの重み数（h含む）
				int w_number_per_neuron_;
				//! 出力ニューロン数
				int output_neuron_number_;

				//! 出力決定関数の関数ポインタ
				double (SDNN_NN_VIRTUAL::*DetermineOutputPointer)(int *adress_data_list);
				//! 学習関数の関数ポインタ
				void (SDNN_NN_VIRTUAL::*TrainPointer)(int* adress_data_list, double target);

				//! 重み初期値の最小値
				int initial_value_min;
				//! 重み初期値の最大値
				int initial_value_max;

			public:
				//! コンストラクタ
				SDNN_NN_VIRTUAL();
				//! デコンストラクタ
				virtual ~SDNN_NN_VIRTUAL();

				/*! @brief NNの初期化
				@param[in,out] sdnn_parameter SDNNのパラメータ
				@param[in] input_pattern_length 入力パターン長 ＝ 入力の不感化後パターンの長さ
				@return なし．
				*/
				virtual void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);

				/*! @brief 出力の決定
				@param[in] adress_data_list アドレスデータリスト
				@note OPENMP利用
				@return 決定した出力
				*/
				double DetermineOutput(int *adress_data_list) { return (this->*DetermineOutputPointer)(adress_data_list); }

				/*! @brief １入出力サンプルの学習
				@param[in] adress_data_list アドレスデータリスト
				@param[in] target 目標値
				@note OPENMP利用
				@return　なし，重いからthrowしない．
				*/
				void Train(int* adress_data_list, double target) { return (this->*TrainPointer)(adress_data_list, target); }

				/*! @brief 重みのリセット
				@param[in] mt メルセンヌツイスタ
				@return　なし，重いからthrowしない．
				*/
				void Reset(std::mt19937 &mt);

				/*! @brief 重みのロード
				@return　なし，thorowあり．
				*/
				void LoadW(std::vector<int> w);

				/*! @brief w_の直接参照．あまり作りたくないが仕方ない．
				@return　なし，
				*/
				void GetW(std::vector<int> &w);

				//! w_の長さを返答
				int GetWNumber(void);

				void GetPotential(std::vector<int> &out_potential, int* adress_data_list);
			};

			//! 学習関数の型定義
			typedef void(SDNN_NN_VIRTUAL::*Train_callback)(int *adress_data_list, double target);
			//! 出力決定関数の型定義
			typedef double(SDNN_NN_VIRTUAL::*DetermineOutput_callback)(int *adress_data_list);
		}
	}
}