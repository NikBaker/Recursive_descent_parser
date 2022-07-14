#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Lexer
{
private:
    istringstream in;           // ������� �����
    string lex;                 // ������
    int number_of_separators;   // ���������� ������������ � ������ �������
    int number_of_rows;         // ����� ����������� ����� ������ � �������
    int max;

    vector<string> ListOfKeyWord = { "program", "var", "const", "begin", "end", "write", "read", "type", "function", "procedure", "true", "false", "integer", "boolean"};
public:
    Lexer();
    string Lower_string(const string& s);      // ������� ������ � ������ �������
    bool is_letter(const char& c);
    bool is_number(const char& c);
    string FSM();                              // ���������� ��������� �������
    string id();                               // id_lexeme
    string num();                              // num_lexeme
    string doubledot();                        // doubledot_lexeme
    friend class Parser;
};

