#include "BIPL_LIB_SDNN.h"
#include "BIPL_LIB_SDNN_ROC.h"
#include "BIPL_LIB_UTILITY.h"
#include <algorithm>
#include <iostream>

class bipl::sdnn::analysis::base::GetROC::POTENTIAL_AND_TF
{
public:
	int potential;
	bool tf;
};

class bipl::sdnn::analysis::base::GetROC::ChooseHighPotential
{
public:
	bool operator()(const POTENTIAL_AND_TF& a, const POTENTIAL_AND_TF& b) const
	{
		return a.potential > b.potential;
	}
};

void bipl::sdnn::analysis::base::GetROC::ConvertPT2ROC(ROC_CURVE &out_roc_curve, std::vector<POTENTIAL_AND_TF> &pat)
{
	std::sort(pat.begin(),pat.end(),ChooseHighPotential());

	int true_number = 0;
	int false_number = 0;

	for (unsigned int potential_counter = 0; potential_counter < pat.size(); potential_counter++)
	{
		if (pat[potential_counter].tf)
			true_number++;
		else
			false_number++;
	}

	ROC_POINT roc_buffer;
	roc_buffer.SetRoc(pat[0].potential-1, 0, 0);
	
	out_roc_curve.push_back(roc_buffer);

	int true_count = 0;
	int false_count = 0;
	int last_potential = pat[0].potential;
	for (unsigned int potential_counter = 0; potential_counter < pat.size(); potential_counter++)
	{
		if (last_potential != pat[potential_counter].potential)
		{
			last_potential = pat[potential_counter].potential;
			roc_buffer.SetRoc(last_potential,(double)true_count / true_number, (double)false_count / false_number);
			out_roc_curve.push_back(roc_buffer);
		}
		if (pat[potential_counter].tf)
			true_count++;
		else
			false_count++;
	}
	roc_buffer.SetRoc(last_potential+1, 1, 1);
	out_roc_curve.push_back(roc_buffer);
}

void bipl::sdnn::analysis::base::GetROC::GetROC1v1(std::vector<ROC_CURVE> &out_roc_curve, int output_neuron_number, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target)
{
}

void bipl::sdnn::analysis::base::GetROC::GetROC1vR(std::vector<ROC_CURVE> &out_roc_curve, int output_neuron_number, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target)
{
	out_roc_curve.clear();
	out_roc_curve.resize(output_neuron_number);

	if (input.size() != target.size())
		utility::error::BugFound(0x10106);

	std::vector<std::vector<POTENTIAL_AND_TF>> potential_and_tf(output_neuron_number); //[neuron_number][sample_number];

	//各サンプルに対する内部電位を取得する．
	for (unsigned int i = 0; i < input.size(); i++)
	{
		std::vector<int> inner_potential;
		sdnn.GetPotential(inner_potential, input[i]);
		for (int output_neuron_counter = 0; output_neuron_counter < output_neuron_number; output_neuron_counter++)
		{
			POTENTIAL_AND_TF buffer;
			buffer.potential = inner_potential[output_neuron_counter];
			buffer.tf = (output_neuron_counter == target[i]);

			potential_and_tf[output_neuron_counter].push_back(buffer);
		}
	}

	for (int output_neuron_counter = 0; output_neuron_counter < output_neuron_number; output_neuron_counter++)
	{
		ConvertPT2ROC(out_roc_curve[output_neuron_counter],potential_and_tf[output_neuron_counter]);
	}
}

void bipl::sdnn::analysis::base::GetROC::operator()(std::vector<ROC_CURVE> &out_roc_curve, SDNN &sdnn, std::vector<std::vector<double>> &input, std::vector<double> &target)
{
	std::string nn_type;
	sdnn.GetParameter(nn_type, "SDNN\\NN\\type");
	if(nn_type != "SP")
		bipl::utility::error::BugFound(0x10103);

	std::string category_num_buffer;
	sdnn.GetParameter(category_num_buffer, "SDNN\\NN\\category_number");
	int output_neuron_number;
	output_neuron_number = stoi(category_num_buffer);

	sdnn.GetParameter(nn_type, "SDNN\\NN\\multi_class_recognition_method");
	if (nn_type == "1vR")
	{
		GetROC1vR(out_roc_curve, output_neuron_number, sdnn, input, target);
	}
	else if (nn_type == "1v1")
	{
		bipl::utility::error::BugFound(0x10105); //1v1でROC書く必要性がわからない．

		output_neuron_number *= (output_neuron_number - 1);
		output_neuron_number /= 2;
		GetROC1v1(out_roc_curve, output_neuron_number, sdnn, input, target);
	}
	else
	{
		bipl::utility::error::BugFound(0x10104);
	}
}
