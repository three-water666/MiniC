void display(struct node *T,char * fkind) //indent控制空格个数生成树形
{
    int i = 1;
    struct node *T0;
    char * a[100]={0};
    char * b[100]={0};
    char * c[100]={0};
    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s\n",a);
            fprintf(fp,"%s[label=\"%s\"]",a,"EXT_DEF_LIST");
            display(T->ptr[0],a);
            display(T->ptr[1],a); //显示该外部定义列表中的其它外部定义
            break;
        case EXT_VAR_DEF:
            // printf("%*c全局变量定义：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"EXT_VAR_DEF\"]\n",a);
            display(T->ptr[0],a); //显示全局变量类型
            display(T->ptr[1],a); //显示全局变量列表
            break;
        case TYPE:
            // printf("%*c类型： %s\n", indent, ' ', T->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"TYPE %s\"]\n",a,T->type_id);
            break;
        case EXT_DEC_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"EXT_DEC_LIST\"]\n",a);
            // fprintf(fp,"%s->EXT_DEC_LIST\n",fkind);
            display(T->ptr[0],a);     //依次显示全局变量名，
            display(T->ptr[1],a); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
            break;
        case FUNC_DEF:
            // printf("%*c函数定义：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_DEF\"]\n",a);
            // fprintf(fp,"%s->FUNC_DEF\n",fkind);
            display(T->ptr[0],a); //显示函数返回类型
            display(T->ptr[1],a); //显示函数名和参数
            display(T->ptr[2], "FUNC_DEF"); //显示函数体
            break;
        case FUNC_DEC:
            // printf("%*c函数名：%s\n", indent, ' ', T->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_DEC %s\"]\n",a,T->type_id);
            // fprintf(fp,"%s->FUNC_DEC_%s\n",fkind,T->type_id);
            if (T->ptr[0])
            {
                // printf("%*c函数形参：\n", indent, ' ');
                //参数暂时没管
                display(T->ptr[0],a); //显示函数参数列表
            }
            else
                // printf("%*c无参函数\n", indent + 4, ' ');
            break;
        case PARAM_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"PARAM_LIST\"]\n",a);
            // fprintf(fp,"%s->PARAM_LIST\n",fkind);
            display(T->ptr[0],a); //依次显示全部参数类型和名称，
            display(T->ptr[1],a);
            break;
        case PARAM_DEC:
            // printf("%*c类型：%s, 参数名：%s\n", indent, ' ',
            //        T->ptr[0]->type == INT ? "int" : T->ptr[0]->type == FLOAT ? "float"
            //                                                                  : "char",
            //        T->ptr[1]->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"PARAM_DEC\"]\n",a);
            // fprintf(fp,"%s->PARAM_DEC\n",fkind);

            strcpy(b,newNode());
            fprintf(fp,"%s->%s\n",a,b);
            fprintf(fp,"%s[label=\"%s\"]\n",T->ptr[0]->type == INT ? "int" : T->ptr[0]->type == FLOAT ? "float"
                                                                                                    : "char");
            // fprintf(fp,"PARAM_DEC->%s\n",T->ptr[0]->type == INT ? "int" : T->ptr[0]->type == FLOAT ? "float"
            //                                                                                         : "char");
                                                                    
                                                                
            strcpy(c,newNode());
            fprintf(fp,"%s->%s\n",a,c);
            fprintf(fp,"%s[label=\"%s\"]\n",T->ptr[1]->type_id);
            // fprintf(fp,"PARAM_DEC->%s\n",T->ptr[1]->type_id);
            break;
        case EXP_STMT:
            // printf("%*c表达式语句：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"EXP_STMT\"]\n",a);
            // fprintf(fp,"%s->EXP_STMT\n",fkind);
            display(T->ptr[0],a);
            break;
        case RETURN:
            // printf("%*c返回语句：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"RETURN\"]\n",a);
            // fprintf(fp,"%s->RETURN\n",fkind);
            display(T->ptr[0],a);
            break;
        case COMP_STM:
            // printf("%*c复合语句：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"COMP_STM\"]\n",a);
            // fprintf(fp,"%s->COMP_STM\n",fkind);
            // printf("%*c复合语句的变量定义：\n", indent + 4, ' ');
            display(T->ptr[0],a); //显示定义部分
            // printf("%*c复合语句的语句部分：\n", indent + 4, ' ');
            display(T->ptr[1],a); //显示语句部分
            break;
        case STM_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"STM_LIST\"]\n",a);
            // fprintf(fp,"%s->STM_LIST\n",fkind);
            display(T->ptr[0],a); //显示第一条语句
            display(T->ptr[1],a); //显示剩下语句
            break;
        case WHILE:
            // printf("%*cWHILE循环：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"WHILE\"]\n",a);
            // fprintf(fp,"%s->WHILE\n",fkind);
            // printf("%*c循环条件：\n", indent + 4, ' ');
            display(T->ptr[0],a); //显示循环条件
            // printf("%*c循环体：\n", indent + 4, ' ');
            display(T->ptr[1],a); //显示循环体
            break;
        case IF_THEN:
            // printf("%*cIF_THEN型条件语句：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"IF_THEN\"]\n",a);
            // fprintf(fp,"%s->IF_THEN\n",fkind);
            // printf("%*c条件：\n", indent + 4, ' ');
            display(T->ptr[0],a); //显示条件
            // printf("%*cIF子句：\n", indent + 4, ' ');
            display(T->ptr[1],a); //显示if子句
            break;
        case IF_THEN_ELSE:
            // printf("%*cIF_THEN_ELSE型条件语句：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"IF_THEN_ELSE\"]\n",a);
            // fprintf(fp,"%s->IF_THEN_ELSE\n",fkind);
            // printf("%*c条件：\n", indent + 4, ' ');
            display(T->ptr[0],a); //显示条件
            // printf("%*cIF子句：\n", indent + 4, ' ');
            display(T->ptr[1],a); //显示if子句
            // printf("%*cELSE子句：\n", indent + 4, ' ');
            display(T->ptr[2], "IF_THEN_ELSE"); //显示else子句
            break;
        case DEF_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"DEF_LIST\"]\n",a);
            // fprintf(fp,"%s->DEF_LIST\n",fkind);
            display(T->ptr[0],a); //显示该局部变量定义列表中的第一个
            display(T->ptr[1],a); //显示其它局部变量定义
            break;
        case VAR_DEF:
            // printf("%*c局部变量：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"VAR_DEF\"]\n",a);
            // fprintf(fp,"%s->VAR_DEF\n",fkind);
            display(T->ptr[0],a); //显示变量类型
            display(T->ptr[1],a); //显示该定义的全部变量名
            break;
        case DEC_LIST:
            // printf("%*c表达式：\n", indent, ' ');
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"DEC_LIST\"]\n",a);
            // fprintf(fp,"%s->DEC_LIST\n",fkind);
            T0 = T;
            while (T0)
            {
                if (T0->ptr[0]->kind == ID)
                {    // printf("%*c %s\n", indent + 4, ' ', T0->ptr[0]->type_id);
                    strcpy(b,newNode());
                    fprintf(fp,"%s->%s\n",a,b);
                    fprintf(fp,"%s[label=\"ID %s\"]\n",b,T0->ptr[0]->type_id);
                    // fprintf(fp,"DEC_LIST->ID_%s\n",T0->ptr[0]->type_id);
                }
                else if (T0->ptr[0]->kind == ASSIGNOP)
                {
                    // printf("%*c %s =\n ", indent + 4, ' ', T0->ptr[0]->ptr[0]->type_id);
                    strcpy(c,newNode());
                    fprintf(fp,"%s->%s\n",a,c);
                    fprintf(fp,"%s[label=\"ASSIGNOP %s\"]\n",c,T0->ptr[0]->type_id);
                    // fprintf(fp,"DEC_LIST->ASSIGNOP_%s\n",T0->ptr[0]->type_id);
                    //显示初始化表达式
                    //此处未完善
                    display(T->ptr[0],a);
                }
                else if (T0->ptr[0]->kind == Array)
                {
                    display(T->ptr[0],a);
                }
                T0 = T0->ptr[1];
            }
            break;
        case ID:
            // printf("%*c变量名： %s\n", indent, ' ', T->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"ID %s\"]\n",a,T->type_id);
            // fprintf(fp,"%s->ID_%s\n",fkind,T->type_id);
            break;
        case Array:
            // if (T->ptr[0] != NULL)
            // {
            //     printf("%*c数组名： %s\n", indent+4, ' ', T->ptr[0]->type_id);
            // }
            // if (T->ptr[1] != NULL)
            // {
            //     printf("%*c数组长度: %d\n", indent + 4, ' ', T->ptr[1]->type_int);
            // }
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"Array %s[%d]\"]\n",a,T->ptr[0]->type_id,T->ptr[1]->type_int);
            // fprintf(fp,"%s->Array_%s[%d]\n",fkind,T->ptr[0]->type_id,T->ptr[1]->type_int);
            break;
        case INT:
            // printf("%*c整型：%d\n", indent, ' ', T->type_int);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"INT %s[%d]\"]\n",a,T->type_int);
            // fprintf(fp,"%s->INT_%d\n",fkind,T->type_int);
            break;
        case FLOAT:
            // printf("%*c浮点型：%f\n", indent, ' ', T->type_float);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FLOAT %s[%d]\"]\n",a,T->type_float);
            // fprintf(fp,"%s->FLOAT_%f\n",fkind,T->type_float);
            break;
        case CHAR:
            // printf("%*c字符型 : %c\n", indent, ' ', T->type_char);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"CHAR %s[%d]\"]\n",a,T->type_char);
            // fprintf(fp,"%s->CHAR_%c\n",fkind,T->type_char);
            break;
        case ASSIGNOP:
        case AND:
        case OR:
        case RELOP:
        case PLUS:
        case PLUSASS:
        case SELFPLUS:
        case MINUS:
        case MINUSASS:
        case SLEFMINUS:
        case STAR:
        case DIV:
            // printf("%*c%s\n", indent, ' ', T->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"%s\"]\n",a,T->type_id);
            // fprintf(fp,"%s->%s\n",fkind,T->type_id);
            display(T->ptr[0],a);
            display(T->ptr[1],a);
            break;
        case NOT:
        case UMINUS:
            // printf("%*c%s\n", indent, ' ', T->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"%s\"]\n",a,T->type_id);
            // fprintf(fp,"%s->%s\n",fkind,T->type_id);
            display(T->ptr[0],a);
            break;
        case FUNC_CALL:
            // printf("%*c函数调用：\n", indent, ' ');
            // printf("%*c函数名：%s\n", indent + 4, ' ', T->type_id);
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_CALL %s\"]\n",a,T->type_id);
            // fprintf(fp,"%s->FUNC_CALL_%s\n",fkind,T->type_id);
            display(T->ptr[0],a);
            break;
        case ARGS:
            i = 1;
            while (T)
            { //ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的。
                struct node *T0 = T->ptr[0];
                // printf("%*c第%d个实际参数表达式：\n", indent, ' ', i++);
                strcpy(a,newNode());
                fprintf(fp,"%s->%s\n",fkind,a);
                fprintf(fp,"%s[label=\"ARGS\"]\n",a);
                // fprintf(fp,"%s->ARGS\n",fkind,T->type_id);
                display(T->ptr[0],a);
                T = T->ptr[1];
            }
            //printf("%*c第%d个实际参数表达式：\n",indent,' ',i);
            //display(T,indent+4);
            // printf("\n");
            break;
        }
    }
}