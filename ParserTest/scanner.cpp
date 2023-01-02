#include "scanner.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

FILE *fp;
unsigned int nowLine;
struct token tokenTab[]=
{
    {CONST_ID,"PI",3.1415926,NULL}, //0
    {CONST_ID,"E",2.71828,NULL}, //1
    {T,"T",0.0,NULL}, //2
    {FUNC,"SIN",0.0,sin}, //3
    {FUNC,"COS",0.0,cos}, //4
    {FUNC,"TAN",0.0,tan}, //5
    {FUNC,"LN",0.0,log}, //6
    {FUNC,"EXP",0.0,exp}, //7
    {FUNC,"SQRT",0.0,sqrt}, //8
    {ORIGIN,"ORIGIN",0.0,NULL}, //9
    {SCALE,"SCALE",0.0,NULL}, //10
    {ROT,"ROT",0.0,NULL}, //11
    {IS,"IS",0.0,NULL}, //12
    {FOR,"FOR",0.0,NULL}, //13
    {FROM,"FROM",0.0,NULL}, //14
    {TO,"TO",0.0,NULL}, //15
    {STEP,"STEP",0.0,NULL},//16
    {DRAW,"DRAW",0.0,NULL}, //17
    {COMMENT,"//",0.0,NULL}, //18
    {COMMENT,"--",0.0,NULL}, //19
    {SEMICO,";",0.0,NULL}, //20
    {L_BRACKET,"(",0.0,NULL}, //21
    {R_BRACKET,")",0.0,NULL}, //22
    {COMMA,",",0.0,NULL}, //23
    {PLUS,"+",0.0,NULL}, //24
    {MINUS,"-",0.0,NULL}, //25
    {MUL,"*",0.0,NULL}, //26
    {DIV,"/",0.0,NULL}, //27
    {POWER,"**",0.0,NULL} //28
}; //符号表;

struct Node nds[]=
{
    //isF,type
    {false,T},
    {true,ID},
    {true,CONST_ID},
    {true,CONST_ID},
    {true,MUL},
    {true,POWER},
    {true,DIV},
    {true,MINUS},
    {true,PLUS},
    {true,COMMA},
    {true,SEMICO},
    {true,L_BRACKET},
    {true,R_BRACKET},
    {true,COMMENT}
};

struct Edge eds[]=
{
    //isD,isL,tag,to,next
    {false,true,-2,1,-1},
    {false,true,-2,1,-1},
    {true,false,-2,2,0},
    {true,false,-2,2,-1},
    {false,false,'.',3,3},
    {true,false,-2,3,-1},
    {false,false,'*',4,2},
    {false,false,'*',5,-1},
    {false,false,'/',6,6},
    {false,false,'/',13,-1},
    {false,false,'-',7,8},
    {false,false,'-',13,-1},
    {false,false,'+',8,10},
    {false,false,',',9,12},
    {false,false,';',10,13},
    {false,false,'(',11,14},
    {false,false,')',12,15}
};

int head[]={16,1,4,5,7,-1,9,11,-1,-1,-1,-1,-1,-1};

void initScanner()
{
    std::cout<<"Please input the filename you wanna compile:"<<std::endl;
    char filename[128];
    scanf("%s",filename);
    nowLine=1;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        std::printf("%s does not exist in current path!\n",filename);
        system("pause");
        exit(EXIT_SUCCESS);
    }
}

void closeScanner()
{fclose(fp);}

int move(int nownode,int nowc)
{
    if(isdigit(nowc))
    {
        for(int nowe=head[nownode];nowe!=-1;nowe=eds[nowe].next)
        {
            if(eds[nowe].isD)
                return eds[nowe].to;
        }
        return -1;
    }
    if(isalpha(nowc))
    {
        for(int nowe=head[nownode];nowe!=-1;nowe=eds[nowe].next)
        {
            if(eds[nowe].isL)
                return eds[nowe].to;
        }
        return -1;
    }
    for(int nowe=head[nownode];nowe!=-1;nowe=eds[nowe].next)
    {
        if(eds[nowe].tag==nowc)
            return eds[nowe].to;
    }
    return -1;
}

