#ifndef _PARSER_H
#define _PARSER_H

#include<bits/stdc++.h>
#include"scanner.h"

using namespace std;

struct ExprNode
{
    tokenType opcode; //节点对应的token type
    struct{ ExprNode *left,*right; }CaseOperator; //该节点为运算符时左右子树根节点指针
    struct{ ExprNode *child;double (*funcPtr)(double); }CaseFunc; //该节点为函数名时对应的函数地址和函数自变量的表达式
    double CaseConst; //该节点为常数时的常数值
    double *CaseParmPtr; //该节点为参数时对应的参数值
};

typedef struct ExprNode* ExprNode_Ptr;

//递归下降子程序
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

//初始化、运行、关闭Parser
void InitParser();
void CloseParser();
void Parser();

//语法树接口
struct ExprNode * MakeExprNode(tokenType opcode,ExprNode_Ptr left,ExprNode_Ptr right);//建语法树
void PrintSyntaxTree(ExprNode_Ptr root); //输出表达式语法树结构
void print_all_tree(); //输出token流的结构
void destroy_tree(ExprNode_Ptr root); //回收表达式语法树内存
void destroy_all_tree(); //回收token流结构存储用到的所有内存

void FetchToken();
void MatchToken(tokenType expected);
void print_token();

void InitNode(ExprNode_Ptr Node);

//#ifdef _PARSER_WITH_SEMANTICS
extern double *getT();
//#endif // _PARSER_WITH_SEMANTICS

#endif // _PARSER_H
