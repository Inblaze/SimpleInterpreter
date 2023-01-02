#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "parser.h"

extern double Scale_x, Scale_y, Origin_x, Origin_y, Rot_rad;
extern double GetExprValue(ExprNode_Ptr root, double bias);
extern void close_drawer();
//设置原点x、y轴的偏移位置，嵌入在origin_statement末尾
extern void setOrigin(ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr);
//设置绘制图像的横纵缩放比例，嵌入在scale_statement末尾
extern void setScale(ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr);
//设置绘制图像的逆时针旋转角度，嵌入在rot_statement末尾
extern void setRot(ExprNode_Ptr angle_ptr);
//for-draw语句绘图，嵌入在for_statement末尾
extern void DrawLoop(ExprNode_Ptr start_ptr, ExprNode_Ptr end_ptr, ExprNode_Ptr step_ptr, ExprNode_Ptr x_ptr, ExprNode_Ptr y_ptr);

#endif
