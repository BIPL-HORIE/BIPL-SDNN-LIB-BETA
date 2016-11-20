#include"BIPL_LIB_SDNN_NN_SP.h"

#include "BIPL_LIB_PARAMETERS.h"
#include "BIPL_LIB_PARAMETERS_JUDGEMENT.h"
#include "BIPL_LIB_UTILITY.h"

#include "BIPL_LIB_SDNN_PARAMETERS.h"

#include <random>
#include <omp.h>

bipl::sdnn::base::nn::SDNN_NN_SP_1vR::SDNN_NN_SP_1vR() :category_number_(0)
{
	SDNN_NN_VIRTUAL::DetermineOutputPointer = static_cast<DetermineOutput_callback>(&SDNN_NN_SP_1vR::DetermineOutputAstarisk);
	SDNN_NN_VIRTUAL::TrainPointer = static_cast<Train_callback>(&SDNN_NN_SP_1vR::TrainAstarisk);
}

void bipl::sdnn::base::nn::SDNN_NN_SP_1vR::InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length)
{
	parameter_property::PARAM_NN_SP_CATEGORY_NUMBER::property_.Read(category_number_, sdnn_parameter);

	output_neuron_number_ = category_number_;

	if (output_neuron_number_ <= 0)
		throw "output neuron number <= 0";

	w_number_per_neuron_ = input_pattern_length + 1;

	w_ = std::make_unique<int[]>(w_number_per_neuron_*output_neuron_number_);

	std::mt19937 mt;
	bipl::utility::InitMt(mt, sdnn_parameter, parameter_property::PARAM_NN_RANDOM_SEED::property_.name_, parameter_property::PARAM_NN_RANDOM_SEED::property_.default_);
	Reset(mt);
}

double bipl::sdnn::base::nn::SDNN_NN_SP_1vR::DetermineOutputAstarisk(int *adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;
	int neuron_output_sum_buffer = 0;
	int max = 0;
	int max_neuron = 0;

	omp_lock_t max_lock;
	omp_init_lock(&max_lock);

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
		if (potential > max)
		{
			omp_set_lock(&max_lock);
			max = potential;
			max_neuron = o;
			omp_unset_lock(&max_lock);
		}
	}
	omp_destroy_lock(&max_lock);
	return static_cast<double>(max_neuron);
}

void bipl::sdnn::base::nn::SDNN_NN_SP_1vR::TrainAstarisk(int* adress_data_list, double target)
{
	int adress_data_list_length = *adress_data_list++;

#ifdef _OPENMP
#pragma omp parallel for
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
		int target_direction = ((o == target) - (potential > 0));
		if (target_direction == 0)
			continue;
		w_buffer = w_.get() + o*w_number_per_neuron_;
		adress_data_list_buffer = adress_data_list;
		*w_buffer -= target_direction;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			*w_buffer += target_direction**adress_data_list_buffer++;
		}
	}
}


bipl::sdnn::base::nn::SDNN_NN_SP_1v1::SDNN_NN_SP_1v1() :category_number_(0)
{
	SDNN_NN_VIRTUAL::DetermineOutputPointer = static_cast<DetermineOutput_callback>(&SDNN_NN_SP_1v1::DetermineOutputAstarisk);
	SDNN_NN_VIRTUAL::TrainPointer = static_cast<Train_callback>(&SDNN_NN_SP_1v1::TrainAstarisk);
}

void bipl::sdnn::base::nn::SDNN_NN_SP_1v1::InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length)
{
	parameter_property::PARAM_NN_SP_CATEGORY_NUMBER::property_.Read(category_number_, sdnn_parameter);

	output_neuron_number_ = category_number_*(category_number_ - 1) / 2;

	if (output_neuron_number_ <= 0)
		throw "output neuron number <= 0";

	w_number_per_neuron_ = input_pattern_length + 1;

	w_ = std::make_unique<int[]>(w_number_per_neuron_*output_neuron_number_);

	std::mt19937 mt;
	bipl::utility::InitMt(mt, sdnn_parameter, parameter_property::PARAM_NN_RANDOM_SEED::property_.name_, parameter_property::PARAM_NN_RANDOM_SEED::property_.default_);
	Reset(mt);
}

double bipl::sdnn::base::nn::SDNN_NN_SP_1v1::DetermineOutputAstarisk(int *adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;

	int *potential = new int[output_neuron_number_];
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int o = 0; o < output_neuron_number_; o++)
	{
		int *w_buffer = w_.get() + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		potential[o] = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential[o] += *w_buffer**adress_data_list_buffer++;
		}
	}

	int *class_counter = new int[category_number_];
	for (int i = 0; i < category_number_; i++)
		class_counter[i] = 0;

	int p_counter = 0;
	for (int c_counter = 0; c_counter < category_number_; c_counter++)
	{
		for (int c2_counter = c_counter + 1; c2_counter < category_number_; c2_counter++)
		{
			if (potential[p_counter] > 0)
				class_counter[c_counter]++;
			else
				class_counter[c2_counter]++;
			p_counter++;
		}
	}

	int max = 0;
	int max_class = 0;
	for (int i = 0; i < category_number_; i++)
	{
		if (max < class_counter[i])
		{
			max = class_counter[i];
			max_class = i;
		}
	}

	delete[]class_counter;
	delete[]potential;
	return static_cast<double>(max_class);
}

