#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "minic_yacc.h"

enum node_kind  { EXT_DEF_LIST,EXT_VAR_DEF,FUNC_DEF,FUNC_DEC,EXT_DEC_LIST,PARAM_LIST,PARAM_DEC,VAR_DEF,DEC_LIST,DEF_LIST,COMP_STM,STM_LIST,EXP_STMT,IF_THEN,IF_THEN_ELSE, FUNC_CALL,ARGS, FUNCTION,PARAM,ARG,CALL,LABEL,GOTO,JLT,JLE,JGT,JGE,EQ,NEQ,Array,Array_Call,USELFPLUS,MSELFPLUS,USLEFMINUS,MSLEFMINUS};

#define MAXLENGTH   1000    //定义符号表的大小

//操作数和结果节点
struct opn
{
    int kind;                  //标识操作的类型
    int type;                  //标识操作数的类型
    union 
    {
        int     const_int;      //整常数值，立即数
        float   const_float;    //浮点常数值，立即数
        char    const_char;    //字符常数值，立即数
        char    id[33];        //变量或临时变量的别名或标号字符串
    };
    int level;                 //变量的层号，0表示是全局变量，数据保存在静态数据区
    char *rtype;
};


//四元式结点,采用双向循环链表存放中间语言代码
struct codenode 
{
    int  op;                          //运算符种类
    struct opn opn1,opn2,result;          //2个操作数和运算结果
    struct codenode  *next,*prior;
};

struct node 
{    //以下对结点属性定义没有考虑存储效率，只是简单地列出要用到的一些属性
	enum node_kind kind;               //结点类型
	union 
    {
        char type_id[33];             //由标识符生成的叶结点
	    int type_int;                 //由整常数生成的叶结点
	    float type_float;              //由浮点常数生成的叶结点
        char type_char;                //由字符常量生成的叶节点
	};
    struct node *ptr[3];                   //子树指针，由kind确定有多少棵子树
    int level;                           //层号
    int place;                //表示结点对应的变量或运算结果符号表的位置序号
    char Etrue[15],Efalse[15];      //对布尔表达式的翻译时，真假转移目标的标号
    char Snext[15];               //该结点对应语句执行后的下一条语句位置标号
    char Bnext[15];               //break 语句下一条标号，也可判断break语句是否合法
    char Cnext[15];              //continue 语句下一条标号
    struct codenode *code;         //该结点中间代码链表头指针
    char op[10];
    int  type;                   //结点对应值的类型
    int pos;                      //语法单位所在位置行号
    int num;                      //变量个数
};

//一个符号表项的部分属性
struct symbol 
{
    char name[33];     //变量或函数名
    int level;   //层号，外部变量名或函数名层号为0，形参名为1，每到1个复合语句层号加1，退出减1
    int type;           //变量类型或函数返回值类型
    int  paramnum;    //形式参数个数
    char alias[10];      //别名，为解决嵌套层次使用，使得每一个数据名称唯一
    char flag;          //符号标记，函数定义：'F'  变量：'V'   参数：'P'  临时变量：'T' 函数声明：'D'
    int listLength[10];  //数组长度
    int isArray;//是不是数组
};

//符号表，是一个顺序栈，index初值为0
struct symboltable
{
    struct symbol symbols[MAXLENGTH];
    int index;
} symbolTable;


//当前作用域的符号在符号表的起始位置序号
struct symbol_scope_begin 
{
    int TX[30];
    int top;
} symbol_scope_TX;


struct err
{   
    int line;
    char *msg1;
    char *msg2; 
};

struct errortable
{
    struct err errs[MAXLENGTH];
    int index;
}errorTable;

struct node *mknode(int kind,struct node *first,struct node *second, struct node *third,int pos );
void display(struct node *T, char * fkind);
char *strcat0(char *s1,char *s2);
char *newAlias();
char *newLabel();
char *newTemp();
struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result);
struct codenode *genLabel(char *label);
struct codenode *genGoto(char *label);
struct codenode *merge(int num,...);
void prnIR(struct codenode *head);
void semantic_error(int line,char *msg1,char *msg2);
void prn_symbol();
int searchSymbolTable(char *name);
int fillSymbolTable(char *name,char *alias,int level,int type,char flag) ;
int fill_Temp(char *name,int level,int type,char flag);
void ext_var_list(struct node *T);
int  match_param(int i,struct node *T);
void boolExp(struct node *T);
void Exp(struct node *T);
void semantic_Analysis0(struct node *T);
void semantic_Analysis(struct node *T);
char* itoa(int num,char* str,int radix);
char OPTION;

FILE *fp;