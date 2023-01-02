#ifndef _SCANNER_H_
#define _SCANNER_H_
#include <cstring>
#include <cmath>
#include<fstream>
#include <cstring>

enum tokenType
{
    ID,
    COMMENT, // // -- ��ʾע��
    ORIGIN,SCALE,ROT,IS,TO,STEP,DRAW,FOR,FROM,//������
    T, //����
    SEMICO,L_BRACKET,R_BRACKET,COMMA, //; ( ) , �ָ���
    PLUS,MINUS,MUL,DIV,POWER,  //+ - * / ** �����
    FUNC, //����ָ��
    CONST_ID, //����
    NONTOKEN, //�ս��
    ERRTOKEN, //�������
    NONINIT    //��ʼ����
};

struct token
{
    tokenType type; //token���
    std::string lexeme; //token��Ӧ���ַ���ֵ
    double value; //���tokenΪ�������ʾ��������ֵ
    double (*funcPtr)(double); //���tokenΪ���������ʾָ��ĺ�����ַ
    int row; //token�����к�
};

//graph-��ʽǰ����
struct Node
{
    bool isF; //�ýڵ��Ƿ�Ϊ��̬
    tokenType type; //����ڵ�Ϊ��̬���ʾ��Ӧ��token����
};

struct Edge
{
    bool isD; //�ñ��Ƿ�Ϊ����
    bool isL; //�ñ��Ƿ�Ϊ��ĸ
    int tag; //�ñߵķ���
    int to; //ͨ���ñߵ����״̬
    int next;
};


//function
extern void initScanner();
extern void closeScanner();
extern struct token getToken(); //��ȡ��һ��token
extern int prepare(struct token& pToken); //ȥ��ǰͷ�Ŀհ׷������ص�һ���ǿհ��ַ�
extern int scan(struct token& pToken,int firstChar); //���ݵ�һ���ǿ��ַ���ʼ�����ַ���
extern int move(int nownode,int nowc); //����Ŀǰ״̬���ַ���DFA��Ѱ����һ����״̬
extern int GetChar(); //��ȡ��һ���ַ����������ĸ����ת��Ϊ��д
extern void setToken(struct token& tk,tokenType tp,double v,double (* funcP)(double),int r); //����token��type��value������ָ����к�

#endif