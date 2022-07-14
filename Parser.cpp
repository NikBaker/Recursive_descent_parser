#include "Parser.h"
#include "Lexer.h"
#include "algorithm"

using namespace std;

// Проверка лексемы
bool Parser::CheckLex(string s) {
    
    auto lexeme = L.FSM();
    if (lexeme == s) {
        int n = L.number_of_rows;
        if (n >= L.max) {
            L.max = n;
        }
        return true;
    }
    return false;
}

// Проверка лексемы с откатом
bool Parser::CheckLex2(string s) {

    auto lexeme = L.FSM();
    auto saved_pos2 = L.in.tellg();
    if (lexeme == s) {
        int n = L.number_of_rows;
        if (n >= L.max) {
            L.max = n;
        }
        return true;
    }
    backtrack(saved_pos2);
    return false;
}

void Parser::backtrack(streampos s) {
    auto right = L.in.tellg();
    auto sz = right - s;
    auto buf = string(sz , ' ');
    L.in.clear();
    L.in.seekg(s, ios::beg);  // откатились
    L.in.read(buf.data(), sz);
    int n = std::count(buf.begin(), buf.end(), '\n');

    L.number_of_rows -= n;
    L.in.seekg(s, ios::beg);
}

bool Parser::PROG()
{
    auto f = ifstream("input.txt"); 
    if (!f.is_open()) {
        cout << "Can't open the file";
    }
    else {
        L.in = istringstream(string(istreambuf_iterator<char>(f), istreambuf_iterator<char>()));
        if(CheckLex("program")) {
            if(CheckLex("ID")) {
                if(CheckLex(";")) {
                    if (TEXT_OF_PROG()) {
                        if(CheckLex(".")) {
                            auto lexeme6 = L.FSM();
                            if (lexeme6 == "NOTLEXEME") {
                                if (L.in.eof()) {
                                    cout << "Programm without errors!" << endl;
                                    return true;
                                }
                            }                           
                            else {
                                Error();
                                cout << "Error on the last row" << endl;
                                cout << "Nothing can come after end. " << endl;
                                return false;
                            }
                        }
                    }
                }
            }
        }
        
        Error();
        cout << "Error on line " << L.max + 1 << endl;
    }
}

bool Parser::TEXT_OF_PROG()
{
    auto saved_pos = L.in.tellg();  // Сохраняем текущую позицию курсора, так как есть ветвление
    if (DESCR_BLOCK()) {      
        if(CheckLex("begin")) {
            if (OPERATIONS()) {
                if(CheckLex("end")) {
                    return true;
                }
            }
        }
    }

    backtrack(saved_pos);           // Если не прошла первая ветка, возвращаем курсор на сохраненную в начале позицию 
    
    if(CheckLex("begin")) {
        if (OPERATIONS()) {
            if(CheckLex("end")) {
                return true;
            }
        }    
    }

    backtrack(saved_pos);                          
    
    return false;                   // Если ни одна ветка не вернула true, значит ошибка
}

bool Parser::DESCR_BLOCK()
{
    auto saved_pos = L.in.tellg();  // Сохраняем текущую позицию курсора, так как есть ветвление
    if(CheckLex("const")) {
        if (CONST_FIRST()) {
            return true;
        }
    }
        
    backtrack(saved_pos);           // Если не прошла первая ветка, возвращаем курсор на сохраненную в начале позицию

    if(CheckLex("type")) {
        if (TYPE_FIRST()) {
            return true;
        }
    }
    backtrack(saved_pos);           // Если не прошла вторая ветка, возвращаем курсор на сохраненную в начале позицию

    if(CheckLex("var")) {
        if (VAR_FIRST()) {
            return true;
        }
    }
    backtrack(saved_pos);           // Если не прошла третья ветка, возвращаем курсор на сохраненную в начале позицию

    if (PROC_FUNC()) {
        return true;
    }
    backtrack(saved_pos);                               

    return false;                   // Если ни одна ветка не вернула true

}

