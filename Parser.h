#pragma once
#include <vector>;
#include "Lexer.h";

using namespace std;

class Parser
{
private:
	Lexer L;

public:
	void backtrack(streampos s);// ������� ��� ������ �������
	bool CheckLex(string s);	// �������� �������
	bool CheckLex2(string s);	// �������� ������� � ���������
	bool PROG();				// ��������� �������

	bool TEXT_OF_PROG();	// ������� �������� ������ ���������
	bool DESCR_BLOCK();		// ������� �������� ����� ��������

	bool CONST_FIRST();		// ������� �������� ����� �������� ����� ���� �������� ���������� � ����� ��������
	bool DESCRCONSTS();		// ������� �������� ����� �������� 
	bool VALUE_OF_CONST();	// ������� �������� �������� �������� (����� ��� ��������� ��������)

	bool TYPE_FIRST();		// ������� �������� ����� �������� ����� ����� ���� �������� ���������� � ����� �������� �����
	bool DESCRTYPES();		// ������� �������� ����� �������� ����� 
	bool TYPE_OF_VALUE();	// ������� �������� �������� ����� (integer, boolean, proc_type)
	bool PROC_TYPE();		// ������� �������� ������������ ����
	bool PARAMS();			// ������� �������� ���������� �������� � �������
	bool METHOD();			// ������� �������� ������������� ���������� �������� � �������
	bool ID_PARAMS();		// ������� �������� id ���������� �������� � �������
	bool TYPE_OF_VAR();		// ������� �������� ����� ���������� �������� � �������

	bool VAR_FIRST();		// ������� �������� ����� �������� ���������� ����� ���� �������� ���������� � ����� �������� ����������
	bool DESCRVARS();		// ������� �������� ����� �������� ����������

	bool PROC_FUNC();		// ������� �������� ����� �������� �������� � �������
	bool PROC();
	bool FUNC();

	bool OPERATIONS();		// ������� �������� ����� ����������
	bool OPERATION();
	bool OP_READ();			// ������� �������� ��������� read()
	bool OP_WRITE();		// ������� �������� ��������� write()

	bool BEGIN_END();		// ������� �������� ����� begin..end
	bool ASSIGN();			// ������� �������� ��������� :=
	bool EXP();				// ������� �������� ���������
	bool TERM();			
	bool FACTOR();

	bool FACT_PARAMS();		// ������� �������� ���������� �������� � ������� ��� ������
	bool FACT_PARAM();
	bool CALL_PROC();

	void Error();			// ������� ��� ��������� ������
};

