#include "scanner.h"
#include <iostream>
#include <iomanip>

int cntToken=0;
struct token tks[1000];

std::string enum2str(tokenType tp)
{
    switch(tp)
    {
        case ID: return "ID";
        case COMMENT: return "COMMENT";
        case ORIGIN: return "ORIGIN";
        case SCALE: return "SCALE";
        case ROT: return "ROT";
        case IS: return "IS";
        case TO: return "TO";
        case STEP: return "STEP";
        case DRAW: return "DRAW";
        case FOR: return "FOR";
        case FROM: return "FROM";
        case T: return "T";
        case SEMICO: return "SEMICO";
        case L_BRACKET: return "L_BRACKET";
        case R_BRACKET: return "R_BRACKET";
        case COMMA: return "COMMA";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case POWER: return "POWER";
        case FUNC: return "FUNC";
        case CONST_ID: return "CONST_ID";
        case NONTOKEN: return "NONTOKEN";
        case ERRTOKEN: return "ERRTOKEN";
    }
    return NULL;
}

int main()
{
    initScanner();
    std::cout<<std::left<<std::setw(10)<<"Token Type"<<"\t|\t"<<std::setw(12)<<"Lexeme"<<"\t|\t"<<std::setw(14)<<"Value"<<"\t|\t"<<std::setw(16)<<"Function Pointer" <<"\t|\t"<<std::setw(6)<<"Row"<<std::endl;
    struct token tki;
    std::string tmp;
    for(tki=getToken();tki.type!=NONTOKEN;tki=getToken())
    {
        if(!tki.funcPtr)
            std::cout<<std::left<<std::setw(10)<<enum2str(tki.type)<<"\t|\t"<<std::setw(12)<<tki.lexeme<<"\t|\t"<<std::setw(14)<<tki.value<<"\t|\t"<<std::setw(16)<<"NULL"<<"\t|\t"<<std::setw(6)<<tki.row<<std::endl;
        else
        {
            std::cout<<std::left<<std::setw(10)<<enum2str(tki.type)<<"\t|\t"<<std::setw(12)<<tki.lexeme<<"\t|\t"<<std::setw(14)<<tki.value<<"\t|\t"<<std::setw(16)<<tki.lexeme<<"\t|\t"<<std::setw(6)<<tki.row<<std::endl;
            //std::cout<<tki.funcPtr(3.1415926/6)<<std::endl;
        }
    }
    closeScanner();
    system("pause");
    return 0;
}