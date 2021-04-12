#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <io.h>

#include "type_conversion.h"

namespace basic_namespace
{
	using std::stringstream;
	using std::string;
	using std::vector;

	using std::cin;
	using std::cout;
	using std::endl;
}

namespace rename_mode
{
	using namespace basic_namespace;

	//ģʽ1��ֱ���޸����ƣ�ֻ����ǰ����ļ�·���Ĳ��֣���insert_context�����ò������޸�֮�������
	string edit_name1(string old_name, string old_suffix, string new_suffix, string insert_context)
	{
		for (int i = old_name.length() - 1, j = old_suffix.length() - 1;
			i > -1 && j > -1; --i, --j)
		{
			if (old_name[i] != old_suffix[j])
				return "";//����ṩ����ȷ�ĺ�׺�������ؿմ�
		}

		if ('.' != old_name[old_name.length() - old_suffix.length() - 1])
			return "";//���û���ҵ���ţ�ֱ�ӷ��ؿմ�

		int signal = old_name.length() - 1;
		while (signal > -1)//Ѱ�ҵ�ַ��ʵ������֮��Ľ���
		if (old_name[--signal] == '\\')
			break;

		//substr�ĵ�һ�����������±����ֵ���ڶ��������ǿ������ַ������������յ�ֵ
		string postfix = old_name.substr(0, signal + 1);//��ȡ�ļ����ĵ�ַ����

		old_name = postfix + insert_context + "." + new_suffix;

		return old_name;
	}

	//�ú�����ɾ�����ò��ֵ�ǰuseful_location���ַ���insert_context����Ҫ�����ò���ǰ��Ҫ���������
	string edit_name2(string old_name, string old_suffix, string new_suffix, string insert_context, int useful_location)
	{//useful_locationΪ����λ�ô��������±�
		for (int i = old_name.length() - 1, j = old_suffix.length() - 1;
			i > -1 && j > -1; --i, --j)
		{
			if (old_name[i] != old_suffix[j])
				return "";//����ṩ����ȷ�ĺ�׺�������ؿմ�
		}

		if ('.' != old_name[old_name.length() - old_suffix.length() - 1])
			return "";//���û���ҵ���ţ�ֱ�ӷ��ؿմ�

		int kk = 0;

		int signal = old_name.length() - 1;
		while (signal > -1)//Ѱ�ҵ�ַ��ʵ������֮��Ľ���
		if (old_name[--signal] == '\\')
			break;

		int actual_content_num = old_name.length() - old_suffix.length() - 1;//�ļ����г���׺�����֮��ʵ�����ݵ��ַ���

		//substr�ĵ�һ�����������±����ֵ���ڶ��������ǿ������ַ������������յ�ֵ
		string postfix = old_name.substr(0, signal + 1);//��ȡ�ļ����ĵ�ַ����

		//��ȡ�ļ��������ò��ֵ�����
		string real_content = old_name.substr(signal + 1 + useful_location, actual_content_num - (signal + 1 + useful_location));

		old_name = postfix + insert_context + real_content + "." + new_suffix;

		return old_name;
	}

}

namespace file_rename
{
	using namespace basic_namespace;
	using namespace rename_mode;
	
	void getFiles(string path, vector<string>& files)//�˺�����Դ�����ϣ�����δ��ɾ��
	{
		//�ļ����
		intptr_t   hFile = 0;

		//�ļ���Ϣ
		_finddata_t fileinfo;
		string temp;
		if ((hFile = _findfirst(temp.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				//�����Ŀ¼,����֮
				//�������,�����б�
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(temp.assign(path).append("\\").append(fileinfo.name), files);
				}//�˴����û�д����ţ��������������������if�������else֮��Ĺ�ϵ
				else//�������������ʹ���˴����ţ�����Ҳʹ�ô����ź���
				{
					files.push_back(temp.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	//�˺������ڼ򵥵������޸ģ��ڸ��������ƺ���ӵ����ı�š�begin_order����ʼ��ţ�Ĭ��Ϊ1
	bool modify_name1(string file_path, string old_suffix, string new_suffix, string insert_context, int begin_order = 1)
	{//�˺���Ҫ�� edit_name1���ʹ��
		vector<string> files;
		getFiles(file_path.data(), files);
		for (int order = 0, file_num = files.size(); order < file_num; ++order)
		{
			/*����Ϊ�˺���ʹ��TypeConversion���intתstring�����������Ķ���
			���ǵ�C++�еĳ����Թ�����ò�Ҫ�����������������*/
			TypeConversion temp;
			string new_name = edit_name1(files[order], old_suffix, new_suffix,
				insert_context + temp.int2string(begin_order + order));//��ð�Ҫ��༭�õ�����

			if (0 == new_name.length())
			{
				cout << "�����Ʋ��Ϸ���" << endl << new_name << endl;
				continue;
			}
				
			if (rename(files[order].data(), new_name.data()))
				cout << "ʧ�ܣ�" << endl << files[order] << endl;
			else
				cout << "�ɹ���" << endl << files[order] << endl;

		}

		return true;//�˺����ķ���ֵ��δͶ��ʹ�ã�����ֱ�����ó� true
	}

	//�˺������������е��ļ�����ɾ�����ò��ֵ�ǰuseful_location���ַ���
	//�������ò��ֵ�ǰ�˲���һ����ͬ�����ݡ�
	bool modify_name2(string file_path, string old_suffix, string new_suffix, string insert_context, int useful_location = 0)
	{//�˺���Ҫ�� edit_name2���ʹ��
		vector<string> files;
		getFiles(file_path.data(), files);
		for (int order = 0, file_num = files.size(); order < file_num; ++order)
		{
			string new_name = edit_name2(files[order], old_suffix, new_suffix, 
				insert_context, useful_location);//��ð�Ҫ��༭�õ�����

			if (0 == new_name.length())
			{
				cout << "�����Ʋ��Ϸ���" << endl << new_name << endl;
				continue;
			}

			if (rename(files[order].data(), new_name.data()))
				cout << "ʧ�ܣ�" << endl << files[order] << endl;
			else
				cout << "�ɹ���" << endl << files[order] << endl;

		}

		return true;//�˺����ķ���ֵ��δͶ��ʹ�ã�����ֱ�����ó� true
	}
	
}