int scan(struct token& pToken,int firstChar)
{
    int nowc=firstChar;
    int nownode=0,nextnode;
    pToken.row=nowLine;
    while(true)
    {
        nextnode=move(nownode,nowc);
        if(nextnode==-1)
        {
            if(!nds[nownode].isF) //当前状态非终态
            {
                do
                {
                    pToken.lexeme.push_back(nowc);
                    nowc=GetChar();
                }while(nowc!='+' && nowc!='-' && nowc!='*' && nowc!='/' && nowc!=',' && nowc!=';' && nowc!='(' && nowc!=')' && nowc!=-1 && !isspace(nowc));
                ungetc(nowc,fp);
                return -1;
            }
            if(nowc=='+' || nowc=='-' || nowc=='*' || nowc=='/' || nowc==',' || nowc==';' || nowc=='(' || nowc==')' || nowc==-1 || isspace(nowc))
            {
                ungetc(nowc,fp);
                return nownode;
            }
            if(nds[nownode].type==COMMENT)
            {
                while(true)
                {
                    if(nowc=='\n') break;
                    if(nowc==-1)
                    {
                        ungetc(nowc,fp);
                        break;
                    }
                    nowc=GetChar();
                }
                nowLine++;
                return nownode;
            }
            if(nds[nownode].type==SEMICO || nds[nownode].type==L_BRACKET || nds[nownode].type==R_BRACKET || nds[nownode].type==COMMA || nds[nownode].type==PLUS || nds[nownode].type==MINUS || nds[nownode].type==MUL || nds[nownode].type==DIV || nds[nownode].type==POWER)
            {
                ungetc(nowc,fp);
                return nownode;
            }
            do
            {
                pToken.lexeme.push_back(nowc);
                nowc=GetChar();
            }while(nowc!='+' && nowc!='-' && nowc!='*' && nowc!='/' && nowc!=',' && nowc!=';' && nowc!='(' && nowc!=')' && nowc!=-1 && !isspace(nowc));
            ungetc(nowc,fp);
            return -1;
        }
        pToken.lexeme.push_back(nowc);
        nownode=nextnode;
        nowc=GetChar();
    }
}

//去掉前头的空白符，返回第一个非空白字符
int prepare(struct token& pToken)
{
    int currentChar;
    memset(&pToken,0,sizeof(struct token));
    while(true)
    {
        currentChar=GetChar();
        if(currentChar==-1)
            return -1;
        if(currentChar=='\n')
            nowLine++;
        if(!isspace(currentChar))
            return currentChar;
    }
}

int GetChar()
{
    int c=getc(fp);
    if(c==EOF)
        return -1;
    return toupper(c);
}

void setToken(struct token& tk,tokenType tp,double v,double (* funcP)(double),int r)
{
    tk.type=tp;
    tk.value=v;
    tk.funcPtr=funcP;
    tk.row=r;
}

struct token getToken()
{
    int firstChar; //token第一个字符
    int lastState=-1; //识别记号过程结束时的状态
    struct token t;
    firstChar=prepare(t);
    if(firstChar==-1)
    {
        setToken(t,NONTOKEN,0.0,NULL,nowLine);
        t.lexeme=" ";
        return t;
    }
    lastState=scan(t,firstChar);
    //post_work
    if(lastState==-1)
    {
        setToken(t,ERRTOKEN,0.0,NULL,nowLine);
        return t;
    }
    switch(nds[lastState].type)
    {
        case PLUS:
        {
            setToken(t,PLUS,0.0,NULL,nowLine);
            t.lexeme="+";
            break;
        }
        case MINUS:
        {
            setToken(t,MINUS,0.0,NULL,nowLine);
            t.lexeme="-";
            break;
        }
        case MUL:
        {
            setToken(t,MUL,0.0,NULL,nowLine);
            t.lexeme="*";
            break;
        }
        case DIV:
        {
            setToken(t,DIV,0.0,NULL,nowLine);
            t.lexeme="/";
            break;
        }
        case COMMA:
        {
            setToken(t,COMMA,0.0,NULL,nowLine);
            t.lexeme=",";
            break;
        }
        case SEMICO:
        {
            setToken(t,SEMICO,0.0,NULL,nowLine);
            t.lexeme=";";
            break;
        }
        case L_BRACKET:
        {
            setToken(t,L_BRACKET,0.0,NULL,nowLine);
            t.lexeme="(";
            break;
        }
        case R_BRACKET:
        {
            setToken(t,R_BRACKET,0.0,NULL,nowLine);
            t.lexeme=")";
            break;
        }
        case POWER:
        {
            setToken(t,POWER,0.0,NULL,nowLine);
            t.lexeme="**";
            break;
        }
        case COMMENT:
        {
            setToken(t,COMMENT,0.0,NULL,nowLine-1);
            //遇到注释会在scan时就nowLine++，而注释在nowLine的上一行，所以要-1
            break;
        }
        case CONST_ID:
        {
            setToken(t,CONST_ID,stod(t.lexeme),NULL,nowLine);
            break;
        }
        case ID:
        {
            bool flag=false;
            for(int i=0;i<=28;i++)
            {
                if(!t.lexeme.compare(tokenTab[i].lexeme))
                {
                    flag=true;
                    t=tokenTab[i];
                    t.row=nowLine;
                    break;
                }
            }
            if(!flag) setToken(t,ERRTOKEN,0.0,NULL,nowLine);
            break;
        }
        default:
        {
            std::cout<<"getToken ERROR!"<<std::endl;
            break;
        }
    }
    return t;
}
