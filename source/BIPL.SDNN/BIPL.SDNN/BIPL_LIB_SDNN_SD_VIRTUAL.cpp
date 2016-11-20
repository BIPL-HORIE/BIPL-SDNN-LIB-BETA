#include "BIPL_LIB_SDNN_SD_VIRTUAL.h"

#include "BIPL_LIB_PARAMETERS.h"
#include "BIPL_LIB_LEXIAL.h"
#include "BIPL_LIB_UTILITY.h"


bipl::sdnn::base::SDNN_SD_VIRTUAL::SDNN_SD_VIRTUAL() :input_number_(0), desensitize_number_(0),desensitized_pattern_neuron_number_(0)
{
	SDNN_SD_VIRTUAL::SelectiveDesensitizationPointer = (&SDNN_SD_VIRTUAL::SelectiveDesensitizationAstarisk);
}

bipl::sdnn::base::SDNN_SD_VIRTUAL::~SDNN_SD_VIRTUAL()
{}

void bipl::sdnn::base::SDNN_SD_VIRTUAL::SelectiveDesensitizationAstarisk(int *out_adress_data_list, const double *input)
{
	bipl::utility::error::BugFound(0x1ff08);
}

void bipl::sdnn::base::SDNN_SD_VIRTUAL::InitSD(parameters::PARAMETERS &sdnn_parameter)
{
	bipl::utility::error::BugFound(0x1ff08);
}

int bipl::sdnn::base::SDNN_SD_VIRTUAL::GetSDPatternNeuronNumber(void)
{
	return desensitized_pattern_neuron_number_;
}

std::string bipl::sdnn::base::SDNN_SD_VIRTUAL::GetDesensitizationList(void)
{
	bipl::utility::error::BugFound(0x1ff08);
	return "";
}
