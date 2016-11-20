#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <BIPL_LIB_SDNN.h>
#include <BIPL_LIB_SDNN_PARAMETERS.h>
#include <BIPL_LIB_UTILITY.h>
#pragma comment(lib,"BIPL.SDNN.lib")

void file2sample(const std::string &filename, std::vector<std::vector<double>> &input, std::vector<double> &target)
{
	std::ifstream train_sample(filename);
	std::string buffer;
	while (std::getline(train_sample, buffer))
	{
		std::string buffer2nd;
		std::stringstream ss(buffer);
		std::getline(ss, buffer2nd, ',');
		target.push_back(std::stod(buffer2nd));
		std::vector<double> input_buffer;
		while (std::getline(ss, buffer2nd, ','))
		{
			input_buffer.push_back(std::stod(buffer2nd));
		}
		input.push_back(input_buffer);
	}
}

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cout << "command error" << std::endl;
		return 0;
	}
	bipl::sdnn::SDNN sdnn;
	
	sdnn.InitSDNN(argv[1]);
	int input_number;
	sdnn.GetParameter(input_number, bipl::sdnn::parameter_property::PARAM_INPUT_NUMBER::property_.name_);

	std::vector<double> target;
	std::vector<std::vector<double>> input;
	file2sample(argv[2], input, target);

	for (auto input_itr = input.begin(); input_itr != input.end(); input_itr++)
	{
		if (input_itr->size() != input_number)
		{
			std::cout << "error";
			exit(1);
		}
	}

	auto start = std::chrono::system_clock::now();
	sdnn.Train(input, target);
	auto end = std::chrono::system_clock::now();
	auto diff2 = end - start;
	std::cout << "elapsed time = "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(diff2).count()
		<< " msec."
		<< std::endl;

	std::string nn_type;
	sdnn.GetParameter(nn_type, bipl::sdnn::parameter_property::PARAM_NN_TYPE::property_.name_);

	if (nn_type == bipl::sdnn::parameter_property::PARAM_NN_TYPE::CNT_NN_TYPE_::pp_)
	{
		auto target_itr = target.begin();
		double rmse = 0;
		for (auto input_itr = input.begin(); input_itr != input.end(); input_itr++)
		{
			rmse += pow(*target_itr++ - sdnn.Estimate(*input_itr), 2);
		}
		rmse /= input.size();
		rmse = sqrt(rmse);
		std::cout << "rmse = " << rmse << std::endl;
	}
	else if (nn_type == bipl::sdnn::parameter_property::PARAM_NN_TYPE::CNT_NN_TYPE_::sp_)
	{
		auto target_itr = target.begin();
		int correct = 0;
		for (auto input_itr = input.begin(); input_itr != input.end(); input_itr++)
		{
			correct += (*target_itr++ == sdnn.Estimate(*input_itr));
		}
		std::cout << "training_error = " << 1.0 - static_cast<double>(correct) / input.size() << std::endl;
	}
	sdnn.Save(argv[3]);
	
	bipl::utility::Wait();

	return 0;
}