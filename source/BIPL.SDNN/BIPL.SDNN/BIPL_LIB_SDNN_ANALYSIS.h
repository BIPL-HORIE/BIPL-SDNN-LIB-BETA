#pragma once

#include<vector>

namespace bipl
{
	namespace sdnn
	{
		class SDNN;
		namespace analysis
		{
			class ROC_POINT
			{
			private:
				int potential_;
				double true_positive_;
				double false_positive_;
			public:
				ROC_POINT();
				~ROC_POINT();
				void SetRoc(const int potential_, const double true_positive, const double false_positive);
				const void GetRoc(int &potential, double &out_true_positive, double &out_false_positive);
			};
			typedef std::vector<ROC_POINT> ROC_CURVE;

			void GetROC(std::vector<ROC_CURVE> &out_roc_curve, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target);
			double GetAUC(ROC_CURVE &roc_curve);
		}
	}
}