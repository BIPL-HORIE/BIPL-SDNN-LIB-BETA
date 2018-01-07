#include "BIPL_LIB_SDNN_FACTORY.h"

#include "BIPL_LIB_PARAMETERS.h"
#include "BIPL_LIB_SDNN_PARAMETERS.h"
#include "BIPL_LIB_PARAMETERS_JUDGEMENT.h"
#include "BIPL_LIB_UTILITY.h"

#include "BIPL_LIB_SDNN_NN_VIRTUAL.h"
#include "BIPL_LIB_SDNN_NN_SP.h"
#include "BIPL_LIB_SDNN_NN_PP.h"

#include "BIPL_LIB_SDNN_SD_VIRTUAL.h"
#include "BIPL_LIB_SDNN_SD_SINGLE.h"

#include <string>
#include <iostream>

void bipl::sdnn::base::FactorySD(std::unique_ptr<SDNN_SD_VIRTUAL> &out_sd_pointer, parameters::PARAMETERS &sdnn_parameter)
{
	try
	{
		out_sd_pointer = std::make_unique<sd::SDNN_SD_SINGLE>();
	}
	catch (std::bad_alloc)
	{
		std::cout << "ƒƒ‚ƒŠ‚ÌŠm•Û‚ÉŽ¸”s‚µ‚Ü‚µ‚½" << std::endl;
		bipl::utility::Wait();
		exit(1);
	}
}

void bipl::sdnn::base::FactoryNN(std::unique_ptr<SDNN_NN_VIRTUAL> &out_nn_pointer, parameters::PARAMETERS &sdnn_parameter)
{
	std::string issue_type;
	parameter_property::PARAM_SDNN_TYPE::property_.Read(issue_type, sdnn_parameter);

	try {
		if (issue_type == parameter_property::PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::function_approximation_)
		{
			out_nn_pointer = std::make_unique<nn::SDNN_NN_PP>();
		}
		else if (issue_type == parameter_property::PARAM_SDNN_TYPE::CNT_SDNN_TYPE_::pattern_recognition_)
		{
			std::string multi_class_recognition_method;
			parameter_property::PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::property_.Read(multi_class_recognition_method, sdnn_parameter);

			if (multi_class_recognition_method == parameter_property::PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovr_)
			{
				out_nn_pointer = std::make_unique<nn::SDNN_NN_SP_1vR>();
			}
			else if (multi_class_recognition_method == parameter_property::PARAM_NN_SP_MULTI_CLASS_RECOGNITION_METHOD::CNT_NN_SP_MULTI_CLASS_RECOGNITION_METHOD_::ovo_)
			{
				out_nn_pointer = std::make_unique<nn::SDNN_NN_SP_1v1>();
			}
			else
			{
				utility::error::BugFound(0x1ff01);
			}
		}
		else
		{
			utility::error::BugFound(0x1ff02);
		}
	}
	catch (std::bad_alloc)
	{
		std::cout << "ƒƒ‚ƒŠ‚ÌŠm•Û‚ÉŽ¸”s‚µ‚Ü‚µ‚½" << std::endl;
		bipl::utility::Wait();
		exit(1);
	}
}