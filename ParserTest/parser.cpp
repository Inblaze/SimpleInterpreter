#include"parser.h"

int indent;


struct t_parser_stuff
{
    int indent;//��ʾ��Ϣ��������
    ExprNode_Ptr start_ptr,     //��ͼ������ʽ���﷨��
                end_ptr,        //��ͼ������ʽ���﷨��
                step_ptr,       //��������ʽ���﷨��
                x_ptr,          //��ĺ��������ʽ���﷨��
                y_ptr,          //������������ʽ���﷨��
                angle_ptr;      //��ת�Ƕȱ���ʽ���﷨��
    struct token  current_token;    //��ǰ�Ǻ�

    double parameter;           //����T�Ĵ洢�ռ�
};

double parameter;//����
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
    case CONST_ID://�����ڵ�
        root->CaseConst = left->CaseConst;
        break;
    case T://�����ڵ�
        root->CaseParmPtr = &parameter;
        break;
    case FUNC://�������ýڵ�
        root->CaseFunc.funcPtr = left->CaseFunc.funcPtr;
        root->CaseFunc.child = right;
        break;
    default://��Ԫ�����
        root->CaseOperator.left = (struct ExprNode*)malloc(sizeof(struct ExprNode));
        root->CaseOperator.right = (struct ExprNode*)malloc(sizeof(struct ExprNode));
        root->CaseOperator.left = left;
        root->CaseOperator.right = right;
        break;
    }
    return root;
};

void InitNode(ExprNode_Ptr Node)//�ڵ��ʼ��
{
    Node->opcode = NONINIT;
    Node->CaseFunc.child = NULL;
    Node->CaseOperator.left = NULL;
    Node->CaseOperator.right = NULL;

}

int InitParser()//�﷨��������ʼ��
{
    parser_data.indent = 0;

    parser_data.parameter = 0;
    parser_data.start_ptr = NULL;
    parser_data.end_ptr = NULL;
    parser_data.start_ptr = NULL;
    parser_data.x_ptr = NULL;
    parser_data.y_ptr = NULL;
    parser_data.angle_ptr = NULL;

    initScanner();//�������Զ��˳�����

    return 1;
}

void CloseParser()
{
    closeScanner();
    destroy_all_tree();     //�ͷſռ�
}

void Parser()
{
    if(InitParser() == 0)
        return;

    FetchToken();
    program();

    CloseParser();
}

void FetchToken()//��ȡһ���Ǻ�
{
    current_token = getToken();
    if(current_token.type == ERRTOKEN) {
        std::cout<<"Error token "<<'"'<<current_token.lexeme<<'"'<<" at line "<<current_token.row<<"!"<<endl;
        system("pause");
        exit(1);
    }
    //print_token();
}

void MatchToken(tokenType expected)
{
    if(current_token.type != expected)
    {
        std::cout<<"Unexpected token "<<'"'<<current_token.lexeme<<'"'<<" at line "<<current_token.row<<"!"<<endl;
        //printf("����Ԥ�ڷ���\n");
        system("pause");
        exit(1);
    }
    LogPrint("MatchToken\t"+current_token.lexeme+"\n",0);
    FetchToken();   //������ȡ��һ�Ǻ�
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
    indent = 0;//����������ʼ��
    LogPrint("enter in program\n", 1);

    while(current_token.type != NONTOKEN)//�������
    {
        statement();
        MatchToken(SEMICO);
    }

    print_all_tree();
    indent--;
    LogPrint("exit from program",0);
}

void statement()
{
    LogPrint("enter in statement\n", 1);

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
        std::cout<<"Unexpected token "<<'"'<<current_token.lexeme<<'"'<<" at line "<<current_token.row<<"!"<<endl;
        //printf("����Ԥ�ڼǺ�\n");
        system("pause");
        exit(1);
    }
    LogPrint("exit from statement\n", -1);
    destroy_all_tree();
}

void origin_statement()
{
    LogPrint("enter in origin_statement\n", 1);
    MatchToken(ORIGIN);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    parser_data.x_ptr = expression();
    PrintSyntaxTree(parser_data.x_ptr,1);
    MatchToken(COMMA);
    parser_data.y_ptr = expression();
    PrintSyntaxTree(parser_data.y_ptr,1);
    MatchToken(R_BRACKET);

    LogPrint("exit from origin_statement\n", -1);
    //setOrigin(parser_data.x_ptr, parser_data.y_ptr);

}

void scale_statement()
{
    LogPrint("enter in scale_statement\n", 1);

    MatchToken(SCALE);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    parser_data.x_ptr = expression();
    PrintSyntaxTree(parser_data.x_ptr,1);
    MatchToken(COMMA);
    parser_data.y_ptr = expression();
    PrintSyntaxTree(parser_data.y_ptr,1);
    MatchToken(R_BRACKET);

    //setScale(parser_data.x_ptr, parser_data.y_ptr);

    LogPrint("exit from scale_statement\n", -1);
}

void rot_statement()
{
    LogPrint("enter in rot_statement\n", 1);

    MatchToken(ROT);
    MatchToken(IS);
    parser_data.angle_ptr = expression();
    PrintSyntaxTree(parser_data.angle_ptr,1);
    //setRot(parser_data.angle_ptr);

    LogPrint("exit from rot_statement\n", -1);
}

