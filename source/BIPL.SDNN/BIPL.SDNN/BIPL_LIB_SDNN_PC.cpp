#include "BIPL_LIB_SDNN_PC.h"
#include "BIPL_LIB_SDNN_PC_SYMBOL.h"
#include "BIPL_LIB_LEXIAL.h"
#include "BIPL_LIB_UTILITY.h"
#include "BIPL_LIB_SDNN_PARAMETERS.h"

#include <vector>
#include <string>
#include <random>
#include <iostream>
namespace bipl
{
	namespace sdnn
	{
		namespace base
		{
			namespace pc
			{
				void MakeRandomPattern(std::vector<bool> &out_pattern, std::mt19937 &mt)
				{
					int n = static_cast<int>(out_pattern.size());
					if (n % 2)
						bipl::utility::error::BugFound(0x1ff0f);

					int random_buffer;

					std::vector<bool> pattern_buffer(n, 0);//0初期化，面倒なのでswap
					out_pattern.swap(pattern_buffer);

					std::uniform_int_distribution<int> kernel(0, n - 1);
					for (int i = 0; i < n / 2; i++)
					{
						do
						{
							random_buffer = kernel(mt);
						} while (out_pattern[random_buffer] != 0);
						out_pattern[random_buffer] = 1;
					}
				}

				void MakeRandomPattern(std::vector<bool> &out_pattern, const std::vector<bool> &material, const int r, std::mt19937 &mt)
				{
					if (out_pattern.size() != material.size())
					{
						utility::error::BugFound(0x1b0ff0a);
					}
					int n = static_cast<int>(out_pattern.size());
					std::uniform_int_distribution<int> kernel(0, n - 1);

					//フラグのクリア
					std::vector<bool> inverse_flag(n,0);
					out_pattern = material;

					//1のものをr個選択
					for (int r_counter = 0; r_counter < r; r_counter++)
					{
						int random_buffer;
						do
						{
							random_buffer = kernel(mt);
						} while (inverse_flag[random_buffer] == 1 || out_pattern[random_buffer] == 0);
						out_pattern[random_buffer] = 0;
						inverse_flag[random_buffer] = 1;
						do
						{
							random_buffer = kernel(mt);
						} while (inverse_flag[random_buffer] == 1 || out_pattern[random_buffer] == 1);
						out_pattern[random_buffer] = 1;
						inverse_flag[random_buffer] = 1;
					}
				}

				void SavePattern(unsigned int *binary, const std::vector<bool> &pattern)
				{
					auto pattern_itr = pattern.begin();
					for (int n_counter = 0; n_counter < pattern.size() - 1; n_counter++)
					{
						binary[n_counter >> 5] += *pattern_itr++;
						if (n_counter % 32 != 31)
							binary[n_counter >> 5] <<= 1;
					}
					binary[(pattern.size() - 1) >> 5] += *pattern_itr;
				}
			}
		}
	}
}

bipl::sdnn::base::SDNN_PC::SDNN_PC():n_(0),n32_(0)
{}

bipl::sdnn::base::SDNN_PC::~SDNN_PC()
{
	for (auto binary_itr = binary_pattern_.begin(); binary_itr != binary_pattern_.end(); binary_itr++)
		delete[] *binary_itr;
}

void bipl::sdnn::base::SDNN_PC::InitPC(const int n, const std::string &pattern_type, std::mt19937 &mt)
{
	//pattern_type
	//NUMERICAL(q,RANDOM_INVERSE(r))
	//NUMERICAL(q,INTERPOLATION(r))
	//SYMBOL(q,RANDOM_INVERSE(r))
	//SYMBOL(SYMBOL_TREE(filename))

	std::vector<std::string> setting_buffer_vector;
	bipl::lexial::Split(setting_buffer_vector,pattern_type, '(');
	setting_buffer_vector[2].erase(--setting_buffer_vector[2].end());
	setting_buffer_vector[2].erase(--setting_buffer_vector[2].end());

	n_ = n;
	n32_ = ((n_ - 1) >> 5) + 1;

	if (setting_buffer_vector[0] == parameter_property::PARAM_SD_PC_TYPE::CNT_INPUT_TYPE_::numerical_)
	{
		//setting_buffer_vector = {NUMERICAL / q,RANDOM_INVERSE / r}
		//setting_buffer_vector = {NUMERICAL / q,INTERPOLATION / r}
		std::vector<std::string> q_make;
		bipl::lexial::Split(q_make, setting_buffer_vector[1], ',');
		if (q_make[1] == parameter_property::PARAM_SD_PC_TYPE::CNT_PC_METHOD_::random_inverse_)
			RandomInverse(stoi(q_make[0]), stoi(setting_buffer_vector[2]), mt);
		else if (q_make[1] == parameter_property::PARAM_SD_PC_TYPE::CNT_PC_METHOD_::interpolation_)
			Interpolation(stoi(q_make[0]), stoi(setting_buffer_vector[2]), mt);
		else
			utility::error::BugFound(0x1ff0a);
	}
	else if(setting_buffer_vector[0] == parameter_property::PARAM_SD_PC_TYPE::CNT_INPUT_TYPE_::symbol_)
	{
		//setting_buffer_vector = {SYMBOL / q,RANDOM_INVERSE / r}
		// setting_buffer_vector = {SYMBOL / SYMBOL_TREE / filename}
		std::vector<std::string> q_make;
		bipl::lexial::Split(q_make, setting_buffer_vector[1], ',');
		switch (q_make.size())
		{
		case 1:
			if(q_make[0] == parameter_property::PARAM_SD_PC_TYPE::CNT_PC_METHOD_::correlation_tree_)
				SymbolTree(setting_buffer_vector[2], mt);
			else
				utility::error::BugFound(0x1fe0a);
			break;
		case 2:
			if (q_make[1] == parameter_property::PARAM_SD_PC_TYPE::CNT_PC_METHOD_::random_inverse_)
				RandomInverse(stoi(q_make[0]), stoi(setting_buffer_vector[2]), mt);
			else
				utility::error::BugFound(0x1fe0b);
			break;
		default:
			utility::error::BugFound(0x1fe0c);
		}
	}
	else
		utility::error::BugFound(0x22);
}

