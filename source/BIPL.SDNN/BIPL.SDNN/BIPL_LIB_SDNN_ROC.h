#pragma once

#include <vector>
#include "BIPL_LIB_SDNN_ANALYSIS.h"

namespace bipl
{
	namespace sdnn
	{
		class SDNN;
		namespace analysis
		{
			namespace base
			{
				class GetROC
				{
				private:
					class POTENTIAL_AND_TF;
					class ChooseHighPotential;
					void ConvertPT2ROC(ROC_CURVE &out_roc_curve, std::vector<POTENTIAL_AND_TF> &pat);
					void GetROC1v1(std::vector<ROC_CURVE> &out_roc_curve, int output_neuron_number, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target);
					void GetROC1vR(std::vector<ROC_CURVE> &out_roc_curve, int output_neuron_number, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target);
				public:
					void operator()(std::vector<ROC_CURVE> &out_roc_curve, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target);
				};
			}
		}
	}
}
