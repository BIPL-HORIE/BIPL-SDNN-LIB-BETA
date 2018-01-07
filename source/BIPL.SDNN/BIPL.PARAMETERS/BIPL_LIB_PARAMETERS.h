/*! @file BIPL_LIB_PARAMETERS.h
@brief 独自企画パラメータのロードと出力に関するクラス

オープンソース化の是非によらず，表面上は最小構成にすることにした．Pointer2implementationを利用．
この部分だけでライブラリ化しても，このhファイル以外をインクルードする必要は無いはず

@note このファイルはオープンソースでない場合，公開されない．前方宣言も無い
@author Kazumasa HORIE
@date 2015/12/1~
*/
#pragma once

#define HARDWARE_ENTROPY "hardware_entropy"

#include <memory>
#include <string>
#include <vector>
namespace bipl
{
	namespace parameters
	{
		namespace judgement
		{
			//! 条件式を含んだクラスの前方宣言
			class JUDGEMENT;
		}
		/*!
		@brief 独自企画パラメータ
		@code
		＜利用例１＞
		bipl::parameters::PARAMETERS test_parameter;
		test_parameter.LoadFile("setting.txt");
		int parameter_input_number;
		test_parameter.ReadParameter(parameter_input_number,"SDNN\\input_number","2");
		std::cout << parameter_input_number << std::endl;
		@endcode
		@code
		＜利用例２＞
		bipl::parameters::PARAMETERS test_parameter;
		test_parameter.LoadFile("setting.txt");
		std::vector<double> output_range;
		test_parameter.ReadParameter(output_range,"SDNN\\NN\\output_range","[-0.2,1.2]");
		if(output_range.size() >= 2)
			std::cout << output_range[0] << "," << output_range[1] << std::endl;
		@endcode
		@note 最小構成にしたので，利用上問題にはならない．．．．はず！
		@author Kazumasa HORIE
		@date 2015/12/1~
		@version β1.0（12/6：ブラックボックステスト終了）
		*/
		class PARAMETERS
		{
		private:
			//! Pointer2Implementation　クラス定義
			class PARAMETERS_ROOT;
			//! Pointer2Implementation　ポインタ本体　兼　パラメータ木の根っこ
			std::unique_ptr<PARAMETERS_ROOT> pimpl_;

			//! コピーコンストラクタは禁止
			PARAMETERS(const PARAMETERS&) = delete;
			//! 代入演算も禁止
			void operator=(const PARAMETERS&) = delete;

		public:
			//! コンストラクタ，中身はPIMPLの実体化
			PARAMETERS(void);
			//! デストラクタ，中身空
			~PARAMETERS(void);

			/*! @brief パラメータファイルからパラメータを引っ張ってきて登録する
			@param[in] parameter_file_name パラメータが書かれたファイルの名前
			@return なし．
			*/
			void LoadFile(const std::string parameter_file_name);

			/*! @brief BPLの書かれたstringstreamからパラメータを引っ張ってきて登録する
			@param[in] parameter_file_name パラメータが書かれたstringstream
			@return なし．
			*/
			void LoadFile(std::stringstream &parameter_stream);

			/*! @brief パラメータの登録＋上書き
			パラメータが存在していた場合は上書き，なかった場合は新たに登録する．
			コードの仕様上，無い場合登録する形になったが，正直単純なパラメータ登録に使うことは非推奨．
			random_seed = hardware_entropyの時に，hardware_entropyを用いてシードを作成．
			作成したシード値でrandom_seedを上書きしたい場合等に用いて欲しい．
			@param[in] parameter_name パラメータ名
			@param[in] parameter_content パラメータの内容
			@return なし．
			*/
			void OverwriteParameter(const std::string parameter_name, const std::string &parameter_content);

			/*! @brief パラメータの登録＋上書き
			contentにベクター形式を許可したもの．
			ベクター形式を登録文法に直すの面倒だからこれだけは追加．
			普通のintとかはstd::stringするだけだからいいかなー．．とか思ってる．
			@param[in] parameter_name パラメータ名
			@param[in] parameter_content パラメータの内容(int double unsigned int std::string対応)
			@return なし．
			*/
			template<typename content_type>
			void OverwriteParameter(const std::string parameter_name, const std::vector<content_type> parameter_content);

			/*! @brief パラメータを読む,条件に合わなかった場合コンソールから再入力を受ける
			@param[out] out_parameter_content パラメータ出力（int, double, unsigned int, std::string可）
			@param[in] parameter_name パラメータ名
			@param[in] default_parameter_content デフォルト出力
			@param[in] judgement 判定式を含んだクラスのアドレス nullptrなら条件無しと判断する（ユーザー定義可能）
			@return なし．パラメータ出力にするか悩んだが，配列を返すほうのReadParameterと出力が変わってしまうためやめた．
			*/
			template<typename content_type>
			void ReadParameter(content_type &out_parameter_content, const std::string parameter_name, const std::string default_parameter_content, judgement::JUDGEMENT *judgement = nullptr);

			/*! @brief パラメータ配列を読む,条件に合わなかった場合コンソールから再入力を受ける
			@param[out] out_parameter_content パラメータ出力（vector(int), (double), (unsigned int), (std::string)可）
			@param[in] parameter_name パラメータ名
			@param[in] default_parameter_content デフォルト出力
			@param[in] judgement 判定式を含んだクラス nullptrなら条件無しと判断する（ユーザー定義可能）
			@return なし．vectorなので適当にやれ．
			*/
			template<typename content_type>
			void ReadParameter(std::vector<content_type> &out_parameter_content, const std::string parameter_name, const std::string default_parameter_content, judgement::JUDGEMENT *judgement = nullptr);


			/*! @brief パラメータのリストを得る
			@param[out] out_parameter_list 出力パラメータリスト
			@warning 自動でout_parameter_listが初期化される．追記は不可．
			@return なし．
			*/
			void ListParameter(std::string &out_parameter_list)const;

			//! パラメータのリストをostreamに突っ込む
			friend std::ostream& operator<<(std::ostream& os, const PARAMETERS& parameter)
			{
				std::string list;
				parameter.ListParameter(list);
				os << list;
				return os;
			}
		};
	}
}