#pragma once
#include <vector>
#include <string>

namespace bipl
{
	namespace parameters
	{
		class PARAMETERS;

		/*!
		@brief パラメータが条件に一致するかどうかの判定式クラスの名前空間
		@author Kazumasa HORIE
		@date 2015/1/26~
		*/
		namespace judgement
		{
			/*!
			@brief 判定式クラスの純粋仮想関数
			@author Kazumasa HORIE
			@date 2015/1/26~
			*/
			class JUDGEMENT
			{
			private:
				//! 判定を満たすパラメータのリスト，説明
				std::string list_;
			public:
				//! コンストラクタ
				JUDGEMENT(const std::string &list) :list_(list) {}
				JUDGEMENT(const std::vector<std::string> &list) {
					for (unsigned int i = 0; i < list.size(); i++)
						list_ += list[i] + "\n";
				}
				//! デコンストラクタ
				virtual ~JUDGEMENT() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return エラー．この関数は呼び出されないはず．
				*/
				virtual bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
				void coutList(void)const;
				std::string GetList(void)const { return list_; };
			};

			/*!
			@brief パラメータは特定範囲内に入るか否か
			parameter >= min && parameter <= max
			両側＝入るので注意
			@author Kazumasa HORIE
			*/
			template<typename content_type>
			class JUDGEMENT_IS_IN_RANGE :public JUDGEMENT
			{
			private:
				//! 最小値
				content_type min_;
				//! 最大値
				content_type max_;
			public:
				//! コンストラクタ
				JUDGEMENT_IS_IN_RANGE(content_type min, content_type max);
				//! デコンストラクタ
				~JUDGEMENT_IS_IN_RANGE() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	(parameter >= min && parameter <= max)
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief パラメータは規定値以上か否か
			parameter >= min
			@author Kazumasa HORIE
			*/
			template<typename content_type>
			class JUDGEMENT_IS_BIGGER_THAN :public JUDGEMENT
			{
			private:
				//! 最小値
				content_type min_;
			public:
				//! コンストラクタ
				JUDGEMENT_IS_BIGGER_THAN(content_type min);
				//! デコンストラクタ
				~JUDGEMENT_IS_BIGGER_THAN() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	(parameter >= min)
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief パラメータが範囲を意味するベクトルか否か
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_IS_RANGE :public JUDGEMENT
			{
			public:
				//! コンストラクタ
				JUDGEMENT_IS_RANGE();
				//! デコンストラクタ
				~JUDGEMENT_IS_RANGE() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	（2要素ベクトル？）&&（第一要素＜第二要素？）
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief パラメータと同名のファイルが存在するかどうか
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_FILE_EXIST :public JUDGEMENT
			{
			public:
				//! コンストラクタ
				JUDGEMENT_FILE_EXIST();
				//! デコンストラクタ
				~JUDGEMENT_FILE_EXIST() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	（ファイルが存在すれば1，しなければ0）
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief パラメータが，規定の値のどれかであるかどうか
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_STRING_LIST :public JUDGEMENT
			{
			private:
				//! パラメータが取るべき値リスト
				std::vector<std::string> judgement_list_;
			public:
				//! コンストラクタ
				JUDGEMENT_STRING_LIST(const std::vector<std::string> judgement_list);
				//! デコンストラクタ
				~JUDGEMENT_STRING_LIST() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	（ファイルが存在すれば1，しなければ0）
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief パラメータが，"Y","N"のどちらかであるかどうか
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_YN :public JUDGEMENT
			{
			public:
				//! コンストラクタ
				JUDGEMENT_YN();
				//! デコンストラクタ
				~JUDGEMENT_YN() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	（ファイルが存在すれば1，しなければ0）
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};

			/*!
			@brief パラメータが，ベクトルでかつ決まった要素数かどうか
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_VECTOR_SIZE :public JUDGEMENT
			{
			private:
				//! ベクトルの取るべき要素数
				int vector_size_;
			public:
				//! コンストラクタ
				JUDGEMENT_VECTOR_SIZE(int vector_size);
				//! デコンストラクタ
				~JUDGEMENT_VECTOR_SIZE() {}
				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	（パラメータはベクトル？）&&（要素数が規定値？）
				*/
				bool judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;

			};
			/*!
			@brief パラメータが乱数のシード値かどうか
			@author Kazumasa HORIE
			*/
			class JUDGEMENT_RANDOMSEED :public JUDGEMENT
			{
			public:
				//! コンストラクタ
				JUDGEMENT_RANDOMSEED() :JUDGEMENT("String \"random_device\", or a unsigned integer array ([uint1, uint2,,,])") {}
				//! デコンストラクタ
				~JUDGEMENT_RANDOMSEED() {}

				/*! @brief 判定式
				@param[in] parameters 判定するパラメータリスト
				@param[in] parameter_name パラメータ名
				@param[in] default_parameter_content パラメータ規定値
				@return	パラメータがrandom_deviceか，unsigned intベクトルならtrue
				*/
				bool judgement(parameters::PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content = "")const;
			};
		}
	}
}

