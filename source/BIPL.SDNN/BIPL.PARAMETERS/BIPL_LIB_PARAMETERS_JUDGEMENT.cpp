#include "BIPL_LIB_PARAMETERS_JUDGEMENT.h"
#include "BIPL_LIB_UTILITY.h"
#include "BIPL_LIB_LEXIAL.h"
#include <vector>
#include <iostream>
#include <fstream>

bool bipl::parameters::judgement::JUDGEMENT::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	utility::error::BugFound(0x100);
	return false;
}

void bipl::parameters::judgement::JUDGEMENT::coutList(void)const
{
	std::cout << list_ << std::endl;
}

template <typename content_type>
bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<content_type>::JUDGEMENT_IS_IN_RANGE(content_type min, content_type max) :JUDGEMENT("x �� [" + std::to_string(min) + "," + std::to_string(max) +"]")
{
	min_ = min;
	max_ = max;
	if(min_ >= max_)
		utility::error::BugFound(0x101);
}

template bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<int>::JUDGEMENT_IS_IN_RANGE(int min, int max);
template bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<double>::JUDGEMENT_IS_IN_RANGE(double min, double max);
template bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<unsigned int>::JUDGEMENT_IS_IN_RANGE(unsigned int min, unsigned int max);

template <typename content_type>
bool bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<content_type>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	content_type parameter_content;
	parameters->ReadParameter(parameter_content, parameter_name, default_parameter_content);
	return (parameter_content >= min_ && parameter_content <= max_);
}

template bool bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<int>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const;
template bool bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<double>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const;
template bool bipl::parameters::judgement::JUDGEMENT_IS_IN_RANGE<unsigned int>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const;




template <typename content_type>
bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<content_type>::JUDGEMENT_IS_BIGGER_THAN(content_type min):JUDGEMENT("x > "+ std::to_string(min))
{
	min_ = min;
}

template bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<int>::JUDGEMENT_IS_BIGGER_THAN(int min);
template bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<double>::JUDGEMENT_IS_BIGGER_THAN(double min);
template bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int>::JUDGEMENT_IS_BIGGER_THAN(unsigned int min);

template <typename content_type>
bool bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<content_type>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	content_type parameter_content;
	parameters->ReadParameter(parameter_content, parameter_name, default_parameter_content);
	return (parameter_content >= min_);
}

template bool bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<int>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const;
template bool bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<double>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const;
template bool bipl::parameters::judgement::JUDGEMENT_IS_BIGGER_THAN<unsigned int>::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const;

bipl::parameters::judgement::JUDGEMENT_IS_RANGE::JUDGEMENT_IS_RANGE():JUDGEMENT("[min,max]")
{}

bool bipl::parameters::judgement::JUDGEMENT_IS_RANGE::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::vector<double> parameter_content;
	parameters->ReadParameter(parameter_content, parameter_name, default_parameter_content);
	if (parameter_content.size() != 2)
		return false;
	if (parameter_content[0] > parameter_content[1])
		return false;
	return true;
}

bipl::parameters::judgement::JUDGEMENT_FILE_EXIST::JUDGEMENT_FILE_EXIST():JUDGEMENT("Existing filename")
{}

bool bipl::parameters::judgement::JUDGEMENT_FILE_EXIST::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::string filename;
	parameters->ReadParameter(filename, parameter_name, default_parameter_content);
	std::ifstream testfile;
	testfile.open(filename);
	return testfile.is_open();
}

bipl::parameters::judgement::JUDGEMENT_STRING_LIST::JUDGEMENT_STRING_LIST(const std::vector<std::string> judgement_list):JUDGEMENT(judgement_list)
{
	if (judgement_list.size() <= 0)
		utility::error::BugFound(0x102);
	judgement_list_ = judgement_list;
}

bool bipl::parameters::judgement::JUDGEMENT_STRING_LIST::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::string parameter_content;
	parameters->ReadParameter(parameter_content, parameter_name, default_parameter_content);
	for (unsigned int i = 0; i < judgement_list_.size(); i++)
	{
		if (parameter_content == judgement_list_[i])
			return true;
	}
	return false;
}

bipl::parameters::judgement::JUDGEMENT_YN::JUDGEMENT_YN():JUDGEMENT("Y or N")
{}

bool bipl::parameters::judgement::JUDGEMENT_YN::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::string parameter_content;
	parameters->ReadParameter(parameter_content, parameter_name, default_parameter_content);
	return (parameter_content == "Y" || parameter_content == "N");
}

bipl::parameters::judgement::JUDGEMENT_VECTOR_SIZE::JUDGEMENT_VECTOR_SIZE(int vector_size):JUDGEMENT("A vector consisting of "+std::to_string(vector_size)+" elements")
{
	if (vector_size <= 0)
		utility::error::BugFound(0x103);
	vector_size_ = vector_size;
}

bool bipl::parameters::judgement::JUDGEMENT_VECTOR_SIZE::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::vector<std::string> parameter_content;
	parameters->ReadParameter(parameter_content, parameter_name, default_parameter_content);
	return (parameter_content.size() == vector_size_);
}

bool bipl::parameters::judgement::JUDGEMENT_RANDOMSEED::judgement(PARAMETERS *parameters, const std::string parameter_name, const std::string default_parameter_content)const
{
	std::string content_buffer;
	parameters->ReadParameter(content_buffer, parameter_name, default_parameter_content);
	if (content_buffer == "random_device")
		return true;
	else
	{
		if (content_buffer.front() != '[' || *(--content_buffer.end()) != ']')
			return false;
		content_buffer.erase(content_buffer.begin());
		content_buffer.erase(--content_buffer.end());
		std::vector<std::string> vector_buffer;
		lexial::Split(vector_buffer, content_buffer, ',');
		for (unsigned int i = 0; i < vector_buffer.size(); i++)
		{
			try {
				std::stoul(vector_buffer[i]);
			}
			catch (...)
			{
				return false;
			}
		}
	}
	return true;
}