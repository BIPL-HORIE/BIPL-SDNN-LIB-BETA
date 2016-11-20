#include"BIPL_LIB_SDNN_NN_VIRTUAL.h"
#include <iostream>

#include "BIPL_LIB_UTILITY.h"

bipl::sdnn::base::SDNN_NN_VIRTUAL::SDNN_NN_VIRTUAL() :w_(nullptr), w_number_per_neuron_(0), output_neuron_number_(0), initial_value_min(-5), initial_value_max(5)
{
	(bipl::sdnn::base::SDNN_NN_VIRTUAL::DetermineOutputPointer) = &SDNN_NN_VIRTUAL::DetermineOutputAstarisk;
	(bipl::sdnn::base::SDNN_NN_VIRTUAL::TrainPointer) = &SDNN_NN_VIRTUAL::TrainAstarisk;
}

bipl::sdnn::base::SDNN_NN_VIRTUAL::~SDNN_NN_VIRTUAL()
{
}

void bipl::sdnn::base::SDNN_NN_VIRTUAL::InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length)
{
	bipl::utility::error::BugFound(0x1ff0b);
}

double bipl::sdnn::base::SDNN_NN_VIRTUAL::DetermineOutputAstarisk(int *adress_data_list)
{
	bipl::utility::error::BugFound(0x1ff0b);
	return 0;
}

void bipl::sdnn::base::SDNN_NN_VIRTUAL::TrainAstarisk(int *adress_data_list, double target)
{
	bipl::utility::error::BugFound(0x1ff0b);
}

void bipl::sdnn::base::SDNN_NN_VIRTUAL::Reset(std::mt19937 &mt)
{
	if (nullptr == w_)
	{
		std::cout << "エラー：SDNNのInit前にResetをかけました" << std::endl;
		bipl::utility::Wait();
		return;
	}
	std::uniform_int_distribution<int> kernel(initial_value_min, initial_value_max);

	int *w_buffer = w_.get();
	for (int w_counter = 0; w_counter < w_number_per_neuron_ * output_neuron_number_; w_counter++)
		*w_buffer++ = kernel(mt);
}

void bipl::sdnn::base::SDNN_NN_VIRTUAL::LoadW(std::vector<int> w)
{
	if (w.size() != w_number_per_neuron_ * output_neuron_number_)
	{
		std::cout << "エラー：設定上の出力層重みの数とロードする重みの数が一致しません" << std::endl;
		utility::Wait();
		return;
	}
	int *w_buffer = w_.get();
	for (int w_counter = 0; w_counter < w_number_per_neuron_ * output_neuron_number_; w_counter++)
		*w_buffer++ = w[w_counter];
}

void bipl::sdnn::base::SDNN_NN_VIRTUAL::GetW(std::vector<int> &w)
{
	int *w_buffer = w_.get();
	for (int w_counter = 0; w_counter < w_number_per_neuron_ * output_neuron_number_; w_counter++)
		w.push_back(*w_buffer++);
}

int bipl::sdnn::base::SDNN_NN_VIRTUAL::GetWNumber(void)
{
	return w_number_per_neuron_ * output_neuron_number_;
}

void bipl::sdnn::base::SDNN_NN_VIRTUAL::GetPotential(std::vector<int> &out_potential, int* adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;
	for (int o = 0; o < output_neuron_number_; o++)
	{
		int *w_buffer = w_.get() + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		int potential = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential += *w_buffer**adress_data_list_buffer++;
		}
		out_potential.push_back(potential);
	}
}