bool Parser::CONST_FIRST()
{
    auto saved_pos = L.in.tellg();
    if (DESCRCONSTS()) {
        auto saved_pos1 = L.in.tellg();  // Сохраняем курсор после одинаковой части во всех ветках
        if(CheckLex("type")) {
            if (TYPE_FIRST()) {
                return true;
            }
        }
        backtrack(saved_pos1);          // Возвращаем курсор для проверки других веток

        if(CheckLex("var")) {
            if (VAR_FIRST()) {
                return true;
            }
        }   
        backtrack(saved_pos1);          // Возвращаем курсор для проверки других веток 

        if (PROC_FUNC()) {
            return true;
        }
        backtrack(saved_pos1);                

        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::TYPE_FIRST()
{
    auto saved_pos = L.in.tellg();
    if (DESCRTYPES()) {
        auto saved_pos1 = L.in.tellg();  // Сохраняем курсор после одинаковой части во всех ветках
        if(CheckLex("var")) {
            if (VAR_FIRST()) {
                return true;
            }
        }
        backtrack(saved_pos1);

        if (PROC_FUNC()) {
            return true;
        }
        backtrack(saved_pos1);

        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::VAR_FIRST()
{
    auto saved_pos = L.in.tellg();
    if (DESCRVARS()) {
        auto saved_pos1 = L.in.tellg();
        if (PROC_FUNC()) {
            return true;
        }
        backtrack(saved_pos1);

        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::DESCRCONSTS()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("ID")) {
        if(CheckLex("=")) {
            if (VALUE_OF_CONST()) {
                if(CheckLex(";")) {
                    if (DESCRCONSTS()) {                
                        return true;
                    }
                    return true;
                }
            }
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::VALUE_OF_CONST()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("CONST")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("true")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("false")) {
        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::DESCRTYPES()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("ID")) {
        if(CheckLex("=")) {
            if (TYPE_OF_VALUE()) {
                if(CheckLex(";")) {
                    if (DESCRTYPES()) {
                        return true;
                    }
                    return true;
                }
            }
        }
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::TYPE_OF_VALUE()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("integer")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("boolean")) {
        return true;
    }
    backtrack(saved_pos);

    if (PROC_TYPE()) {
        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::PROC_TYPE() {
    auto saved_pos = L.in.tellg();
    if(CheckLex("procedure")) {
        if(CheckLex2("("))
        {
            if (PARAMS()) {
                if(CheckLex(")")) {
                    return true;
                }
            }
        }
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("function")) {
        auto saved_pos3 = L.in.tellg();
        if(CheckLex(":")) {
            if (TYPE_OF_VAR()) {
                return true;
            }
        }
        backtrack(saved_pos3);

        if(CheckLex("(")) {
            if (PARAMS()) {
                if(CheckLex(")")) {
                    if(CheckLex(":")) {
                        if (TYPE_OF_VAR()) {
                            return true;
                        }
                    }
                }
            }
            
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::PARAMS()
{
    auto saved_pos = L.in.tellg();
    if (METHOD()) {
        if (ID_PARAMS()) {
            if(CheckLex(":")) {
                if (TYPE_OF_VAR()) {
                    auto saved_pos2 = L.in.tellg();
                    if(CheckLex(";")) {
                        if (PARAMS()) {
                            return true;
                        }
                    }
                    backtrack(saved_pos2);
                    return true;
                }
            }
        }
    }
    backtrack(saved_pos);

    if (ID_PARAMS()) {
        if(CheckLex(":")) {
            if (TYPE_OF_VAR()) {
                auto saved_pos3 = L.in.tellg();
                if(CheckLex(";")) {
                    if (PARAMS()) {
                        return true;
                    }
                }
                backtrack(saved_pos3);
                return true;
            }
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::METHOD()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("var")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("const")) {
        return true;
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::ID_PARAMS()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("ID")) {
        auto saved_pos2 = L.in.tellg();
        if(CheckLex(",")) {
            if (ID_PARAMS()) {
                return true;
            }
        }
        backtrack(saved_pos2);

        return true;
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::TYPE_OF_VAR()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("integer")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("boolean")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("ID")) {
        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::DESCRVARS()
{
    auto saved_pos = L.in.tellg();
    if (ID_PARAMS()) {
        if(CheckLex(":")) {
            if (TYPE_OF_VAR()) {
                if(CheckLex(";")) { 
                    if (DESCRVARS()) {
                        return true;
                    }   
                    return true;
                }
            }
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::PROC_FUNC()
{
    auto saved_pos = L.in.tellg();
    if (PROC()) {
        if (PROC_FUNC()) {
            return true;
        }
        return true;
    }
    backtrack(saved_pos);

    if (FUNC()) {
        if (PROC_FUNC()) {
            return true;
        }
        return true;
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::PROC()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("procedure")) {
        if(CheckLex("ID")) {
            auto saved_pos2 = L.in.tellg();     // первый сейв
            if(CheckLex("(")) {
                if (PARAMS()) {
                    if(CheckLex(")")) {
                        if(CheckLex(";")) {
                            auto saved_pos3 = L.in.tellg();     // второй сейв
                            if (DESCR_BLOCK()) {                        
                                if(CheckLex("begin")) {
                                    if (OPERATIONS()) {
                                        if(CheckLex("end")) {
                                            if(CheckLex(";")) {
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                            backtrack(saved_pos3);

                            if (CheckLex("begin")) {
                                if (OPERATIONS()) {
                                    if (CheckLex("end")) {
                                        if (CheckLex(";")) {
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            backtrack(saved_pos2);

            if(CheckLex(";")) {
                auto saved_pos4 = L.in.tellg();     // третий сейв
                if (DESCR_BLOCK()) {                    
                    if (CheckLex("begin")) {
                        if (OPERATIONS()) {
                            if (CheckLex("end")) {
                                if (CheckLex(";")) {
                                    return true;
                                }
                            }                           
                        }
                    }
                }
                backtrack(saved_pos4);
                if (CheckLex("begin")) {
                    if (OPERATIONS()) {
                        if (CheckLex("end")) {
                            if (CheckLex(";")) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::FUNC()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("function")) {
        if(CheckLex("ID")) {
            auto saved_pos2 = L.in.tellg();             // первый сейв
            if(CheckLex("(")) {
                if (PARAMS()) {
                    if(CheckLex(")")) {
                        if(CheckLex(":")) {
                            if (TYPE_OF_VAR()) {
                                if(CheckLex(";")) {
                                    auto saved_pos3 = L.in.tellg();              // второй сейв
                                    if (DESCR_BLOCK()) {                
                                        if(CheckLex("begin")) {
                                            if (OPERATIONS()) {
                                                if(CheckLex("end")) {
                                                    if(CheckLex(";")) {
                                                        return true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    backtrack(saved_pos3);
                                    if (CheckLex("begin")) {
                                        if (OPERATIONS()) {                                            
                                            if (CheckLex("end")) {                                                
                                                if (CheckLex(";")) {
                                                    return true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            backtrack(saved_pos2);

            if (CheckLex(":")) {
                if (TYPE_OF_VAR()) {
                    if (CheckLex(";")) {
                        auto saved_pos4 = L.in.tellg();              // третий сейв
                        if (DESCR_BLOCK()) {                        
                            if (CheckLex("begin")) {
                                if (OPERATIONS()) {
                                    if (CheckLex("end")) {
                                        if (CheckLex(";")) {
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                        backtrack(saved_pos4);

                        if (CheckLex("begin")) {
                            if (OPERATIONS()) {
                                if (CheckLex("end")) {
                                    if (CheckLex(";")) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::OPERATIONS()   
{

    auto saved_pos = L.in.tellg();
    if (OPERATION()) {
        if(CheckLex(";")) {
            if (OPERATIONS()) {
                return true;
            }
            return true;
        }
    }
    backtrack(saved_pos);
    
    if(CheckLex(";")) {
        if (OPERATIONS()) {
            return true;
        }
        return true;
    }

    backtrack(saved_pos);

    return false;
}

bool Parser::OPERATION()
{
    auto saved_pos = L.in.tellg();
    if (ASSIGN()) {
        return true;
    }

    if (CALL_PROC()) {
        return true;
    }

    if (OP_READ()) {
        return true;
    }

    if (OP_WRITE()) {
        return true;
    }

    if (BEGIN_END()) {
        return true;
    }

    backtrack(saved_pos);
    
    return false;
}


bool Parser::OP_READ()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("read")) {
        if(CheckLex("(")) {
            auto saved_pos2 = L.in.tellg();
            if (ID_PARAMS()) {
                if(CheckLex(")")) {
                    return true;
                }
            }
            backtrack(saved_pos2);

            if(CheckLex(")")) {
                return true;
            }
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::OP_WRITE()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("write")) {
        if(CheckLex("(")) {
            auto saved_pos2 = L.in.tellg();
            if (ID_PARAMS()) {
                if(CheckLex(")")) {
                    return true;
                }
            }
            backtrack(saved_pos2);

            if(CheckLex(")")) {
                return true;
            }
        }
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::BEGIN_END()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("begin")) {
        if (OPERATIONS()) {
            if(CheckLex("end")) {
                return true;
            }
        }
    }
    backtrack(saved_pos);
    
    return false;
}


bool Parser::ASSIGN()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("ID")) {
        if(CheckLex(":=")) {
            if (EXP()) {
                return true;
            }

            auto saved_pos2 = L.in.tellg(); //
            if(CheckLex("true")) {
                return true;
            }
            backtrack(saved_pos2);
            if (CheckLex("false")) {
                return true;
            }
            backtrack(saved_pos2);
        }
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::EXP()
{
    auto saved_pos = L.in.tellg();
    if (TERM()) {
        auto saved_pos2 = L.in.tellg();
        if(CheckLex("+")) {
            if (EXP()) {
                return true;
            }
        }
        backtrack(saved_pos2);
        if(CheckLex("-")) {
            if (EXP()) {
                return true;
            }
        }
        backtrack(saved_pos2);

        return true;
    }
    backtrack(saved_pos);

    return false;
}



bool Parser::TERM()
{
    auto saved_pos = L.in.tellg();
    if (FACTOR()) {
        auto saved_pos2 = L.in.tellg();
        if(CheckLex("*")) {
            if (TERM()) {
                return true;
            }
        }
        backtrack(saved_pos2);
        if(CheckLex("/")) {
            if (TERM()) {
                return true;
            }
        }
        backtrack(saved_pos2);

        return true;
    }

    backtrack(saved_pos);

    return false;
}

bool Parser::FACTOR()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("ID")) {
        auto saved_pos2 = L.in.tellg();  
        if(CheckLex("(")) {                 // Вызов функции
            if (FACT_PARAMS()) {
                if(CheckLex(")")) {
                    return true;
                }
            }
        }
        backtrack(saved_pos2);

        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("CONST")) {
        return true;
    }
    backtrack(saved_pos);

    if(CheckLex("(")) {
        if (EXP()) {
            if(CheckLex(")")) {
                return true;
            }
        }
    }
    backtrack(saved_pos);

    return false;
}


bool Parser::FACT_PARAMS()
{
    auto saved_pos = L.in.tellg();
    if (FACT_PARAM()) {
        auto saved_pos2 = L.in.tellg();
        if(CheckLex(",")) {
            if (FACT_PARAMS()) {
                return true;
            }
        }
        backtrack(saved_pos2);

        return true;
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::FACT_PARAM()
{
    auto saved_pos = L.in.tellg();
    if (EXP()) {
        return true;
    }
    backtrack(saved_pos);

    return false;
}

bool Parser::CALL_PROC()
{
    auto saved_pos = L.in.tellg();
    if(CheckLex("ID")) {
        auto saved_pos2 = L.in.tellg();
        if(CheckLex("(")) {
            if (FACT_PARAMS()) {
                if(CheckLex(")")) {
                    return true;
                }
            }
        }
        backtrack(saved_pos2);

        return true;
    }
    backtrack(saved_pos);

    return false;
}

void Parser::Error()
{
    cout << "Programm with error!" << endl;
}

