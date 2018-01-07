#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <BIPL_LIB_SDNN.h>
#include <BIPL_LIB_SDNN_PARAMETERS.h>
#include <BIPL_LIB_UTILITY.h>
#pragma comment(lib,"BIPL.SDNN.lib")

void file2sample(const std::string &filename, std::vector<std::vector<double>> &input, std::vector<double> &target)
{
	std::ifstream train_sample;

	bipl::utility::OpenFile(train_sample, filename);

	std::string buffer;
	while (std::getline(train_sample, buffer, '\n'))
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

	sdnn.Load(argv[1]);

	std::vector<double> target;
	std::vector<std::vector<double>> input;
	file2sample(argv[2], input, target);

	std::ofstream output_file;
	bipl::utility::OpenFile(output_file, argv[3]);

	std::string nn_type;
	sdnn.GetParameter(nn_type, bipl::sdnn::parameter_property::PARAM_SDNN_TYPE::property_.name_);

	if (nn_type == bipl::sdnn::parameter_property::PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::function_approximation_)
	{
		double rmse = 0;
		double result;
		for (int i = 0; i < input.size(); i++)
		{
			result = sdnn.Estimate(input[i]);
			rmse += pow(result - target[i], 2);
			output_file << target[i] << "," << result << std::endl;
		}
		rmse /= input.size();
		rmse = sqrt(rmse);
		std::cout << "rmse = " << rmse << std::endl;
	}
	else if (nn_type == bipl::sdnn::parameter_property::PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::pattern_recognition_)
	{
		int correct = 0;
		double result;
		for (int i = 0; i < input.size(); i++)
		{
			result = sdnn.Estimate(input[i]);
			correct += (result == target[i]);
			output_file << target[i] << "," << result << std::endl;
		}
		std::cout << "error = " << 1.0 - static_cast<double>(correct) / input.size() << std::endl;
	}

	output_file.close();
	bipl::utility::Wait();
	return 0;
}