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
				class SDNN_NN_CUSTOM :public SDNN_NN_VIRTUAL
				{
				private:
				public:
					//! コンストラクタ
					SDNN_NN_CUSTOM() {}
					//! デコンストラクタ
					~SDNN_NN_CUSTOM() {}
				};
			}
		}
	}
}