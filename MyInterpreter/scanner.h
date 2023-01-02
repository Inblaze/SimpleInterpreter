#ifndef _SCANNER_H_
#define _SCANNER_H_
#include <cstring>
#include <cmath>
#include<fstream>
#include <cstring>

enum tokenType
{
    ID,
    COMMENT, // // -- 表示注释
    ORIGIN,SCALE,ROT,IS,TO,STEP,DRAW,FOR,FROM,//保留字
    T, //参数
    SEMICO,L_BRACKET,R_BRACKET,COMMA, //; ( ) , 分隔符
    PLUS,MINUS,MUL,DIV,POWER,  //+ - * / ** 运算符
    FUNC, //函数指针
    CONST_ID, //常数
    NONTOKEN, //终结符
    ERRTOKEN, //错误符号
    NONINIT    //开始符号
};

struct token
{
    tokenType type; //token类别
    std::string lexeme; //token对应的字符串值
    double value; //如果token为常数则表示常数的数值
    double (*funcPtr)(double); //如果token为函数名则表示指向的函数地址
    int row; //token所在行号
};

//graph-链式前向星
struct Node
{
    bool isF; //该节点是否为终态
    tokenType type; //如果节点为终态则表示对应的token类型
};

struct Edge
{
    bool isD; //该边是否为数字
    bool isL; //该边是否为字母
    int tag; //该边的符号
    int to; //通过该边到达的状态
    int next;
};


//function
extern void initScanner();
extern void closeScanner();
extern struct token getToken(); //获取下一个token
extern int prepare(struct token& pToken); //去掉前头的空白符，返回第一个非空白字符
extern int scan(struct token& pToken,int firstChar); //根据第一个非空字符开始接收字符流
extern int move(int nownode,int nowc); //根据目前状态和字符在DFA中寻找下一步的状态
extern int GetChar(); //获取下一个字符，如果是字母将其转换为大写
extern void setToken(struct token& tk,tokenType tp,double v,double (* funcP)(double),int r); //设置token的type，value，函数指针和行号

#endif