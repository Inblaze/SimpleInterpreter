#include"parser.h"
#include "my_semantics.h"



struct t_parser_stuff
{
    int indent;//显示信息的缩进量
    ExprNode_Ptr start_ptr,     //绘图起点表达式的语法树
                end_ptr,        //绘图起点表达式的语法树
                step_ptr,       //步长表达式的语法树
                x_ptr,          //点的横坐标表达式的语法树
                y_ptr,          //点的纵坐标表达式的语法树
                angle_ptr;      //旋转角度表达式的语法树
    struct token  current_token;    //当前记号

    double parameter;           //参数T的存储空间
};

double parameter;//参数
struct token current_token;
struct token last_token;

static struct t_parser_stuff parser_data;

double * getT(){ return &(parser_data.parameter); }

struct ExprNode * MakeExprNode(tokenType opcode,ExprNode_Ptr left,ExprNode_Ptr right)
{
    struct ExprNode* root = (struct ExprNode *)malloc(sizeof(struct ExprNode));
    InitNode(root);
    root->opcode = opcode;
    switch(opcode)
    {
    case CONST_ID://常数节点
        root->CaseConst = left->CaseConst;
        break;
    case T://参数节点
        root->CaseParmPtr = &parameter;
        break;
    case FUNC://函数调用节点
        root->CaseFunc.funcPtr = left->CaseFunc.funcPtr;
        root->CaseFunc.child = right;
        break;
    default://二元运算符
        root->CaseOperator.left = (struct ExprNode*)malloc(sizeof(struct ExprNode));
        root->CaseOperator.right = (struct ExprNode*)malloc(sizeof(struct ExprNode));
        root->CaseOperator.left = left;
        root->CaseOperator.right = right;
        break;
    }
    return root;
};

void InitNode(ExprNode_Ptr Node)//节点初始化
{
    Node->opcode = NONINIT;
    Node->CaseFunc.child = NULL;
    Node->CaseOperator.left = NULL;
    Node->CaseOperator.right = NULL;

}

void InitParser()//语法分析器初始化
{
    parser_data.indent = 0;

    parser_data.parameter = 0;
    parser_data.start_ptr = NULL;
    parser_data.end_ptr = NULL;
    parser_data.start_ptr = NULL;
    parser_data.x_ptr = NULL;
    parser_data.y_ptr = NULL;
    parser_data.angle_ptr = NULL;

    initScanner();//出错会自动退出程序
    return;
}

void CloseParser()
{
    closeScanner();
    destroy_all_tree();     //释放空间
}

void Parser()
{
    InitParser();

    FetchToken();
    program();

    CloseParser();
}

void FetchToken()//获取一个记号
{
    current_token = getToken();
    if(current_token.type == ERRTOKEN) {
        printf("非法单词\n");
        exit(1);
    }
    //print_token();
}

void MatchToken(tokenType expected)
{
    if(current_token.type != expected)
    {
        printf("不是预期符号\n");
        exit(1);
    }
    FetchToken();   //继续读取下一记号
    while(current_token.type == COMMENT) {
        current_token = getToken();
    }
}

void print_token()
{
    //cout<<"type:"<<current_token.type<<"\t"<<"value:"<<current_token.value<<endl;
}

void program()
{
    //printf("program enter\n");
    while(current_token.type != NONTOKEN)//输入结束
    {
        statement();
        MatchToken(SEMICO);
    }

    print_all_tree();
    //printf("program back\n");
}

void statement()
{
    //printf("statement enter\n");
    switch(current_token.type)
    {
    case ORIGIN:
        origin_statement();
        break;
    case SCALE:
        scale_statement();
        break;
    case ROT:
        rot_statement();
        break;
    case FOR:
        for_statement();
        break;
    default:
        printf("不是预期记号\n");
        exit(1);
    }
    destroy_all_tree();
}

void origin_statement()
{
    MatchToken(ORIGIN);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    parser_data.x_ptr = expression();

    MatchToken(COMMA);
    parser_data.y_ptr = expression();

    MatchToken(R_BRACKET);

    setOrigin(parser_data.x_ptr, parser_data.y_ptr);

}

void scale_statement()
{
    MatchToken(SCALE);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    parser_data.x_ptr = expression();
    MatchToken(COMMA);
    parser_data.y_ptr = expression();
    MatchToken(R_BRACKET);

    setScale(parser_data.x_ptr, parser_data.y_ptr);


}

void rot_statement()
{


    MatchToken(ROT);
    MatchToken(IS);
    parser_data.angle_ptr = expression();

    setRot(parser_data.angle_ptr);


}

void for_statement()
{
    MatchToken(FOR);
    MatchToken(T);
    MatchToken(FROM);
    parser_data.start_ptr = expression();
    MatchToken(TO);
    parser_data.end_ptr = expression();
    MatchToken(STEP);
    parser_data.step_ptr = expression();
    MatchToken(DRAW);
    MatchToken(L_BRACKET);
    parser_data.x_ptr = expression();
    MatchToken(COMMA);
    parser_data.y_ptr = expression();
    MatchToken(R_BRACKET);


    DrawLoop(parser_data.start_ptr, parser_data.end_ptr, parser_data.step_ptr, parser_data.x_ptr, parser_data.y_ptr);



}

