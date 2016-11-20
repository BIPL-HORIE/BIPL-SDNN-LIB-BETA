#pragma once
#include <vector>
#include <string>
#include <random>

namespace bipl
{
	namespace sdnn
	{
		namespace base
		{
			namespace pc
			{
				class SYMBOL_NODE;

				class SYMBOL_ROOT
				{
				private:
					std::vector<SYMBOL_NODE> down_;
					int n_;
					void AddSymbol(std::vector<std::string>tree_line, std::mt19937 &mt);
				public:
					SYMBOL_ROOT(int n) { n_ = n; }
					~SYMBOL_ROOT() {}
					void Load(const std::string &filename, std::mt19937 &mt);
					void PrintSymbol(void);
					void GetPatternList(std::vector<std::vector<bool>> &out_list);
				};

				class SYMBOL_NODE
				{
				private:
					std::string node_name_;
					std::vector<bool> pattern_;
					std::vector<SYMBOL_NODE> down_;
				public:
					SYMBOL_NODE(const std::string &name, const int n, std::mt19937 &mt);
					SYMBOL_NODE(const std::string &name, const std::vector<bool> &material, const double correspond, std::mt19937 &mt);
					~SYMBOL_NODE() {}

					void AddSymbol(std::vector<std::string>tree_line, std::mt19937 &mt);
					void PrintSymbol(const int tagnum);
					std::string &GetName(void) { return node_name_; };
					std::vector<bool> &GetPattern(void) { return pattern_; };
					void GetPatternList(std::vector<std::vector<bool>> &out_list);
				};
			}
		}
	}
}