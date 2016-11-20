#pragma once
#include <string>
#include <vector>
#include <memory>

namespace bipl
{
	namespace sdnn
	{
		/*! @brief SDNNのパラメータファイルのチェック，基本的にMATLAB用．
		@param[in] parameter_filename SDNNのパラメータファイル名
		@return 問題なければ”OK”，問題があればエラー文字列返却．
		@todo random_seedのチェックだけ未対応．
		*/
		std::string CheckParameterFile(std::string parameter_filename);
			
		/*! @brief SDNNのクラス
		*/
		class SDNN
		{
		private:
			class SDNN_PIMPL;
			//! SDNNのPIMPL
			std::unique_ptr<SDNN_PIMPL> pimpl_;

			//! コピーコンストラクタは禁止
			SDNN(const SDNN&) = delete;
			//! 代入演算も禁止
			void operator=(const SDNN&) = delete;

		public:
			//! コンストラクタ
			SDNN();
			//! デコンストラクタ
			~SDNN();

			/*! @brief SDNNの初期化
			@param[in] parameter_filename SDNNのパラメータファイル名
			@return なし．エラー時throw
			*/
			void InitSDNN(const std::string &parameter_filename);

			/*! @brief 1サンプルの学習
			@param[in] input 入力
			@param[in] target 目標値
			@return　なし
			@memo 機械学習での利用を想定
			*/
			void TrainOneSample(const std::vector<double> &input, const double target);

			/*! @brief 学習方法リストに従って学習．
			@param[in] input 入力リスト
			@param[in] target 目標値リスト
			@return　なし，重いからthrowしない．
			*/
			void Train(const std::vector<std::vector<double>>input, const std::vector<double> target);

			/*! @brief 出力の決定
			@param[in] input 入力
			@return 決定した出力
			*/
			double Estimate(const std::vector<double> &input);

			/*! @brief 内部電位取得
			@param[in] out_potential 内部電位
			@param[in] input 入力
			@return 決定した出力
			*/
			void GetPotential(std::vector<int> &out_potential, const std::vector<double> input);

			/*! @brief 学習結果の保存
			@param[in] filename 保存先ファイル名称
			@return　なし，
			*/
			void Save(const std::string &filename);

			/*! @brief 学習結果のロード
			@param[in] filename 学習結果ファイル名称
			@return　なし，
			*/
			void Load(const std::string &filename);

			/*! @brief リセット．Initからやり直し
			@return　なし，
			*/
			void Reset(void);

			/*! @brief sdnnのパラメータの呼び出し
			@param[out] out_parameter パラメータの内容
			@param[in] parameter_name パラメータの名前
			@return　なし，
			*/
			template <typename content_type>
			void GetParameter(content_type &out_parameter, const std::string &parameter_name);

			/*! @brief matlabライブラリ用関数
			*/
			void Train4Matlab(double *input, double *target, int target_num);
			double Estimate4Matlab(double *input);
		};
	}
}