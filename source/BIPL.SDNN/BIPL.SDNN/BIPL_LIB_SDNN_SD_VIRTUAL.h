#pragma once
#include <string>
#include <memory>
#include <BIPL_LIB_PARAMETERS_JUDGEMENT.h>

namespace bipl
{
	namespace parameters
	{
		//! パラメータの前方宣言
		class PARAMETERS;
	}
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
			class SDNN_SD_VIRTUAL
			{
			private:
				//! コピーコンストラクタは禁止
				SDNN_SD_VIRTUAL(const SDNN_SD_VIRTUAL&) = delete;
				//! 代入演算も禁止
				void operator=(const SDNN_SD_VIRTUAL&) = delete;

			protected:
				//! 入力数
				unsigned int input_number_;
				//! inputを量子化したもの
				std::vector<int> quantized_input_;
				//! inputの種類
				enum INPUT_TYPE_
				{
					NUMERICAL = 0,
					SYMBOL = 1,
				};
				std::vector<INPUT_TYPE_> input_type_;
				std::vector<int> q_;

				//! 不感化の結果得られる不感化パターンの素子の数
				int desensitized_pattern_neuron_number_;

				//! 不感化数
				unsigned int desensitize_number_;

				//! 不感化部分の具体的な中身．関数ポインタリストを使って欲しくないためにわざとvirtualにしていない．
				void SelectiveDesensitizationAstarisk(int* out_adress_data_list, const double *input);
				//! 不感化部分の関数ポインタ
				void (SDNN_SD_VIRTUAL::*SelectiveDesensitizationPointer)(int* out_adress_data_list, const double *input);

			public:
				//! コンストラクタ
				SDNN_SD_VIRTUAL(void);
				//! デコンストラクタ
				virtual ~SDNN_SD_VIRTUAL(void);

				/*! @brief SDの初期化
				設定すべきパラメータ
				SDNN\\input_number:入力数
				SDNN\\SD\\method(mutual|file):不感化の組み合わせの決定方法
				SDNN\\SD\\file(method == fileの場合):参照すべきファイル名
				@param[in,out] sdnn_parameter パラメータ
				@return なし．
				*/
				virtual void InitSD(parameters::PARAMETERS &sdnn_parameter);

				/*! @brief 選択的不感化の実行
				@param[out] out_adress_data_list 出力アドレス・データリスト
				@param[in] input SDNNへの入力
				@return なし．
				*/
				void SelectiveDesensitization(int* out_adress_data_list, const double* input) { (this->*SelectiveDesensitizationPointer)(out_adress_data_list, input); }

				/*! @brief 不感化済みパターンの素子数
				@return 不感化済みパターンの素子数=desensitized_pattern_neuron_number．
				*/
				int GetSDPatternNeuronNumber(void);

				/*! @brief 不感化リストの返却
				@return 不感化リスト [(S1,C1),(S2,C2),,,,]
				*/
				virtual std::string GetDesensitizationList(void);
			};

			//! 不感化部分の関数ポインタに継承されたクラスからアクセスできるように型定義．
			typedef void(SDNN_SD_VIRTUAL::*SelectiveDesensitization_callback)(int* out_adress_data_list, const double *input);
		}
	}
}