#include "BIPL_LIB_UTILITY.h"

#include "BIPL_LIB_PARAMETERS.h"
#include "BIPL_LIB_PARAMETERS_JUDGEMENT.h"
#include "BIPL_LIB_LEXIAL.h"

#include <iostream>
#include <fstream>
#include <string>

namespace bipl
{
	namespace utility
	{
	}
}

template<typename file_type>
std::string bipl::utility::OpenFile(file_type &file_stream, const std::string &file_name, const std::ios::openmode mode)
{
	std::string file_name_buffer = file_name;
	file_stream.open(file_name_buffer,mode);
	if (!file_stream.is_open())
	{
		std::cout << "file is not opened: " << file_name_buffer << std::endl;
		std::cout << "retype the filename correctly" << std::endl;
		while (!file_stream.is_open())
		{
			std::cout << "filename: ";
			std::cin >> file_name_buffer;
			std::cin.ignore();
			file_stream.open(file_name_buffer,mode);
		}
	}
	return file_name_buffer;
}

template std::string bipl::utility::OpenFile(std::ifstream &file_stream, const std::string &file_name, const std::ios::openmode mode);
template std::string bipl::utility::OpenFile(std::ofstream &file_stream, const std::string &file_name, const std::ios::openmode mode);

template<typename memory_type>
void bipl::utility::AllocateMemory(memory_type** &out_memory, int row_size, int column_size)
{
	if (row_size <= 0 || column_size <= 0)
	{
		utility::error::BugFound(0xf0f00f);
	}
	try
	{
		out_memory = new memory_type* [row_size];
		memory_type ** memory_buffer = out_memory;
		for (int r_count = 0; r_count < row_size; r_count++)
		{
			*memory_buffer = new memory_type[column_size];
			for (int c_count = 0; c_count < column_size; c_count++)
				(*memory_buffer)[c_count] = 0;
			memory_buffer++;
		}
	}
	catch (std::bad_alloc)
	{
		error::AllocationError(0x1);
	}
}

template void bipl::utility::AllocateMemory(int **&out_memory, int row_size, int column_size);
template void bipl::utility::AllocateMemory(double **&out_memory, int row_size, int column_size);
template void bipl::utility::AllocateMemory(unsigned int **&out_memory, int row_size, int column_size);

template<typename memory_type>
void bipl::utility::AllocateMemory(std::vector<memory_type*> &out_memory, int row_size, int column_size)
{
	if (row_size <= 0 || column_size <= 0)
	{
		utility::error::BugFound(0xf0f00f);
	}
	try
	{
		out_memory.resize(row_size);
		for (auto out_memory_itr = out_memory.begin(); out_memory_itr != out_memory.end(); out_memory_itr++)
		{
			*out_memory_itr = new memory_type[column_size];
			for (int c_count = 0; c_count < column_size; c_count++)
				(*out_memory_itr)[c_count] = 0;
		}
	}
	catch (std::bad_alloc)
	{
		error::AllocationError(0x1);
	}
}
template void bipl::utility::AllocateMemory(std::vector<unsigned int *>&out_memory, int row_size, int column_size);

void bipl::utility::Wait(void)
{
	std::cout << "Press Enter key to continue ...\n";
	do
	{
	} while (std::cin.get() != '\n');
}

void bipl::utility::InitMt(std::mt19937 &out_mt, bipl::parameters::PARAMETERS &parameter, const std::string &seed_name, const std::string &default_content)
{
	std::vector<unsigned int> random_seed;

	std::string random_seed_buffer;
	parameters::judgement::JUDGEMENT_RANDOMSEED judgement_randomseed;

	parameter.ReadParameter(random_seed_buffer, seed_name, default_content, &judgement_randomseed);
	if (random_seed_buffer == RANDOM_DEVICE)
	{
		random_seed.clear();
		std::random_device random_buffer;
		for (int counter = 0; counter < 10; counter++)
		{
			random_seed.push_back(random_buffer());
		}
		parameter.OverwriteParameter(seed_name, random_seed);
	}
	else
	{
		parameter.ReadParameter(random_seed, seed_name, default_content);
	}
	std::seed_seq random_seed_seq(random_seed.begin(), random_seed.end());
	out_mt.seed(random_seed_seq);
}

void bipl::utility::error::BugFound(int place)
{
	std::cout << "Unexpected error: (" << place <<  ")" << std::endl;
	std::ofstream ofs("error.txt");
	ofs << "Unexpected error: (" << place << ")" << std::endl;
	ofs.close();
	Wait();
	exit(1);
}

void bipl::utility::error::AllocationError(int place)
{
	std::cout << "Memory allocation error: (" << std::ios::hex << place << std::ios::dec << ")" << std::endl;
	Wait();
	exit(1);
}
