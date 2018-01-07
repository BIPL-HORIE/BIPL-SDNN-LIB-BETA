/*! @file
@brief �Ǝ����p�����[�^�̃^�O���p�����[�^�ƃ^�O�Ȃ��p�����[�^

�Ǝ����p�����[�^�̊�b�����D��������J�i�̂���j
�^�O�����̏����Ȗ��O�|�p�����[�^��FILE�ƁC�^�O�Ǘ���FOLDER�ɕ������D
���͍̂�҂��o���₷����������Dcontent�Ƃ����O�t����ƃR���^�~����̂ŁD�D

�����̃N���X�͌����Ƃ���PARAMETERS������PIMLP�ł̂݌Ăяo�����D

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
			@brief �Ǝ����p�����[�^�̍ŏ����i�C���O�Ɠ��e�̃Z�b�g
			�؍\���̗t�����ɂ�����
			@author Kazumasa HORIE
			*/
			class PARAMETER_LEAF
			{
			private:
				//! �p�����[�^��
				std::string parameter_name_;
				//! �p�����[�^���e
				std::string parameter_content_;
			public:
				PARAMETER_LEAF(const std::string parameter_name, const std::string parameter_content):
				parameter_name_(parameter_name), parameter_content_(parameter_content){}
				~PARAMETER_LEAF() {}

				/*! @brief ���O�̈�v���o
				*/
				bool IsSameName(const std::string& parameter_name)const { return (parameter_name_ == parameter_name); }

				/*! @brief ���g�̏o��
				*/
				std::string GetContent()const { return parameter_content_; }

				/*! @brief ���g�̏㏑��
				*/
				void Overwrite(const std::string &parameter_content) {
					if(parameter_content.size () < 300)
						std::cout << "overwrite parameter: " << parameter_name_ << " = " << parameter_content << std::endl;				
					parameter_content_ = parameter_content;
				}

				/*! @brief �p�����[�^�̃��X�g��Ԃ�
				@param[out] out_parameter_list �o�̓p�����[�^���X�g
				@param[in] tab_number �C���f���g�̐�
				@return �Ȃ���
				*/
				void ListParameter(std::string &out_parameter_list, const int tag_number)const {
					for (int i = 0; i < tag_number; i++)
						out_parameter_list += "    ";
					out_parameter_list += parameter_name_ + "=" + parameter_content_ + "\n";
				}
			};

			/*!
			@brief�@�Ǝ����p�����[�^�̃^�O�����C�؍\���̃m�[�h�ɓ�����
			@author Kazumasa HORIE
			*/
			class PARAMETER_NODE
			{
			private:
				//! �^�O��
				std::string node_name_;
				//! �^�O���̃p�����[�^���X�g
				std::vector<PARAMETER_LEAF> leafs_;
				//! �^�O���̃^�O���X�g
				std::vector<PARAMETER_NODE> down_nodes_;

			public:
				PARAMETER_NODE(std::string node_name): node_name_(node_name) {}
				~PARAMETER_NODE() {}

				/*! @brief ���O�̈�v���o
				*/
				bool IsSameName(const std::string &node_name) { return (node_name_ == node_name); }

				/*! @brief �p�����[�^�̒ǉ�
				@param[in] tag_list �^�O�̃��X�g
				@param[in] parameter_name �p�����[�^�̖��O
				@param[in] parameter_content �p�����[�^�̓��e
				@param[in] mode �p�����[�^�̏㏑���̉ہC�ʏ��
				@return �Ȃ��D�G���[��������throw�C�A���P�[�V�����ƃI�[�o�[���[�h�ŃG���[�͂�
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
						//���������ꍇ�Cleafs�ɒǉ��D
						PARAMETER_LEAF new_leaf(parameter_name, parameter_content);
						leafs_.push_back(new_leaf);
						return;
					}
					else
					{
						tag_list.erase(tag_list.begin());//��ڂ��폜
						for (auto down_node_itr = down_nodes_.begin(); down_node_itr != down_nodes_.end(); down_node_itr++)
						{
							if (down_node_itr->IsSameName(tag_list.front()))
							{
								down_node_itr->AddParameter(tag_list, parameter_name, parameter_content);
								return;
							}
						}
						//���������ꍇ�Cnodes�ɒǉ�
						PARAMETER_NODE new_node(tag_list.front());
						down_nodes_.push_back(new_node);
						(--down_nodes_.end())->AddParameter(tag_list, parameter_name, parameter_content);
						return;
					}
					return;
				}

				/*! @brief �p�����[�^�̒��g��Ԃ�
				@param[in] tag_list �^�O�̃��X�g
				@param[in] parameter_name �ǂ݂����p�����[�^�̖��O
				@return �p�����[�^�̓��e������string�̃|�C���^�C������Ȃ�������nullptr
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
						//���������ꍇ�ǉ�
						PARAMETER_LEAF new_leaf(parameter_name, default_content);
						leafs_.push_back(new_leaf);
						std::cout << "add parameter: " << parameter_name << " = " << default_content << std::endl;
						return leafs_.back().GetContent();
					}
					else
					{
						tag_list.erase(tag_list.begin());//��ڂ��폜
						for (auto down_node_itr = down_nodes_.begin(); down_node_itr != down_nodes_.end(); down_node_itr++)
						{
							if (down_node_itr->IsSameName(tag_list.front()))
							{
								return down_node_itr->ReadParameter(tag_list, parameter_name, default_content);
							}
						}
						//���������ꍇ�Cnodes�ɒǉ�
						PARAMETER_NODE new_node(tag_list.front());
						down_nodes_.push_back(new_node);
						std::cout << "add tag: " << tag_list.front() << std::endl;
						return down_nodes_.back().ReadParameter(tag_list,parameter_name,default_content);
					}
				}

				/*! @brief �p�����[�^�̃��X�g��Ԃ�
				@param[out] out_parameter_list �o�̓p�����[�^���X�g
				@param[in] tab_number �C���f���g�̐�
				@return �Ȃ���
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
