/*! @file BIPL_LIB_PARAMETERS.h
@brief �Ǝ����p�����[�^�̃��[�h�Əo�͂Ɋւ���N���X

�I�[�v���\�[�X���̐���ɂ�炸�C�\�ʏ�͍ŏ��\���ɂ��邱�Ƃɂ����DPointer2implementation�𗘗p�D
���̕��������Ń��C�u���������Ă��C����h�t�@�C���ȊO���C���N���[�h����K�v�͖����͂�

@note ���̃t�@�C���̓I�[�v���\�[�X�łȂ��ꍇ�C���J����Ȃ��D�O���錾������
@author Kazumasa HORIE
@date 2015/12/1~
*/
#pragma once

#define HARDWARE_ENTROPY "hardware_entropy"

#include <memory>
#include <string>
#include <vector>
namespace bipl
{
	namespace parameters
	{
		namespace judgement
		{
			//! ���������܂񂾃N���X�̑O���錾
			class JUDGEMENT;
		}
		/*!
		@brief �Ǝ����p�����[�^
		@code
		�����p��P��
		bipl::parameters::PARAMETERS test_parameter;
		test_parameter.LoadFile("setting.txt");
		int parameter_input_number;
		test_parameter.ReadParameter(parameter_input_number,"SDNN\\input_number","2");
		std::cout << parameter_input_number << std::endl;
		@endcode
		@code
		�����p��Q��
		bipl::parameters::PARAMETERS test_parameter;
		test_parameter.LoadFile("setting.txt");
		std::vector<double> output_range;
		test_parameter.ReadParameter(output_range,"SDNN\\NN\\output_range","[-0.2,1.2]");
		if(output_range.size() >= 2)
			std::cout << output_range[0] << "," << output_range[1] << std::endl;
		@endcode
		@note �ŏ��\���ɂ����̂ŁC���p����ɂ͂Ȃ�Ȃ��D�D�D�D�͂��I
		@author Kazumasa HORIE
		@date 2015/12/1~
		@version ��1.0�i12/6�F�u���b�N�{�b�N�X�e�X�g�I���j
		*/
		class PARAMETERS
		{
		private:
			//! Pointer2Implementation�@�N���X��`
			class PARAMETERS_ROOT;
			//! Pointer2Implementation�@�|�C���^�{�́@���@�p�����[�^�؂̍�����
			std::unique_ptr<PARAMETERS_ROOT> pimpl_;

			//! �R�s�[�R���X�g���N�^�͋֎~
			PARAMETERS(const PARAMETERS&) = delete;
			//! ������Z���֎~
			void operator=(const PARAMETERS&) = delete;

		public:
			//! �R���X�g���N�^�C���g��PIMPL�̎��̉�
			PARAMETERS(void);
			//! �f�X�g���N�^�C���g��
			~PARAMETERS(void);

			/*! @brief �p�����[�^�t�@�C������p�����[�^�����������Ă��ēo�^����
			@param[in] parameter_file_name �p�����[�^�������ꂽ�t�@�C���̖��O
			@return �Ȃ��D
			*/
			void LoadFile(const std::string parameter_file_name);

			/*! @brief BPL�̏����ꂽstringstream����p�����[�^�����������Ă��ēo�^����
			@param[in] parameter_file_name �p�����[�^�������ꂽstringstream
			@return �Ȃ��D
			*/
			void LoadFile(std::stringstream &parameter_stream);

			/*! @brief �p�����[�^�̓o�^�{�㏑��
			�p�����[�^�����݂��Ă����ꍇ�͏㏑���C�Ȃ������ꍇ�͐V���ɓo�^����D
			�R�[�h�̎d�l��C�����ꍇ�o�^����`�ɂȂ������C�����P���ȃp�����[�^�o�^�Ɏg�����Ƃ͔񐄏��D
			random_seed = hardware_entropy�̎��ɁChardware_entropy��p���ăV�[�h���쐬�D
			�쐬�����V�[�h�l��random_seed���㏑���������ꍇ���ɗp���ė~�����D
			@param[in] parameter_name �p�����[�^��
			@param[in] parameter_content �p�����[�^�̓��e
			@return �Ȃ��D
			*/
			void OverwriteParameter(const std::string parameter_name, const std::string &parameter_content);

			/*! @brief �p�����[�^�̓o�^�{�㏑��
			content�Ƀx�N�^�[�`�������������́D
			�x�N�^�[�`����o�^���@�ɒ����̖ʓ|�����炱�ꂾ���͒ǉ��D
			���ʂ�int�Ƃ���std::string���邾�������炢�����ȁ[�D�D�Ƃ��v���Ă�D
			@param[in] parameter_name �p�����[�^��
			@param[in] parameter_content �p�����[�^�̓��e(int double unsigned int std::string�Ή�)
			@return �Ȃ��D
			*/
			template<typename content_type>
			void OverwriteParameter(const std::string parameter_name, const std::vector<content_type> parameter_content);

			/*! @brief �p�����[�^��ǂ�,�����ɍ���Ȃ������ꍇ�R���\�[������ē��͂��󂯂�
			@param[out] out_parameter_content �p�����[�^�o�́iint, double, unsigned int, std::string�j
			@param[in] parameter_name �p�����[�^��
			@param[in] default_parameter_content �f�t�H���g�o��
			@param[in] judgement ���莮���܂񂾃N���X�̃A�h���X nullptr�Ȃ���������Ɣ��f����i���[�U�[��`�\�j
			@return �Ȃ��D�p�����[�^�o�͂ɂ��邩�Y�񂾂��C�z���Ԃ��ق���ReadParameter�Əo�͂��ς���Ă��܂����߂�߂��D
			*/
			template<typename content_type>
			void ReadParameter(content_type &out_parameter_content, const std::string parameter_name, const std::string default_parameter_content, judgement::JUDGEMENT *judgement = nullptr);

			/*! @brief �p�����[�^�z���ǂ�,�����ɍ���Ȃ������ꍇ�R���\�[������ē��͂��󂯂�
			@param[out] out_parameter_content �p�����[�^�o�́ivector(int), (double), (unsigned int), (std::string)�j
			@param[in] parameter_name �p�����[�^��
			@param[in] default_parameter_content �f�t�H���g�o��
			@param[in] judgement ���莮���܂񂾃N���X nullptr�Ȃ���������Ɣ��f����i���[�U�[��`�\�j
			@return �Ȃ��Dvector�Ȃ̂œK���ɂ��D
			*/
			template<typename content_type>
			void ReadParameter(std::vector<content_type> &out_parameter_content, const std::string parameter_name, const std::string default_parameter_content, judgement::JUDGEMENT *judgement = nullptr);


			/*! @brief �p�����[�^�̃��X�g�𓾂�
			@param[out] out_parameter_list �o�̓p�����[�^���X�g
			@warning ������out_parameter_list�������������D�ǋL�͕s�D
			@return �Ȃ��D
			*/
			void ListParameter(std::string &out_parameter_list)const;

			//! �p�����[�^�̃��X�g��ostream�ɓ˂�����
			friend std::ostream& operator<<(std::ostream& os, const PARAMETERS& parameter)
			{
				std::string list;
				parameter.ListParameter(list);
				os << list;
				return os;
			}
		};
	}
}