void bipl::sdnn::base::nn::SDNN_NN_SP_1v1::TrainAstarisk(int* adress_data_list, double target)
{
	int adress_data_list_length = *adress_data_list++;

	int *target_potential = new int[output_neuron_number_];
	int target_counter = 0;
	for (int c_counter = 0; c_counter < category_number_; c_counter++)
	{
		for (int c2_counter = c_counter + 1; c2_counter < category_number_; c2_counter++)
		{
			if (c_counter == target)
				target_potential[target_counter] = 1;
			else if (c2_counter == target)
				target_potential[target_counter] = 0;
			else
				target_potential[target_counter] = -1;
			target_counter++;
		}
	}

#ifdef _OPENMP
#pragma omp parallel for 
#endif
	for (int o = 0; o < output_neuron_number_; o++)
	{
		if (target_potential[o] == -1)
			continue;
		int *w_buffer = w_.get() + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		int potential = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential += *w_buffer**adress_data_list_buffer++;
		}
		int target_direction = ((target_potential[o]) - (potential > 0));
		if (target_direction == 0)
			continue;
		w_buffer = w_.get() + o*w_number_per_neuron_;
		adress_data_list_buffer = adress_data_list;
		*w_buffer -= target_direction;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			*w_buffer += target_direction**adress_data_list_buffer++;
		}
	}
	delete[]target_potential;
	return;
}

//”¼‘Î”¼–@‚Í–¢ŽÀ‘•‚Å‚·D

bipl::sdnn::base::nn::SDNN_NN_SP_hvh::SDNN_NN_SP_hvh() :category_number_(0)
{
	SDNN_NN_VIRTUAL::DetermineOutputPointer = static_cast<DetermineOutput_callback>(&SDNN_NN_SP_hvh::DetermineOutputAstarisk);
	SDNN_NN_VIRTUAL::TrainPointer = static_cast<Train_callback>(&SDNN_NN_SP_hvh::TrainAstarisk);
}

void bipl::sdnn::base::nn::SDNN_NN_SP_hvh::InitNN(parameters::PARAMETERS &sdnn_parameter, const int input_pattern_length)
{
	parameter_property::PARAM_NN_SP_CATEGORY_NUMBER::property_.Read(category_number_, sdnn_parameter);

	output_neuron_number_ = category_number_*(category_number_ - 1) / 2;

	if (output_neuron_number_ <= 0)
		throw "output neuron number <= 0";

	w_number_per_neuron_ = input_pattern_length + 1;

	w_ = std::make_unique<int[]>(w_number_per_neuron_*output_neuron_number_);

	std::mt19937 mt;
	bipl::utility::InitMt(mt, sdnn_parameter, parameter_property::PARAM_NN_RANDOM_SEED::property_.name_, parameter_property::PARAM_NN_RANDOM_SEED::property_.default_);
	Reset(mt);
}

double bipl::sdnn::base::nn::SDNN_NN_SP_hvh::DetermineOutputAstarisk(int *adress_data_list)
{
	int adress_data_list_length = *adress_data_list++;
	int neuron_output_sum_buffer = 0;

	int *potential = new int[output_neuron_number_];
#ifdef _OPENMP
#pragma omp parallel for reduction(+:neuron_output_sum_buffer)
#endif
	for (int o = 0; o < output_neuron_number_; o++)
	{
		int *w_buffer = w_.get() + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		potential[o] = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential[o] += *w_buffer**adress_data_list_buffer++;
		}
	}

	int *class_counter = new int[category_number_];
	for (int i = 0; i < category_number_; i++)
		class_counter[i] = 0;

	int p_counter = 0;
	for (int c_counter = 0; c_counter < category_number_; c_counter++)
	{
		for (int c2_counter = c_counter + 1; c2_counter < category_number_; c2_counter++)
		{
			if (potential[p_counter] > 0)
				class_counter[c_counter]++;
			else
				class_counter[c2_counter]++;
			p_counter++;
		}
	}

	int max = 0;
	int max_class = 0;
	for (int i = 0; i < category_number_; i++)
	{
		if (max < class_counter[i])
		{
			max = class_counter[i];
			max_class = i;
		}
	}

	delete[]class_counter;
	delete[]potential;
	return static_cast<double>(max_class);
}

void bipl::sdnn::base::nn::SDNN_NN_SP_hvh::TrainAstarisk(int* adress_data_list, double target)
{
	int adress_data_list_length = *adress_data_list++;
	int neuron_output_sum_buffer = 0;

	int *target_potential = new int[output_neuron_number_];
	int target_counter = 0;
	for (int c_counter = 0; c_counter < category_number_; c_counter++)
	{
		for (int c2_counter = c_counter + 1; c2_counter < category_number_; c2_counter++)
		{
			if (c_counter == target)
				target_potential[target_counter] = 1;
			else if (c2_counter == target)
				target_potential[target_counter] = 0;
			else
				target_potential[target_counter] = -1;
			target_counter++;
		}
	}

#ifdef _OPENMP
#pragma omp parallel for reduction(+:neuron_output_sum_buffer)
#endif
	for (int o = 0; o < output_neuron_number_; o++)
	{
		if (target_potential[o] == -1)
			continue;
		int *w_buffer = w_.get() + o*w_number_per_neuron_;
		int *adress_data_list_buffer = adress_data_list;
		int potential = -*w_buffer;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			potential += *w_buffer**adress_data_list_buffer++;
		}
		int target_direction = ((target_potential[o]) - (potential > 0));
		if (target_direction == 0)
			continue;
		w_buffer = w_.get() + o*w_number_per_neuron_;
		adress_data_list_buffer = adress_data_list;
		*w_buffer -= target_direction;
		for (int i = 0; i < adress_data_list_length; i++)
		{
			w_buffer += *adress_data_list_buffer++;
			*w_buffer += target_direction**adress_data_list_buffer++;
		}
	}
	delete[]target_potential;
	return;
}

