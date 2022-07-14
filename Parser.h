#pragma once
#include <vector>;
#include "Lexer.h";

using namespace std;

class Parser
{
private:
	Lexer L;

public:
	void backtrack(streampos s);// функция для отката курсора
	bool CheckLex(string s);	// Проверка лексемы
	bool CheckLex2(string s);	// Проверка лексемы с возвратом
	bool PROG();				// Начальная функция

	bool TEXT_OF_PROG();	// функция проверки текста программы
	bool DESCR_BLOCK();		// функция проверки блока описаний

	bool CONST_FIRST();		// функция проверки блока констант когда блок описаний начинается с блока констант
	bool DESCRCONSTS();		// функция проверки блока констант 
	bool VALUE_OF_CONST();	// функция проверки значений констант (число или булевское значение)

	bool TYPE_FIRST();		// функция проверки блока описания типов когда блок описаний начинается с блока описания типов
	bool DESCRTYPES();		// функция проверки блока описания типов 
	bool TYPE_OF_VALUE();	// функция проверки значений типов (integer, boolean, proc_type)
	bool PROC_TYPE();		// функция проверки процедурного типа
	bool PARAMS();			// функция проверки параметров процедур и функций
	bool METHOD();			// функция проверки модификаторов параметров процедур и функций
	bool ID_PARAMS();		// функция проверки id параметров процедур и функций
	bool TYPE_OF_VAR();		// функция проверки типов параметров процедур и функций

	bool VAR_FIRST();		// функция проверки блока описания переменных когда блок описаний начинается с блока описания переменных
	bool DESCRVARS();		// функция проверки блока описания переменных

	bool PROC_FUNC();		// функция проверки блока описания процедур и функций
	bool PROC();
	bool FUNC();

	bool OPERATIONS();		// функция проверки блока операторов
	bool OPERATION();
	bool OP_READ();			// функция проверки оператора read()
	bool OP_WRITE();		// функция проверки оператора write()

	bool BEGIN_END();		// функция проверки блока begin..end
	bool ASSIGN();			// функция проверки оператора :=
	bool EXP();				// функция проверки выражения
	bool TERM();			
	bool FACTOR();

	bool FACT_PARAMS();		// функция проверки параметров процедур и функций при вызове
	bool FACT_PARAM();
	bool CALL_PROC();

	void Error();			// функция для обработки ошибок
};