void bipl::sdnn::base::SDNN_PC::RandomInverse(const int q, const int r, std::mt19937 &mt)
{
	utility::AllocateMemory(binary_pattern_, q, n32_);
	std::vector<bool> pattern(n_);
	std::vector<bool> previous_pattern(n_);

	auto binary_itr = binary_pattern_.begin();
	pc::MakeRandomPattern(pattern, mt);
	pc::SavePattern(*binary_itr++, pattern);
	for (;binary_itr != binary_pattern_.end(); binary_itr++)
	{
		previous_pattern = pattern;
		pc::MakeRandomPattern(pattern, previous_pattern, r, mt);
		pc::SavePattern(*binary_itr, pattern);
	}
}

void bipl::sdnn::base::SDNN_PC::Interpolation(const int q, const int r, std::mt19937 &mt)
{
	utility::AllocateMemory(binary_pattern_, q, n32_);

	int split_width = q / r;

	std::vector<bool> pattern(n_);
	std::vector<bool> first_pattern(n_);
	std::vector<bool> next_pattern(n_);

	std::vector<int> difference(n_);

	std::uniform_int_distribution<int> kernel(0, n_ - 1);

	//パターンの作成

	pc::MakeRandomPattern(pattern, mt);
	first_pattern = pattern;
	std::uniform_int_distribution<int> kernel_sw(0, split_width - 1);

	for (int s_counter = 0; s_counter < r; s_counter++)
	{
		if (s_counter == r - 1)
			next_pattern = first_pattern;
		else
			pc::MakeRandomPattern(next_pattern, mt);

		int different_number = 0;
		for (int n_counter = 0; n_counter < n_; n_counter++)
		{
			difference[n_counter] = static_cast<int>(next_pattern[n_counter] - pattern[n_counter]);
			if (difference[n_counter] == 1)
				different_number++;
		}

		std::vector<int> reverse(split_width, (different_number - (different_number % (split_width))) / (split_width));
		different_number %= (split_width);

		if (different_number != 0)
		{
			int default_reverse_number = reverse[0];
			while (different_number > 0)
			{
				int reverse_buf = 0;
				do
				{
					reverse_buf = kernel_sw(mt);
				} while (reverse[reverse_buf] != default_reverse_number);
				reverse[reverse_buf]++;
				different_number--;
			}
		}

		//patternのbinary変換
		pc::SavePattern(binary_pattern_[(s_counter*split_width)], pattern);

		for (int sw_counter = 0; sw_counter < split_width - 1; sw_counter++)
		{
			for (int r_counter = 0; r_counter < reverse[sw_counter]; r_counter++)
			{
				int random_buffer;
				do
				{
					random_buffer = kernel(mt);
				} while (difference[random_buffer] != 1);
				pattern[random_buffer] = !pattern[random_buffer];
				difference[random_buffer] = 0;

				do
				{
					random_buffer = kernel(mt);
				} while (difference[random_buffer] != -1);
				pattern[random_buffer] = !pattern[random_buffer];
				difference[random_buffer] = 0;
			}
			pc::SavePattern(binary_pattern_[(s_counter*split_width + 1 + sw_counter)], pattern);
		}

		//nextpatternをpatternに移送
		pattern = next_pattern;
	}

	// 先頭と末尾をコピー
	for (int n32_counter = 0; n32_counter < n32_; n32_counter++)
		binary_pattern_[q - 1][n32_counter] = binary_pattern_[0][n32_counter];
}

void bipl::sdnn::base::SDNN_PC::SymbolTree(const std::string &tree_filename, std::mt19937 &mt)
{
	pc::SYMBOL_ROOT symbol_tree(n_);
	symbol_tree.Load(tree_filename, mt);

	std::vector<std::vector<bool>> pattern_list;
	symbol_tree.GetPatternList(pattern_list);

	utility::AllocateMemory(binary_pattern_, static_cast<int>(pattern_list.size()), n32_);

	auto pattern_list_itr = pattern_list.begin();
	for (auto binary_itr = binary_pattern_.begin(); binary_itr != binary_pattern_.end(); binary_itr++)
	{
		pc::SavePattern(*binary_itr, *pattern_list_itr++);
	}
}
