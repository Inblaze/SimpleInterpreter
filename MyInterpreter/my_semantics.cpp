#include <cstdlib>
#include <cmath>
#include "my_semantics.h"
#include "parser.h"

//原点x, y轴平移长度  横纵坐标比例  旋转弧度(逆时针)
double Origin_x = 0.0, Origin_y = 0.0, Scale_x = 1, Scale_y = 1, Rot_rad = 0.0;

bool drawer_init = false;
FILE *draw_py_file;

//py绘图文件初始化
void init_drawer()
{
    draw_py_file = fopen("./drawit.py", "w");
    if (!draw_py_file)
    {
        //err_exit("unable to create py file, draw failed.", nullptr, -EFAULT);
        printf("unable to create py.file, fail to draw...\n");
        exit(1);
    }

    drawer_init = true;

    fprintf(draw_py_file, "from tkinter import *\n");
    fprintf(draw_py_file, "tk = Tk()\n");
    fprintf(draw_py_file, "tk.title(\"Final Painting\")\n");
    fprintf(draw_py_file, "canvas = Canvas(tk, width = 1024, height = 500)\n");
    fprintf(draw_py_file, "canvas.pack()\n");
}

void close_drawer()
{
    if (draw_py_file)
    {
        fprintf(draw_py_file, "input()\n");
        fclose(draw_py_file);
    }
}

//单点绘制语句写入py文件
void DrawPixel(double x, double y, double step)
{
    if (!drawer_init)
        init_drawer();

    if (fabs(x) != NAN && fabs(x) != INFINITY && fabs(y) != NAN && fabs(y) != INFINITY)
        fprintf(draw_py_file, "canvas.create_oval(%lf, %lf, %lf, %lf)\n", x, y, x, y);
}

// 计算表达式的值,深度优先后序遍历语法树，bias为T参数当前的值
double GetExprValue(ExprNode_Ptr root, double bias) {
    if (root == NULL)
        return 0.0;
    switch (root -> opcode) {
        case PLUS :
            return GetExprValue(root -> CaseOperator.left, bias) + GetExprValue(root -> CaseOperator.right, bias);
        case MINUS:
            return GetExprValue(root -> CaseOperator.left, bias) - GetExprValue(root -> CaseOperator.right, bias);
        case MUL:
            return GetExprValue(root -> CaseOperator.left, bias) * GetExprValue(root -> CaseOperator.right, bias);
        case DIV:
            return GetExprValue(root -> CaseOperator.left, bias) / GetExprValue(root -> CaseOperator.right, bias);
        case POWER:
            return pow(GetExprValue(root -> CaseOperator.left, bias), GetExprValue(root -> CaseOperator.right, bias));
        // 调用指定的函数
        case FUNC:
            return  (root -> CaseFunc.funcPtr) (GetExprValue(root -> CaseFunc.child, bias));
        case CONST_ID:
            return root -> CaseConst;
        case T:
            return bias;
        default:
            return 0.0;

    }
}

// 计算点的坐标：获取坐标后进行变换
void CalcCoord(ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr, double * x_value, double * y_value, double bias) {
    double x_val, y_val, temp;
    // 初始坐标
    x_val = GetExprValue(x_ptr, bias);
    y_val = GetExprValue(y_ptr, bias);
    // 比例变换
    x_val *= Scale_x;
    y_val *= Scale_y;
    // 旋转变换
    temp = x_val * cos(Rot_rad) + y_val * sin(Rot_rad);
    y_val = y_val * cos(Rot_rad) - x_val * sin(Rot_rad);
    x_val = temp;
    // 平移变换
    x_val += Origin_x;
    y_val += Origin_y;
    // 返回变换后坐标
    if (NULL != x_value)
        *x_value = x_val;
    if (NULL != y_value)
        *y_value = y_val;
}

void setOrigin(ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr) {
    Origin_x = GetExprValue(x_ptr, 0);
    Origin_y = GetExprValue(y_ptr, 0);
}

void setScale(ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr) {
    Scale_x = GetExprValue(x_ptr, 0);
    Scale_y = GetExprValue(y_ptr, 0);
}

void setRot(ExprNode_Ptr angle_ptr) {
    Rot_rad = GetExprValue(angle_ptr, 0);
}

// 循环绘制点坐标
void DrawLoop(ExprNode_Ptr start_ptr, ExprNode_Ptr end_ptr, ExprNode_Ptr step_ptr, ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr) {
    double x_val, y_val;
    double start_val, end_val, step_val;
    //double * p_T_value = getT();

    if (!drawer_init)
        init_drawer();

    start_val = GetExprValue(start_ptr, 0);
    end_val = GetExprValue(end_ptr, 0);
    step_val = GetExprValue(step_ptr, 0);

    for(double i = start_val; i <= end_val; i += step_val) {
        CalcCoord(x_ptr, y_ptr, &x_val, &y_val, i);
        DrawPixel(x_val, y_val, step_val);
    }

}
