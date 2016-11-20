#include "BIPL_LIB_SDNN_ANALYSIS.h"
#include "BIPL_LIB_SDNN_ROC.h"
#include "BIPL_LIB_UTILITY.h"

bipl::sdnn::analysis::ROC_POINT::ROC_POINT() :true_positive_(0), false_positive_(0), potential_(0)
{}

bipl::sdnn::analysis::ROC_POINT::~ROC_POINT()
{}

void bipl::sdnn::analysis::ROC_POINT::SetRoc(const int potential, const double true_positive, const  double false_positive)
{
	if (true_positive < 0 || true_positive > 1)
		bipl::utility::error::BugFound(0x10101);
	if (false_positive < 0 || false_positive > 1)
		bipl::utility::error::BugFound(0x10101);
	true_positive_ = true_positive;
	false_positive_ = false_positive;
	potential_ = potential;
}

const void bipl::sdnn::analysis::ROC_POINT::GetRoc(int &out_potential, double &out_true_positive, double &out_false_positive)
{
	out_potential = potential_;
	out_true_positive = true_positive_;
	out_false_positive = false_positive_;
}

void bipl::sdnn::analysis::GetROC(std::vector<ROC_CURVE> &out_roc_curve, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target)
{
	base::GetROC()(out_roc_curve, sdnn, input, target);
}

double bipl::sdnn::analysis::GetAUC(bipl::sdnn::analysis::ROC_CURVE &roc_curve)
{
	if (roc_curve.size() < 2)
		bipl::utility::error::BugFound(0x10102);

	double auc = 0;
	for (unsigned int roc_counter = 0; roc_counter < roc_curve.size() - 1; roc_counter++)
	{
		double false_positive, true_positive;
		double next_false_positive, next_true_positive;
		int potential;
		roc_curve[roc_counter].GetRoc(potential, true_positive, false_positive);
		roc_curve[roc_counter + 1].GetRoc(potential, next_true_positive, next_false_positive);

		double false_dif = next_false_positive - false_positive;
		double true_add = next_true_positive + true_positive;
		auc += true_add*false_dif;
	}
	return auc / 2;
}