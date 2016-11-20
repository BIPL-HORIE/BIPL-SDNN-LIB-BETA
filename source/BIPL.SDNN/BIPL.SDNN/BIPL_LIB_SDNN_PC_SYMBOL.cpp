#include "BIPL_LIB_SDNN_PC.h"
#include "BIPL_LIB_SDNN_PC_SYMBOL.h"
#include <BIPL_LIB_UTILITY.h>
#include <BIPL_LIB_LEXIAL.h>

#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>


void bipl::sdnn::base::pc::SYMBOL_ROOT::Load(const std::string &filename, std::mt19937 &mt)
{
	std::ifstream tree_file;
	utility::OpenFile(tree_file, filename);

	std::string line_buffer;
	std::vector<std::string> previous_treeline;

	while (std::getline(tree_file, line_buffer))
	{
		std::vector<std::string> treeline;
		lexial::Split(treeline, line_buffer, ',');

		int cut_flag = 0;
		for (int i = 0; i < treeline.size(); i++)
		{
			if (treeline[i] == "")
			{
				if (cut_flag == 0)
				{
					if (i < previous_treeline.size())
						treeline[i] = previous_treeline[i];
					else
						utility::error::BugFound(0x1c0023);
				}
				else
				{
					treeline.erase(treeline.begin() + i, treeline.end());
					break;
				}
			}
			else
				cut_flag = 1;
		}
		previous_treeline = treeline;
		AddSymbol(treeline, mt);
	}
	tree_file.close();
}

void bipl::sdnn::base::pc::SYMBOL_ROOT::AddSymbol(std::vector<std::string> treeline, std::mt19937 &mt)
{
	if (treeline.size() % 2 != 1)
	{
		for (auto treeline_itr = treeline.begin(); treeline_itr != treeline.end(); treeline_itr++)
			std::cout << *treeline_itr << std::endl;
		bipl::utility::error::BugFound(0x1e00ff);
	}
	if (treeline.size() == 0)
	{
		return;
	}

	std::string  finding_node_name = treeline[0];
	treeline.erase(treeline.begin());

	for (auto down_itr = down_.begin(); down_itr != down_.end(); down_itr++)
	{
		if (down_itr->GetName() == finding_node_name)
		{
			down_itr->AddSymbol(treeline, mt);
			return;
		}
	}

	SYMBOL_NODE node_buffer(finding_node_name, n_, mt);
	down_.push_back(node_buffer);
	(--down_.end())->AddSymbol(treeline, mt);
}

void bipl::sdnn::base::pc::SYMBOL_ROOT::PrintSymbol(void)
{
	for (auto down_itr = down_.begin(); down_itr != down_.end(); down_itr++)
		down_itr->PrintSymbol(0);
}
void bipl::sdnn::base::pc::SYMBOL_ROOT::GetPatternList(std::vector<std::vector<bool>> &out_list)
{
	out_list.clear();
	out_list.shrink_to_fit();
	for (auto down_itr = down_.begin(); down_itr != down_.end(); down_itr++)
		down_itr->GetPatternList(out_list);
}


bipl::sdnn::base::pc::SYMBOL_NODE::SYMBOL_NODE(const std::string &name, const int n, std::mt19937 &mt)
{
	node_name_ = name;
	pattern_.resize(n);
	pc::MakeRandomPattern(pattern_, mt);
}

bipl::sdnn::base::pc::SYMBOL_NODE::SYMBOL_NODE(const std::string &name, const std::vector<bool> &material, const double correspond, std::mt19937 &mt)
{
	node_name_ = name;
	pattern_.resize(material.size());
	int r = static_cast<int>((1.0 - correspond)* static_cast<int>(material.size()) / 2.0);
	pc::MakeRandomPattern(pattern_, material, r, mt);
}

void bipl::sdnn::base::pc::SYMBOL_NODE::AddSymbol(std::vector<std::string> treeline, std::mt19937 &mt)
{
	if (treeline.size() % 2 != 0)
	{
		for (auto treeline_itr = treeline.begin(); treeline_itr != treeline.end(); treeline_itr++)
			std::cout << *treeline_itr << std::endl;
		bipl::utility::error::BugFound(0x1e00ff);
	}
	if (treeline.size() == 0)
	{
		return;
	}
	std::string finding_node_name = treeline[1];
	double correspond = stof(treeline[0]);
	treeline.erase(treeline.begin(), treeline.begin() + 2);

	for (auto down_itr = down_.begin(); down_itr != down_.end(); down_itr++)
	{
		if (down_itr->GetName() == finding_node_name)
		{
			down_itr->AddSymbol(treeline, mt);
			return;
		}
	}

	SYMBOL_NODE node_buffer(finding_node_name, pattern_, correspond, mt);
	down_.push_back(node_buffer);
	(--down_.end())->AddSymbol(treeline, mt);
}

void bipl::sdnn::base::pc::SYMBOL_NODE::PrintSymbol(const int tagnum)
{
	for (int i = 0; i < tagnum; i++)
		std::cout << "\t";
	std::cout << "<" << node_name_ << ">";
	for (auto pattern_itr = pattern_.begin(); pattern_itr != pattern_.end(); pattern_itr++)
	{
		std::cout << *pattern_itr;
	}
	std::cout << std::endl;
	if (down_.size() != 0)
	{
		for (int i = 0; i < tagnum; i++)
			std::cout << "\t";
		std::cout << "{" << std::endl;
		for (auto down_itr = down_.begin(); down_itr != down_.end(); down_itr++)
			down_itr->PrintSymbol(tagnum + 1);
		for (int i = 0; i < tagnum; i++)
			std::cout << "\t";
		std::cout << "}" << std::endl;
	}
}
void bipl::sdnn::base::pc::SYMBOL_NODE::GetPatternList(std::vector<std::vector<bool>> &out_list)
{
	if (down_.size() == 0)//—tƒm[ƒh‚¾‚Á‚½‚ç
		out_list.push_back(pattern_);
	else
	{
		for (auto down_itr = down_.begin(); down_itr != down_.end(); down_itr++)
			down_itr->GetPatternList(out_list);
	}
}