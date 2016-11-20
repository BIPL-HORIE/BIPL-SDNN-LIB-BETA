#pragma once
#include "BIPL_LIB_SDNN_NN_VIRTUAL.h"
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
				class SDNN_NN_SP_1vR :public SDNN_NN_VIRTUAL
				{
				private:
					int category_number_;
					double DetermineOutputAstarisk(int *adress_data_list);
					void TrainAstarisk(int *adress_data_list, double target);

				public:
					SDNN_NN_SP_1vR();
					~SDNN_NN_SP_1vR() {}
					void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);
				};
				class SDNN_NN_SP_1v1 :public SDNN_NN_VIRTUAL
				{
				private:
					int category_number_;
					double DetermineOutputAstarisk(int *adress_data_list);
					void TrainAstarisk(int *adress_data_list, double target);

				public:
					SDNN_NN_SP_1v1();
					~SDNN_NN_SP_1v1() {}
					void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);
				};
				class SDNN_NN_SP_hvh :public SDNN_NN_VIRTUAL
				{
				private:
					int category_number_;
					double DetermineOutputAstarisk(int *adress_data_list);
					void TrainAstarisk(int *adress_data_list, double target);

				public:
					SDNN_NN_SP_hvh();
					~SDNN_NN_SP_hvh() {}
					void InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length);
				};
			}
		}
	}
}