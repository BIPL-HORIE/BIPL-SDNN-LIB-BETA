#pragma once

#include<memory>
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
			//! SD部分の前方宣言
			class SDNN_SD_VIRTUAL;

			/*! @brief SD部分のファクトリメソッド　パラメータから適したSDを実体化，返却
			現状SINGLE_SDのみ
			@param[out] out_sd_pointer SDのポインタ，内部で実体化して返却
			@param[in,out] sdnn_parameter SDNNのパラメータ
			@return	(parameter >= min && parameter <= max)
			*/
			void FactorySD(std::unique_ptr<SDNN_SD_VIRTUAL> &out_sd_pointer, parameters::PARAMETERS &sdnn_parameter);

			//! NN部分の前方宣言
			class SDNN_NN_VIRTUAL;

			/*! @brief NN部分のファクトリメソッド　パラメータから適したNNを実体化，返却
			現状PPとSPのみ．SPはソフトウェアテストかけてない．
			@param[out] out_nn_pointer NNのポインタ，内部で実体化して返却
			@param[in,out] sdnn_parameter SDNNのパラメータ
			@return	(parameter >= min && parameter <= max)
			*/
			void FactoryNN(std::unique_ptr<SDNN_NN_VIRTUAL> &out_nn_pointer, parameters::PARAMETERS &sdnn_parameter);
		}
	}
}