ExprNode_Ptr expression()
{
    ExprNode_Ptr left,right;        //左右子树节点指针
    tokenType lastType;

    left = term();                  //分析左操作数且得到其语法树
    while(current_token.type == PLUS || current_token.type==MINUS)
    {
        lastType = current_token.type;
        MatchToken(lastType);
        right = term();             //分析右操作数并得到其语法树
        left = MakeExprNode(lastType,left,right);   //构造运算的语法树，结果为左子树
    }

    return left;
}

ExprNode_Ptr term()
{
    ExprNode_Ptr left,right;
    tokenType lastType;

    left = factor();
    while(current_token.type == MUL || current_token.type == DIV)
    {
        lastType = current_token.type;
        MatchToken(lastType);
        right = factor();
        left = MakeExprNode(lastType,left,right);           //让left指向根节点
    }
    return left;
}

ExprNode_Ptr factor()
{
    ExprNode_Ptr left,right;

    if(current_token.type == PLUS)         //匹配一元加运算
    {
        MatchToken(PLUS);
        right = factor();               //表达式退化为只有右操作数的表达式
    }
    else if(current_token.type == MINUS)    //匹配一元减运算
    {
        MatchToken(MINUS);
        right = factor();
        left = (ExprNode_Ptr)malloc(sizeof(ExprNode));
        left->opcode = CONST_ID;
        left->CaseConst = 0.0;
        right = MakeExprNode(MINUS,left,right);             //让left指向根节点
    }
    else right = component();               //匹配非终结符component
    return right;
}

ExprNode_Ptr component()
{
    ExprNode_Ptr left,right;

    left = atom();
    if(current_token.type == POWER)
    {
        MatchToken(POWER);
        right = component();                               //递归调用component以实现POWER的右结合性质
        left = MakeExprNode(POWER,left,right);              //让left指向根节点
    }
    return left;
}

ExprNode_Ptr atom()
{
    struct token t = current_token;
    ExprNode_Ptr ptr,tmp;
    ExprNode_Ptr left = (struct ExprNode*)malloc(sizeof(struct ExprNode));

    switch(current_token.type)
    {
    case CONST_ID://常量
        left->CaseConst = current_token.value;
        left->opcode = current_token.type;
        MatchToken(CONST_ID);
        ptr = MakeExprNode(CONST_ID,left,NULL);
        break;
    case T://参数
        MatchToken(T);
        ptr = MakeExprNode(T,NULL,NULL);
        break;
    case FUNC://函数
        MatchToken(FUNC);
        MatchToken(L_BRACKET);
        tmp = expression();
        left->CaseFunc.funcPtr = t.funcPtr;
        ptr = MakeExprNode(FUNC,left,tmp);
        MatchToken(R_BRACKET);
        break;
    case L_BRACKET://左括号              //(expression)
        MatchToken(L_BRACKET);
        ptr = expression();
        MatchToken(R_BRACKET);
        break;
    default:
        printf("不是预期记号\n");
        exit(1);
    }
    return ptr;
}

void print_all_tree()
{
    PrintSyntaxTree(parser_data.start_ptr);
    PrintSyntaxTree(parser_data.end_ptr);
    PrintSyntaxTree(parser_data.start_ptr);
    PrintSyntaxTree(parser_data.x_ptr);
    PrintSyntaxTree(parser_data.y_ptr);
    PrintSyntaxTree(parser_data.angle_ptr);
}

void PrintSyntaxTree(ExprNode_Ptr root)//打印语法树
{

    if(root == NULL) return;
    switch(root->opcode)
    {
    case PLUS:
        printf("+\n");
        break;
    case MINUS:
        printf("-\n");
        break;
    case MUL:
        printf("*\n");
        break;
    case DIV:
        printf("/\n");
        break;
    case POWER:
        printf("**\n");
        break;
    case T:
        printf("T\n");
        return;
    case FUNC:
        printf("%p\n",root->CaseFunc.funcPtr);
        PrintSyntaxTree(root->CaseFunc.child);
        return;
    case CONST_ID:
        printf("%lf\t",root->CaseConst);
        return;
    default:
        printf("非法的树节点\n");
        return;
    }

    //只有操作符节点会走到这里
    //递归打印两个孩子的节点
    PrintSyntaxTree(root->CaseOperator.left);
    PrintSyntaxTree(root->CaseOperator.right);
}

void destroy_all_tree()
{
    destroy_tree(parser_data.start_ptr);parser_data.start_ptr = NULL;
    destroy_tree(parser_data.end_ptr);parser_data.end_ptr = NULL;
    destroy_tree(parser_data.step_ptr);parser_data.step_ptr = NULL;
    destroy_tree(parser_data.x_ptr);parser_data.x_ptr = NULL;
    destroy_tree(parser_data.y_ptr);parser_data.y_ptr = NULL;
    destroy_tree(parser_data.angle_ptr);parser_data.angle_ptr = NULL;
}

void destroy_tree(ExprNode_Ptr root)
{
    if(root == NULL) return;
    switch( root->opcode )
    {
    case PLUS:
    case MINUS:
    case MUL:
    case DIV:
    case POWER:
        destroy_tree(root->CaseOperator.left);
        destroy_tree(root->CaseOperator.right);
        break;
    case FUNC:
        destroy_tree(root->CaseFunc.child);
        break;
    default:
        break;
    }
    free(root);
    return;
}

