/*! @file
@brief 数値，数式解析

@code
BNF記法
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
			/*! @brief 数値，数式解析
			@param[in] formula 解析したい数式
			@return 数式を解析した結果，得られた数値
			@author Kazumasa HORIE
			@version β1.0
			*/
			double NumericalFormulaAnalysis(const std::string &formula);
		}
	}
}
