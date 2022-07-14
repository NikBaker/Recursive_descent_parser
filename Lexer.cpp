#include "Lexer.h"

Lexer::Lexer()
{
    max = 0;
	lex = "";
    number_of_separators = 0;
    number_of_rows = 0;
}

string Lexer::Lower_string(const string& s)
{
    string tmp = s;
    for (int i = 0; i < tmp.size(); ++i) {
        tmp[i] = tolower(tmp[i]);
    }
    return tmp;
}

bool Lexer::is_letter(const char& c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool Lexer::is_number(const char& c)
{
    return (c >= '0' && c <= '9');
}

string Lexer::FSM()
{
    lex = "";
    auto ch = char(in.get()); // ��������� ������ ������ � ������� (��� ����)
    lex.push_back(ch); // ��������� ������ � ������ (����� ��� ID)

    if (ch == ' ' || ch == '\n' || ch == '\t') {
        if (ch == '\n') {
            number_of_rows++;
        }
        number_of_separators++;  
        return FSM();
    }

    if (is_letter(ch)) {
        return id();
    }
    else if (is_number(ch)) {
        return num();
    }
    else if (ch == ':') {
        return doubledot();
    }
    else if (ch == '(') {
        return "(";             // ���������� ������� (
    }
    else if (ch == ')') {
        return ")";             // ���������� ������� )
    }
    else if (ch == ',') {
        return ",";             // ���������� ������� ,
    }
    else if (ch == '=') {
        return "=";             // ���������� ������� =
    }
    else if (ch == ';') {
        return ";";             // ���������� ������� ;
    }
    else if (ch == '.') {
        return ".";             // ���������� ������� .
    }
    else if (ch == '+') {
        return "+";             // ���������� ������� +
    }
    else if (ch == '-') {
        return "-";             // ���������� ������� -
    }
    else if (ch == '*') {
        return "*";             // ���������� ������� *
    }
    else if (ch == '/') {
        return "/";             // ���������� ������� /
    }
    else {
        return "NOTLEXEME";
    }
}

string Lexer::id()
{
    auto ch = char(in.get());
    if (in.eof()) {  // ���� ����� �����
        if (find(ListOfKeyWord.begin(), ListOfKeyWord.end(), (Lower_string(lex))) != ListOfKeyWord.end()) {  // ���� ������ ����� ������-�� ��������� �����
            return Lower_string(lex);
        }
        else {  // �������� id
            return "ID";
        }
    }

    if (!is_letter(ch) && !is_number(ch)) {
        in.unget(); // ������� ��������� ������ � �����
        if (find(ListOfKeyWord.begin(), ListOfKeyWord.end(), (Lower_string(lex))) != ListOfKeyWord.end()) {  // ���� ������ ����� ������-�� ��������� �����
            return Lower_string(lex);
        }
        else {  // �������� id
            return "ID";
        }
    }
    else {
        lex.push_back(ch);  // ���������� ������� ������ � ������
        return id();
    }
}

string Lexer::num()
{
    auto ch = char(in.get());

    if (in.eof()) {  // ���� ����� �����
        return "CONST";
    }

    if (!is_number(ch)) {
        in.unget(); // ������� ��������� ������ � ����� ��� ��� ��������� � ���� �������
        return "CONST";
    }
    else {
        lex.push_back(ch);  // ���������� ������� ������ � ������
        return num();
    }
}

string Lexer::doubledot()
{
    auto ch = char(in.get());
    if (in.eof()) {
        return ":";
    }

    if (ch != '=') {
        in.unget();  // ������� ��������� ������ � ����� ��� ��� ��������� � ���� �������
        return ":";
    }
    else {
        lex.push_back(ch);
        return ":=";
    }
}
