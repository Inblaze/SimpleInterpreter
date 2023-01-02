#ifndef _PARSER_H
#define _PARSER_H

#include<bits/stdc++.h>
#include"scanner.h"

using namespace std;

struct ExprNode
{
    tokenType opcode;
    struct{ ExprNode *left,*right; }CaseOperator;
    struct{ ExprNode *child;double (*funcPtr)(double); }CaseFunc;
    double CaseConst;
    double *CaseParmPtr;
};

typedef struct ExprNode* ExprNode_Ptr;

void program();
void statement();
void for_statement();
void origin_statement();
void rot_statement();
void scale_statement();
ExprNode_Ptr expression();
ExprNode_Ptr term();
ExprNode_Ptr factor();
ExprNode_Ptr component();
ExprNode_Ptr atom();

int InitParser();
void CloseParser();
void Parser();

void PrintSyntaxTree(ExprNode_Ptr root,int choose);//打印语法树
void print_all_tree();
void destroy_tree(ExprNode_Ptr root);
void destroy_all_tree();

void FetchToken();
void MatchToken(tokenType expected);
void print_token();
void SyntaxError(int case_of);

struct ExprNode * MakeExprNode(tokenType opcode,ExprNode_Ptr left,ExprNode_Ptr right);//构造语法树
void InitNode(ExprNode_Ptr Node);//初始化节点
void LogPrint(std::string content, int choose);


extern void Parser(const char * file_name);

//#ifdef _PARSER_WITH_SEMANTICS
extern double *getT();
//#endif // _PARSER_WITH_SEMANTICS

#endif // _PARSER_H
