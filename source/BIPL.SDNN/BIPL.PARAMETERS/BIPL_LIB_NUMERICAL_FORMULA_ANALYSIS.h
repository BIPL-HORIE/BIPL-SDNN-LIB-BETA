/*! @file
@brief ���l�C�������

@code
BNF�L�@
//AdditionAndSubtraction :: = <MultiplicationAndDivision> | {<MultiplicationAndDivision>{+|-}{<AdditionAndSubtraction}>}
AdditionAndSubtraction :: = <MultiplicationAndDivision> | {<AdditionAndSubtraction>{+|-}<MultiplicationAndDivision>}
MultiplicationAndDivision :: = <Pulus_MinusSign> | {<Pulus_MinusSign> {*|/} <MultiplicationAndDivision>}
Pulus_MinusSign :: = {+|-| } <Power>
Power	 :: = <Factor> | {<Power> {^} <Factor>}
Factor	 :: = <Number> | (<AdditionAndSubtraction>)
@endcode

@author Kazumasa HORIE
@date 2015/11/30~
*/

#pragma once
#include <string>


namespace bipl
{
	namespace parameters
	{
		namespace nfa
		{
			/*! @brief ���l�C�������
			@param[in] formula ��͂���������
			@return ��������͂������ʁC����ꂽ���l
			@author Kazumasa HORIE
			@version ��1.0
			*/
			double NumericalFormulaAnalysis(const std::string &formula);
		}
	}
}
