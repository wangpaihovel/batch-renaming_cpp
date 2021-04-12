#pragma once

#include <string>

using std::string;

class TypeConversion
{
private:
	char* pointer_int2char = nullptr;
	string* pointer_int2string = nullptr;

public:
	char* int2char(int target)
	{
		delete[] pointer_int2char;

		int target_absolute = target;
		bool sign = 0;//������,Ϊfalse��ʾ������Ϊtrue��ʾ��������true��Ϊ�����кܶ�ԭ�򣬱ȷ�˵���Ǹ�����Ҫ���кܶ฽�ӵĲ���
		if (target < 0)
		{
			sign = true;
			target_absolute *= -1;//ȡ����ֵ
		}

		int digit = 0;//��ø��ַ���λ��
		int target_abs_dup = target_absolute;//target_abs_dup��target absolute duplicate Ŀ�����ֵ�ĸ���

		/*֮�����������ѭ��������Ϊʹ����ģ�ķ���ֻ�ܴ�Ŀ��ĸ�λ����ʼ��ã�
		**���ﲻ����ʹ��ջ���������������һЩ�����Ȼ��Ŀ���λ��.
		**����Ϊ�˺��洴�����ʴ�С������*/
		do
		{
			target_abs_dup /= 10;
			digit++;
		} while (target_abs_dup);

		char* bit_array = new char[digit + sign + 1];
		if (sign)
			bit_array[0] = '-';//���Ŀ���Ǹ��������ַ���ǰ���һ������
		bit_array[digit + sign] = '\0';//������������Ϊ�ַ���
		for (int bit = digit + sign - 1, target_abs_dup = target_absolute; bit >= sign; --bit)//��λ�ŵ�����ĺ���
		{
			bit_array[bit] = target_abs_dup % 10 + '0';//���ַ�0��Ϊ�˽���λ��intֵת��Ϊ����char�ַ�
			target_abs_dup /= 10;
		}

		this->pointer_int2char = bit_array;

		return this->pointer_int2char;
	}

	string& int2string(int target)//Ҳ����ʹ��C++11���ӵķ���to_string������ʹ�ô˷���
	{
		delete[] pointer_int2string;

		string* p_result = new string;
		this->pointer_int2string = p_result;
		*p_result = "";//��ʼ��

		char* int2char_backup = this->pointer_int2char;//����int2char�����е�����
		this->pointer_int2char = nullptr;

		*p_result += this->int2char(target);

		this->pointer_int2char = int2char_backup;//�ָ�int2char�����е�����

		return *p_result;
	}

public:
	TypeConversion(){}

	~TypeConversion()//����������Ҳ�����ֶ�����
	{
		delete[] pointer_int2char;
		this->pointer_int2char = nullptr;

		delete pointer_int2string;
		this->pointer_int2string = nullptr;
	}

};
