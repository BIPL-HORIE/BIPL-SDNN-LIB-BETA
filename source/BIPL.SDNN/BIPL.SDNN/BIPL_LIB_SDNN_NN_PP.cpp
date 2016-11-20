 #include"BIPL_LIB_SDNN_NN_PP.h"

#include "BIPL_LIB_PARAMETERS.h"
#include "BIPL_LIB_PARAMETERS_JUDGEMENT.h"
#include "BIPL_LIB_SDNN_PARAMETERS.h"
#include "BIPL_LIB_UTILITY.h"

#include <iostream>
#include<string>
#include<vector>
#include <random>
#include <algorithm>
#include <omp.h>
#include <thread>

class bipl::sdnn::base::nn::SDNN_NN_PP::CHOOSE_LESS_POTENTIAL
{
public:
	bool operator()(const NEURON_OUTPUT& a, const NEURON_OUTPUT& b) const {
		return (a.potential != b.potential) ? (a.potential < b.potential) : (a.index < b.index);
	}
};

bipl::sdnn::base::nn::SDNN_NN_PP::SDNN_NN_PP() :a_(0), b_(0)
{
	SDNN_NN_VIRTUAL::DetermineOutputPointer = static_cast<DetermineOutput_callback>(&SDNN_NN_PP::DetermineOutputAstarisk_MultiThread);
	SDNN_NN_VIRTUAL::TrainPointer = static_cast<Train_callback>(&SDNN_NN_PP::TrainAstarisk_MultiThread);
}

void bipl::sdnn::base::nn::SDNN_NN_PP::InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length)
{
	std::vector<double> output_range;
	parameter_property::PARAM_NN_PP_OUTPUT_RANGE::property_.Read(output_range, sdnn_parameter);

	double step_size;
	auto judgement_buffer = std::make_unique<bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<double>>(0.000001, (output_range[1] - output_range[0]));
	parameter_property::PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE::property_.judgement_ = std::move(judgement_buffer);
	parameter_property::PARAM_NN_PP_OUTPUT_QUANTIZATION_STEP_SIZE::property_.Read(step_size, sdnn_parameter);

	output_neuron_number_ = static_cast<int>((output_range[1] - output_range[0]) / step_size);
	if (((output_range[1] - output_range[0]) / step_size) != output_neuron_number_)
	{
		std::cout << "�x��: �o�͂Ɍ덷���܂܂��\��������܂��D" << std::endl;
		std::cout << "�x��: �o�͂̕��Ƃ��̍��ݕ��ɂ��Č������Ă�������" << std::endl;
	}

	w_number_per_neuron_ = input_pattern_length + 1;

	w_ = std::make_unique<int[]>(w_number_per_neuron_*output_neuron_number_);

	std::vector<int> initial_value_range;
	parameter_property::PARAM_NN_INITIAL_VALUE_RANGE::property_.Read(initial_value_range, sdnn_parameter);
	initial_value_min = initial_value_range[0];
	initial_value_max = initial_value_range[1];

	std::mt19937 mt;
	bipl::utility::InitMt(mt, sdnn_parameter, parameter_property::PARAM_NN_RANDOM_SEED::property_.name_, parameter_property::PARAM_NN_RANDOM_SEED::property_.default_);
	Reset(mt);

	a_ = step_size;
	b_ = output_range[0];

	std::string use_multi_thread;
	parameter_property::PARAM_OPTION_MULTI_THREAD_USE::property_.Read(use_multi_thread, sdnn_parameter);

	if (use_multi_thread == parameter_property::PARAM_OPTION_MULTI_THREAD_USE::CNT_OPTION_MULTI_THREAD_USE::y_)
	{
		SDNN_NN_VIRTUAL::DetermineOutputPointer = static_cast<DetermineOutput_callback>(&SDNN_NN_PP::DetermineOutputAstarisk_MultiThread);
		SDNN_NN_VIRTUAL::TrainPointer = static_cast<Train_callback>(&SDNN_NN_PP::TrainAstarisk_MultiThread);

	}
	else
	{
		SDNN_NN_VIRTUAL::DetermineOutputPointer = static_cast<DetermineOutput_callback>(&SDNN_NN_PP::DetermineOutputAstarisk);
		SDNN_NN_VIRTUAL::TrainPointer = static_cast<Train_callback>(&SDNN_NN_PP::TrainAstarisk);
	}
}

double bipl::sdnn::base::nn::SDNN_NN_PP::DetermineOutputAstarisk(int *adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;
	int neuron_output_sum_buffer = 0;
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
		neuron_output_sum_buffer += (potential > 0);
	}

	return a_*neuron_output_sum_buffer + b_;
}

