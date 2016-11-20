/*! @file
@brief 独自企画パラメータのタグつきパラメータとタグなしパラメータ

独自企画パラメータの基礎部分．原則非公開（のつもり）
タグ無しの純粋な名前－パラメータのFILEと，タグ管理のFOLDERに分かれる．
名称は作者が覚えやすかったから．contentとか名前付けるとコンタミするので．．

これらのクラスは原則としてPARAMETERSかそのPIMLPでのみ呼び出される．

@author Kazumasa HORIE
@date 2015/12/1~
*/

#pragma once
#include "BIPL_LIB_PARAMETERS.h"
#include <vector>
#include "BIPL_LIB_UTILITY.h"
#include<iostream>

namespace bipl
{
	namespace parameters
	{
		namespace base
		{
			/*!
			@brief 独自企画パラメータの最小部品，名前と内容のセット
			木構造の葉部分にあたる
			@author Kazumasa HORIE
			*/
			class PARAMETER_LEAF
			{
			private:
				//! パラメータ名
				std::string parameter_name_;
				//! パラメータ内容
				std::string parameter_content_;
			public:
				PARAMETER_LEAF(const std::string parameter_name, const std::string parameter_content):
				parameter_name_(parameter_name), parameter_content_(parameter_content){}
				~PARAMETER_LEAF() {}

				/*! @brief 名前の一致検出
				*/
				bool IsSameName(const std::string& parameter_name)const { return (parameter_name_ == parameter_name); }

				/*! @brief 中身の出力
				*/
				std::string GetContent()const { return parameter_content_; }

				/*! @brief 中身の上書き
				*/
				void Overwrite(const std::string &parameter_content) {
					std::cout << "overwrite parameter: " << parameter_name_ << " = " << parameter_content << std::endl;				
					parameter_content_ = parameter_content;
				}

				/*! @brief パラメータのリストを返す
				@param[out] out_parameter_list 出力パラメータリスト
				@param[in] tab_number インデントの数
				@return ないで
				*/
				void ListParameter(std::string &out_parameter_list, const int tag_number)const {
					for (int i = 0; i < tag_number; i++)
						out_parameter_list += "    ";
					out_parameter_list += parameter_name_ + "=" + parameter_content_ + "\n";
				}
			};

			/*!
			@brief　独自企画パラメータのタグ部分，木構造のノードに当たる
			@author Kazumasa HORIE
			*/
			class PARAMETER_NODE
			{
			private:
				//! タグ名
				std::string node_name_;
				//! タグ内のパラメータリスト
				std::vector<PARAMETER_LEAF> leafs_;
				//! タグ内のタグリスト
				std::vector<PARAMETER_NODE> down_nodes_;

			public:
				PARAMETER_NODE(std::string node_name): node_name_(node_name) {}
				~PARAMETER_NODE() {}

				/*! @brief 名前の一致検出
				*/
				bool IsSameName(const std::string &node_name) { return (node_name_ == node_name); }

				/*! @brief パラメータの追加
				@param[in] tag_list タグのリスト
				@param[in] parameter_name パラメータの名前
				@param[in] parameter_content パラメータの内容
				@param[in] mode パラメータの上書きの可否，通常非
				@return なし．エラー発生時はthrow，アロケーションとオーバーロードでエラーはく
				*/
				void AddParameter(std::vector<std::string> tag_list, const std::string &parameter_name, const std::string &parameter_content)
				{
					if (tag_list.front() != node_name_)
					{
						std::cout << tag_list.front() <<"," << node_name_<< std::endl;
						utility::error::BugFound(0x2f00ff);
					}
					if (tag_list.size() == 1)
					{
						for (auto leaf_itr = leafs_.begin(); leaf_itr != leafs_.end(); leaf_itr++)
						{
							if (leaf_itr->IsSameName(parameter_name))
							{
								leaf_itr->Overwrite(parameter_content);
								return;
							}
						}
						//無かった場合，leafsに追加．
						PARAMETER_LEAF new_leaf(parameter_name, parameter_content);
						leafs_.push_back(new_leaf);
						return;
					}
					else
					{
						tag_list.erase(tag_list.begin());//一個目を削除
						for (auto down_node_itr = down_nodes_.begin(); down_node_itr != down_nodes_.end(); down_node_itr++)
						{
							if (down_node_itr->IsSameName(tag_list.front()))
							{
								down_node_itr->AddParameter(tag_list, parameter_name, parameter_content);
								return;
							}
						}
						//無かった場合，nodesに追加
						PARAMETER_NODE new_node(tag_list.front());
						down_nodes_.push_back(new_node);
						(--down_nodes_.end())->AddParameter(tag_list, parameter_name, parameter_content);
						return;
					}
					return;
				}

				/*! @brief パラメータの中身を返す
				@param[in] tag_list タグのリスト
				@param[in] parameter_name 読みたいパラメータの名前
				@return パラメータの内容を持つstringのポインタ，見つからなかったらnullptr
				*/
				std::string ReadParameter(std::vector<std::string> &tag_list, const std::string &parameter_name, const std::string &default_content)
				{
					if (tag_list.front() != node_name_)
					{
						utility::error::BugFound(0x2f00fe);
					}
					if (tag_list.size() == 1)
					{
						for (auto leaf_itr = leafs_.begin(); leaf_itr != leafs_.end(); leaf_itr++)
						{
							if (leaf_itr->IsSameName(parameter_name))
							{
								return leaf_itr->GetContent();
							}
						}
						//無かった場合追加
						PARAMETER_LEAF new_leaf(parameter_name, default_content);
						leafs_.push_back(new_leaf);
						std::cout << "add parameter: " << parameter_name << " = " << default_content << std::endl;
						return leafs_.back().GetContent();
					}
					else
					{
						tag_list.erase(tag_list.begin());//一個目を削除
						for (auto down_node_itr = down_nodes_.begin(); down_node_itr != down_nodes_.end(); down_node_itr++)
						{
							if (down_node_itr->IsSameName(tag_list.front()))
							{
								return down_node_itr->ReadParameter(tag_list, parameter_name, default_content);
							}
						}
						//無かった場合，nodesに追加
						PARAMETER_NODE new_node(tag_list.front());
						down_nodes_.push_back(new_node);
						std::cout << "add tag: " << tag_list.front() << std::endl;
						return down_nodes_.back().ReadParameter(tag_list,parameter_name,default_content);
					}
				}

				/*! @brief パラメータのリストを返す
				@param[out] out_parameter_list 出力パラメータリスト
				@param[in] tab_number インデントの数
				@return ないで
				*/
				void ListParameter(std::string &out_parameter_list, const int tab_number)const
				{
					for (int i = 0; i < tab_number; i++)
						out_parameter_list += "    ";
					out_parameter_list += "<" + node_name_ + ">\n";
					for (int i = 0; i < tab_number; i++)
						out_parameter_list += "    ";
					out_parameter_list += "{\n";

					for (auto leaf_itr = leafs_.begin(); leaf_itr != leafs_.end(); leaf_itr++)
					{
						leaf_itr->ListParameter(out_parameter_list, tab_number + 1);
					}
					for (auto down_node_itr = down_nodes_.begin(); down_node_itr != down_nodes_.end(); down_node_itr++)
					{
						down_node_itr->ListParameter(out_parameter_list, tab_number + 1);
					}
					for (int i = 0; i < tab_number; i++)
						out_parameter_list += "    ";
					out_parameter_list += "}\n";
				}
			};
		}
	}
}
