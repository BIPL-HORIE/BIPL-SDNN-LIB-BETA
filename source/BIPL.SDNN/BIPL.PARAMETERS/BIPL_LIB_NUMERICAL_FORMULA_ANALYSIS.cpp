#include "BIPL_LIB_NUMERICAL_FORMULA_ANALYSIS.h"
#include "BIPL_LIB_LEXIAL.h"
#include "BIPL_LIB_UTILITY.h"
#include <iostream>

namespace bipl
{
	namespace parameters
	{
		namespace nfa
		{
			namespace base
			{
				//! 加減
				double AdditionAndSubtraction(std::string formula);
				//! 乗除
				double MultiplicationAndDivision(std::string formula);
				//! 接頭辞
				double Pulus_MinusSign(std::string formula);
				//! 累乗
				double Power(std::string formula);
				//! 括弧と数値
				double Factor(std::string formula);

				double AdditionAndSubtraction(std::string formula)
				{
					int parentheses_number = 0;

					for (std::string::iterator formula_iterator = (formula.size() != 0)?--formula.end():formula.begin(); formula_iterator != formula.begin(); --formula_iterator)
					{
						//()の対応
						if (*formula_iterator == ')')
							parentheses_number++;
						else if (*formula_iterator == '(')
						{
							parentheses_number--;
							if (parentheses_number < 0)
							{
								throw "num of ( != num of )";
							}
						}
						//()が無い場合だけ反応
						else if (parentheses_number == 0)
						{
							if (*(formula_iterator - 1) == '*' || *(formula_iterator - 1) == '/' || *(formula_iterator - 1) == '^');
							else if (*formula_iterator == '+' || *formula_iterator == '-')
							{
								if (formula_iterator == --formula.end())
									throw "not binary +-";

								return (*formula_iterator == '+') ? AdditionAndSubtraction(formula.substr(0, formula_iterator - formula.begin())) + MultiplicationAndDivision(formula.substr(formula_iterator - formula.begin() + 1)) :
									AdditionAndSubtraction(formula.substr(0, formula_iterator - formula.begin())) - MultiplicationAndDivision(formula.substr(formula_iterator - formula.begin() + 1));
							}
						}
					}
					return MultiplicationAndDivision(formula);
				}
				double MultiplicationAndDivision(std::string formula)
				{
					int parentheses_number = 0;
					for (std::string::iterator formula_iterator = (formula.size() != 0) ? --formula.end() : formula.begin(); formula_iterator != formula.begin(); --formula_iterator)
					{
						//()の対応
						if (*formula_iterator == ')')
							parentheses_number++;
						else if (*formula_iterator == '(')
						{
							parentheses_number--;
							if (parentheses_number < 0)
							{
								throw "num of ( != num of )";
							}
						}
						//()が無い場合だけ反応
						else if (parentheses_number == 0)
						{
							if (*formula_iterator == '*' || *formula_iterator == '/')
							{
								if (formula_iterator == formula.begin() || formula_iterator == --formula.end())
									throw "not binary *,/";

								if (*formula_iterator == '*')
									return MultiplicationAndDivision(formula.substr(0, formula_iterator-formula.begin()))*Pulus_MinusSign(formula.substr(formula_iterator - formula.begin() + 1));
								else
								{
									double backward_result = Pulus_MinusSign(formula.substr(formula_iterator - formula.begin() + 1));
									if (backward_result == 0)//0割対策
									{
										throw "division by zero";
									}
									return MultiplicationAndDivision(formula.substr(0, formula_iterator - formula.begin())) / backward_result;
								}
							}
						}
					}
					return Pulus_MinusSign(formula);
				}
				double Pulus_MinusSign(std::string formula)
				{
					if (*(formula.begin()) == '-')
					{
						formula.erase(formula.begin()); //-をカット；
						return -Power(formula);
					}
					if (*(formula.begin()) == '+')
					{
						formula.erase(formula.begin()); //+をカット；
					}
					return Power(formula);
				}
				double Power(std::string formula)
				{
					int parentheses_number = 0;

					for (std::string::iterator formula_iterator = formula.begin(); formula_iterator != formula.end(); ++formula_iterator)
					{
						//()の対応
						if (*formula_iterator == '(')
							parentheses_number++;
						else if (*formula_iterator == ')')
						{
							parentheses_number--;
							if (parentheses_number < 0)
							{
								throw "num of ( != num of )";
							}
						}
						//()が無い場合だけ反応
						else if (parentheses_number == 0)
						{
							if (*formula_iterator == '^')
							{
								if (formula_iterator == formula.begin() || formula_iterator == --formula.end())
									throw "not binary ^";

								return pow(Factor(formula.substr(0, formula_iterator-formula.begin())), Power(formula.substr(formula_iterator-formula.begin()+1)));
							}
						}
					}
					return Factor(formula);
				}

				double Factor(std::string formula)
				{
					if (*(formula.begin()) == '(')//（）の場合
					{
						if (*(--formula.end())!= ')')
						{
							throw "num of ( != num of )";
						}
						//最初の(と最後の)を削除してequationに投げる．
						formula.erase(--formula.end());
						formula.erase(formula.begin());

						return AdditionAndSubtraction(formula);
					}
					else if (*(formula.begin()) == '[')
						throw "it is vector, not formula";
					else
					{
						if (lexial::CountCharactor(formula, ".") > 1)
						{
							throw "too many .";
						}
						double result;
						try
						{
							result = std::stod(formula);
						}
						catch (...)
						{
							throw "syntactic error";
						}
						return result;
					}
				}
			}
		}
	}
}

double bipl::parameters::nfa::NumericalFormulaAnalysis(const std::string &formula)
{
	std::string formula_buffer = formula;
	bipl::lexial::DeleteCharactor(formula_buffer, " \t");
	double result = 0;
	if (formula == "") //念のため，基本的には手前で空チェックはしている．
		throw "empty line";
	return result = base::AdditionAndSubtraction(formula_buffer);
}