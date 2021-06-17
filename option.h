#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "minic_yacc.h"

//节点类型
enum node_kind  { EXT_DEF_LIST,EXT_VAR_DEF,FUNC_DEF,FUNC_DEC,EXT_DEC_LIST,PARAM_LIST,PARAM_DEC,VAR_DEF,DEC_LIST,DEF_LIST,COMP_STM,STM_LIST,EXP_STMT,IF_THEN,IF_THEN_ELSE, FUNC_CALL,ARGS, FUNCTION,PARAM,ARG,CALL,LABEL,GOTO,JLT,JLE,JGT,JGE,EQ,NEQ,Array,Array_Call,USELFPLUS,MSELFPLUS,USLEFMINUS,MSLEFMINUS};

#define MAXLENGTH   1000    //定义符号表,错误表的大小

//抽象语法树节点
struct node 
{
	enum node_kind kind;               //结点类型
	union 
    {
        char type_id[33];             //由标识符生成的叶结点
	    int type_int;                 //由整常数生成的叶结点
	    float type_float;              //由浮点常数生成的叶结点
        char type_char;                //由字符常量生成的叶节点
	};
    struct node *ptr[3];                   //子树指针
    int level;                           //层号，变量的层号
    int place;                //表示结点对应的变量或运算结果在符号表的位置序号
    char Etrue[15],Efalse[15];      //对布尔表达式的翻译时，真假转移目标的标号
    char Snext[15];               //该结点对应语句执行后的下一条语句位置标号
    char Bnext[15];               //break 语句下一条标号，也可判断break语句是否合法
    char Cnext[15];              //continue 语句下一条标号
    struct irnode *ir;         //该结点四元组链表头指针
    int  type;                   //结点对应值的类型,INT,CHAR,FLOAT
    int pos;                      //语法单位所在位置行号,用于报错
    int num;                      //用于储存变量个数
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
    char *rtype;               //RELOP类型
};

//四元式结点,采用双向循环链表存放中间语言代码
struct irnode 
{
    int  op;                          //运算符种类
    struct opn opn1,opn2,result;          //2个操作数和运算结果
    struct irnode  *next,*prior;   //双向链表，利于合并
};


//单条错误，包括行号，错误提示
struct err
{   
    int line;
    char *msg1;
    char *msg2; 
};

//错误表，收集所有错误，最后输出
struct errortable
{
    struct err errs[MAXLENGTH];
    int index;
}errorTable;


//函数声明
struct node *makeNode(int kind,struct node *first,struct node *second, struct node *third,int pos );//生成抽象语法树节点
void createAST(struct node *T, char * fkind);//生成.gv文件，用于可视化抽象语法树
char *stringConcatenate(char *s1,char *s2);//字符串连接，用于起名
char *newValueName();//返回新的变量名
char *newLabelName();//返回新的label名
char *newTempName();//返回新的临时变量名
struct irnode *getIR(int op,struct opn opn1,struct opn opn2,struct opn result);//生成普通四元式节点
struct irnode *getLabelIR(char *label);//生成label四元式节点
struct irnode *getGotoIR(char *label);//生成跳转四元式节点
struct irnode *mergeIR(int num,...);//连接四元式节点
void printIR(struct irnode *head);//输出四元式
void semanticError(int line,char *msg1,char *msg2);//记录语义错误
void printSymbol();//输出符号表
int searchSymbolTable(char *name);//按名字搜索符号表
int fillSymbolTable(char *name,char *alias,int level,int type,char flag) ;//将变量加入符号表
int fillTemp(char *name,int level,int type,char flag);//将临时变量加入符号表
void dealExtvarlist(struct node *T);//处理函数参数列表
int  matchParam(int i,struct node *T);//处理函数头
void boolExp(struct node *T);//处理bool表达式
void Exp(struct node *T);//处理表达式
void startSemanticAnalysis(struct node *T);//开始语义分析
void semanticAnalysis(struct node *T);//基于抽象语法树开始语义分析，生成符号表，错误表等
char* intToChar(int num,char* str,int radix);//int转换为字符串

//全局变量
char OPTION;//minic操作
FILE *fp;//用于储存抽象语法树生成的文件