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
    auto ch = char(in.get()); // считываем первый символ в лексеме (нач сост)
    lex.push_back(ch); // добавляем символ в буффер (Нужен для ID)

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
        return "(";             // возвращаем лексему (
    }
    else if (ch == ')') {
        return ")";             // возвращаем лексему )
    }
    else if (ch == ',') {
        return ",";             // возвращаем лексему ,
    }
    else if (ch == '=') {
        return "=";             // возвращаем лексему =
    }
    else if (ch == ';') {
        return ";";             // возвращаем лексему ;
    }
    else if (ch == '.') {
        return ".";             // возкращаем лексему .
    }
    else if (ch == '+') {
        return "+";             // возкращаем лексему +
    }
    else if (ch == '-') {
        return "-";             // возкращаем лексему -
    }
    else if (ch == '*') {
        return "*";             // возкращаем лексему *
    }
    else if (ch == '/') {
        return "/";             // возкращаем лексему /
    }
    else {
        return "NOTLEXEME";
    }
}

string Lexer::id()
{
    auto ch = char(in.get());
    if (in.eof()) {  // если конец файла
        if (find(ListOfKeyWord.begin(), ListOfKeyWord.end(), (Lower_string(lex))) != ListOfKeyWord.end()) {  // если буффер равен какому-то ключевому слову
            return Lower_string(lex);
        }
        else {  // получили id
            return "ID";
        }
    }

    if (!is_letter(ch) && !is_number(ch)) {
        in.unget(); // вернули неудачный символ в поток
        if (find(ListOfKeyWord.begin(), ListOfKeyWord.end(), (Lower_string(lex))) != ListOfKeyWord.end()) {  // если буффер равен какому-то ключевому слову
            return Lower_string(lex);
        }
        else {  // получили id
            return "ID";
        }
    }
    else {
        lex.push_back(ch);  // записываем текущий символ в буффер
        return id();
    }
}

string Lexer::num()
{
    auto ch = char(in.get());

    if (in.eof()) {  // если конец файла
        return "CONST";
    }

    if (!is_number(ch)) {
        in.unget(); // вернули неудачный символ в поток для его обработки в след лексеме
        return "CONST";
    }
    else {
        lex.push_back(ch);  // записываем текущий символ в буффер
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
        in.unget();  // вернули неудачный символ в поток для его обработки в след лексеме
        return ":";
    }
    else {
        lex.push_back(ch);
        return ":=";
    }
}