double bipl::sdnn::base::nn::SDNN_NN_PP::DetermineOutputAstarisk_MultiThread(int *adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;
	int neuron_output_sum_buffer = 0;
#ifdef _OPENMP
#pragma omp parallel for reduction(+:neuron_output_sum_buffer)
#endif
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
		neuron_output_sum_buffer += (potential > 0);
	}

	return a_*neuron_output_sum_buffer + b_;
}
/*
double bipl::sdnn::base::nn::SDNN_NN_PP::DetermineOutputAstarisk_MultiThread_C11thread(int *adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;
	int neuron_output_sum_buffer = 0;
	int *w_buffer = w_;
	int w_number_per_neuron_buffer = w_number_per_neuron_;

	std::vector<std::thread> threads;
	for (int t_counter = 0; t_counter < threads_number_; t_counter)
	{
		int start = start_array_[t_counter];
		int end = end_array_[t_counter];

		threads.push_back(std::thread([start,end,w_buffer,w_number_per_neuron_buffer,adress_data_list,adress_data_list_length,&neuron_output_sum_buffer]
		{
			for (int o = start; o <= end; o++)
			{
				int *w_buf = w_buffer + o*w_number_per_neuron_buffer;
				int *adress_data_list_buffer = adress_data_list;
				int potential = -*w_buf;
				for (int i = 0; i < adress_data_list_length; i++)
				{
					w_buf += *adress_data_list_buffer++;
					potential += *w_buf**adress_data_list_buffer++;
				}
				neuron_output_sum_buffer += (potential > 0);
			}
		}));
	}
	for (std::thread &th : threads)
	{
		th.join();
	}

	return a_*neuron_output_sum_buffer + b_;
	return 0;
}
*/

void bipl::sdnn::base::nn::SDNN_NN_PP::TrainAstarisk(int *adress_data_list, double target)
{
	std::vector<NEURON_OUTPUT> result0(output_neuron_number_);
	std::vector<NEURON_OUTPUT> result1(output_neuron_number_);

	int neuron_output_sum_buffer = 0;
	int adress_data_list_length = *adress_data_list++;
	int w_number_per_neuron_access_buffer = w_number_per_neuron_;
	int *w_access_buffer = w_.get();

	for (int o = 0; o < output_neuron_number_; o++)
	{
		int *w_buffer = w_access_buffer + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		int potential = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential += *w_buffer**adress_data_list_buffer++;
		}
		neuron_output_sum_buffer += (potential > 0);

		result0[o].potential = (potential > 0) ? potential : 0x8fffff;
		result0[o].index = o;
		result1[o].potential = (potential <= 0) ? -potential : 0x8fffff;
		result1[o].index = o;
	}

	neuron_output_sum_buffer -= static_cast<int>(0.5 + (target - b_) / a_);

	if (neuron_output_sum_buffer == 0)
		return;

	int target_direction = 1;
	NEURON_OUTPUT *looking_result_buffer;

	if (neuron_output_sum_buffer < 0)
	{
		looking_result_buffer = result1.data();
		neuron_output_sum_buffer *= -1;
	}
	else
	{
		looking_result_buffer = result0.data();
		target_direction = -1;
	}

	std::partial_sort(looking_result_buffer, looking_result_buffer + neuron_output_sum_buffer,
		looking_result_buffer + output_neuron_number_, CHOOSE_LESS_POTENTIAL());

	for (int t = 0; t < neuron_output_sum_buffer; t++)
	{
		int *w_buffer = w_.get() + w_number_per_neuron_ * looking_result_buffer[t].index;
		int *adress_data_list_buffer = adress_data_list;
		*w_buffer -= target_direction;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			*w_buffer += target_direction**adress_data_list_buffer++;
		}
	}
}

void bipl::sdnn::base::nn::SDNN_NN_PP::TrainAstarisk_MultiThread(int *adress_data_list, double target)
{
	std::vector<NEURON_OUTPUT> result0(output_neuron_number_);
	std::vector<NEURON_OUTPUT> result1(output_neuron_number_);

	int neuron_output_sum_buffer = 0;
	int adress_data_list_length = *adress_data_list++;
	int w_number_per_neuron_access_buffer = w_number_per_neuron_;
	int *w_access_buffer = w_.get();

#ifdef _OPENMP
#pragma omp parallel for reduction(+:neuron_output_sum_buffer)
#endif
	for (int o = 0; o < output_neuron_number_; o++)
	{
		int *w_buffer = w_access_buffer + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		int potential = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential += *w_buffer**adress_data_list_buffer++;
		}
		neuron_output_sum_buffer += (potential > 0);

		result0[o].potential = (potential > 0) ? potential : 0x8fffff;
		result0[o].index = o;
		result1[o].potential = (potential <= 0) ? -potential : 0x8fffff;
		result1[o].index = o;
	}

	neuron_output_sum_buffer -= static_cast<int>(0.5 + (target - b_) / a_);

	if (neuron_output_sum_buffer == 0)
		return;

	int target_direction = 1;
	NEURON_OUTPUT *looking_result_buffer;

	if (neuron_output_sum_buffer < 0)
	{
		looking_result_buffer = result1.data();
		neuron_output_sum_buffer *= -1;
	}
	else
	{
		looking_result_buffer = result0.data();
		target_direction = -1;
	}

	std::partial_sort(looking_result_buffer, looking_result_buffer + neuron_output_sum_buffer,
		looking_result_buffer + output_neuron_number_, CHOOSE_LESS_POTENTIAL());

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int t = 0; t < neuron_output_sum_buffer; t++)
	{
		int *w_buffer = w_.get() + w_number_per_neuron_ * looking_result_buffer[t].index;
		int *adress_data_list_buffer = adress_data_list;
		*w_buffer -= target_direction;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			*w_buffer += target_direction**adress_data_list_buffer++;
		}
	}
}