void for_statement()
{
    LogPrint("enter from for_statement\n", 1);

    MatchToken(FOR);
    MatchToken(T);
    MatchToken(FROM);
    parser_data.start_ptr = expression();
    PrintSyntaxTree(parser_data.start_ptr,1);
    MatchToken(TO);
    parser_data.end_ptr = expression();
    PrintSyntaxTree(parser_data.end_ptr,1);
    MatchToken(STEP);
    parser_data.step_ptr = expression();
    PrintSyntaxTree(parser_data.step_ptr,1);
    MatchToken(DRAW);
    MatchToken(L_BRACKET);
    parser_data.x_ptr = expression();
    PrintSyntaxTree(parser_data.x_ptr,1);
    MatchToken(COMMA);
    parser_data.y_ptr = expression();
    PrintSyntaxTree(parser_data.y_ptr,1);
    MatchToken(R_BRACKET);

    LogPrint("exit from for_statement\n", -1);
    //DrawLoop(parser_data.start_ptr, parser_data.end_ptr, parser_data.step_ptr, parser_data.x_ptr, parser_data.y_ptr);

}

ExprNode_Ptr expression()
{
    LogPrint("enter in expression\n", 0);

    ExprNode_Ptr left,right;        //���������ڵ�ָ��
    tokenType lastType;

    left = term();                  //������������ҵõ����﷨��
    while(current_token.type == PLUS || current_token.type==MINUS)
    {
        lastType = current_token.type;
        MatchToken(lastType);
        right = term();             //�����Ҳ��������õ����﷨��
        left = MakeExprNode(lastType,left,right);   //����������﷨�������Ϊ������
    }

    LogPrint("exit from expression\n", 0);

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
        left = MakeExprNode(lastType,left,right);           //��leftָ����ڵ�
    }
    return left;
}

ExprNode_Ptr factor()
{
    ExprNode_Ptr left,right;

    if(current_token.type == PLUS)          //ƥ��һԪ������
    {
        MatchToken(PLUS);
        right = factor();               //����ʽ�˻�Ϊֻ���Ҳ������ı���ʽ
    }
    else if(current_token.type == MINUS)    //ƥ��һԪ������
    {
        MatchToken(MINUS);
        right = factor();
        left = (ExprNode_Ptr)malloc(sizeof(ExprNode));
        left->opcode = CONST_ID;
        left->CaseConst = 0.0;
        right = MakeExprNode(MINUS,left,right);             //��leftָ����ڵ�
    }
    else right = component();               //ƥ����ս��component
    return right;
}

ExprNode_Ptr component()
{
    ExprNode_Ptr left,right;

    left = atom();
    if(current_token.type == POWER)
    {
        MatchToken(POWER);
        right = component();                                //�ݹ����component��ʵ��POWER���ҽ������
        left = MakeExprNode(POWER,left,right);              //��leftָ����ڵ�
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
    case CONST_ID://����
        left->CaseConst = current_token.value;
        left->opcode = current_token.type;
        MatchToken(CONST_ID);
        ptr = MakeExprNode(CONST_ID,left,NULL);
        break;
    case T://����
        MatchToken(T);
        ptr = MakeExprNode(T,NULL,NULL);
        break;
    case FUNC://����
        MatchToken(FUNC);
        MatchToken(L_BRACKET);
        tmp = expression();
        left->CaseFunc.funcPtr = t.funcPtr;
        ptr = MakeExprNode(FUNC,left,tmp);
        MatchToken(R_BRACKET);
        break;
    case L_BRACKET://������            //(expression)
        MatchToken(L_BRACKET);
        ptr = expression();
        MatchToken(R_BRACKET);
        break;
    default:
        std::cout<<"Unexpected token "<<'"'<<current_token.lexeme<<'"'<<" at line "<<current_token.row<<"!"<<endl;
        system("pause");
        exit(1);
    }
    return ptr;
}

void print_all_tree()
{
    PrintSyntaxTree(parser_data.start_ptr,0);
    PrintSyntaxTree(parser_data.end_ptr,0);
    PrintSyntaxTree(parser_data.start_ptr,0);
    PrintSyntaxTree(parser_data.x_ptr,0);
    PrintSyntaxTree(parser_data.y_ptr,0);
    PrintSyntaxTree(parser_data.angle_ptr,0);
}

void PrintSyntaxTree(ExprNode_Ptr root,int choose)//��ӡ�﷨��
{
    if(root == NULL) return;
    switch(root->opcode)
    {
    case PLUS:
        LogPrint("+\n",1);
        break;
    case MINUS:
        LogPrint("-\n",1);
        break;
    case MUL:
        LogPrint("*\n",1);
        break;
    case DIV:
        LogPrint("/\n",1);
        break;
    case POWER:
        LogPrint("**\n",1);
        break;
    case T:
        LogPrint("T\n",-1);
        return;
    case FUNC:
        for (int i = 0; i < indent; i++)
            printf(" ");
        printf("%p\n",root->CaseFunc.funcPtr);
        indent++;
        PrintSyntaxTree(root->CaseFunc.child,-1);
        return;
    case CONST_ID:
        for (int i = 0; i < indent; i++)
            printf(" ");
        printf("%lf\n",root->CaseConst);
        if(choose==-1)
            indent--;
        return;
    default:
        printf("�Ƿ������ڵ�\n");
        return;
    }

    //ֻ�в������ڵ���ߵ�����
    //�ݹ��ӡ�������ӵĽڵ�
    PrintSyntaxTree(root->CaseOperator.left,1);;
    PrintSyntaxTree(root->CaseOperator.right,0);
    indent--;
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

void LogPrint(std::string content, int choose)
{
    for (int i = 0; i < indent; i++)
        printf(" ");
    std::cout << content;
    switch (choose)
    {
    case 1:
        indent++;
        break;
    case -1:
        indent--;
        break;
    }
}
