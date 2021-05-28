%error-verbose
%locations
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "option.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
void display(struct node *,int);
int TypeCheck(char* type);
%}

%union {
	int    type_int;
	float  type_float;
        char type_char;
	char   type_id[32];
	struct node *ptr;
};


//%type 定义【非终结符】的语义值类型
%type  <ptr> program ExtDefList  ExtDef        Specifier ExtDecList     FuncDec             CompSt   VarList  VarDec   
//           开始符  外部定义列表 外部函数与变量 类型标识符 外部变量名列表 函数变量名和参数表列  复合语句 参数列表  变量名或函数名
//

%type <ptr>  ParamDec  Stmt     StmList       DefList       Def          DecList              Dec                Exp         Args            List_INT 
//           形参       单条语句  语句列表      定义语句列表  定义语句      单条语句定义多个变量  定义变量名和初始化   运算表达式  调用时参数列表   辅助数组支持
//

/*---------------------1.Tokens---------------------*/
//% token 定义【终结符】的语义值类型
%token <type_int> INT              //指定INT的语义值是type_int，有词法分析得到的数值  
//                整数
%token <type_id> ID RELOP TYPE  //指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串 
//           变量名  >,<,==...  int,float,char   
%token <type_float> FLOAT         //指定ID的语义值是type_id，有词法分析得到的标识符字符串 
//                  浮点数
%token <type_char> CHAR   
//                 字符串
%token LP RP LC RC LB RB SEMI COMMA   //用bison对该文件编译时，带参数-d，生成的exp.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码
//     (  )  {  }  [  ]    ;    ,  
%token PLUS PLUSASS MINUS MINUSASS STAR DIV ASSIGNOP AND OR NOT IF ELSE WHILE RETURN SELFPLUS SLEFMINUS FOR
//      +     +=      -     -=      *    /    =       &&  || !  if  else  while  return  ++      --     for  

%left ASSIGNOP
%left OR
%left AND
%left RELOP
%left SELFPLUS SLEFMINUS
%left MINUSASS PLUSASS
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT 

%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%
/*---------------------2.High-level Definitions---------------------*/
/*初始语法单元，表示整个程序*/
program: ExtDefList{semantic_Analysis0($1);}; 

/*零个或多个ExtDef*/
ExtDefList: {$$=NULL;} 
        | ExtDef ExtDefList {$$=mknode(EXT_DEF_LIST,$1,$2,NULL,yylineno);}
        ; 

/*表示全局变量，结构体，函数定义*/
ExtDef: Specifier ExtDecList SEMI {$$=mknode(EXT_VAR_DEF,$1,$2,NULL,yylineno);}   //全局变量
        //|Specifier SEMI {} //结构体当前未实现
        |Specifier FuncDec CompSt {$$=mknode(FUNC_DEF,$1,$2,$3,yylineno);}         //函数定义
        | error SEMI   {$$=NULL; }
        ;
 
/*零个或多个变量的定义VarDec*/
ExtDecList: VarDec {$$=$1;} 
        | VarDec COMMA ExtDecList {$$=mknode(EXT_DEC_LIST,$1,$3,NULL,yylineno);}
        ; 


/*---------------------3.Specifiers---------------------*/
/*类型描述符,如int float char*/
Specifier: TYPE {$$=mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);$$->type=TypeCheck($1);} 
        //|结构体
        ;    


/*---------------------4.Declarators---------------------*/
/*对一个变量的定义*/ 
VarDec:  ID {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}   //标识符，如a
        | VarDec LB List_INT RB        {$$=mknode(Array,$1,$3,NULL,yylineno);} //数组，如a[10]
        ;

/*用于辅助数组文法*/
List_INT: INT {$$=mknode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}
        ;

/*函数头的定义*/
FuncDec: ID LP VarList RP {$$=mknode(FUNC_DEC,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//含参数函数
        |ID LP  RP   {$$=mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//无参数函数
        ;  

/*参数列表*/
VarList: ParamDec  {$$=mknode(PARAM_LIST,$1,NULL,NULL,yylineno);}//一个形参的定义
        | ParamDec COMMA  VarList  {$$=mknode(PARAM_LIST,$1,$3,NULL,yylineno);}
        ;

/*一个形参的定义*/
ParamDec: Specifier VarDec         {$$=mknode(PARAM_DEC,$1,$2,NULL,yylineno);}
        ;



/*---------------------5.Statements---------------------*/
/*花括号括起来的语句块*/
CompSt: LC DefList StmList RC    {$$=mknode(COMP_STM,$2,$3,NULL,yylineno);}
        ;

/*一系列语句列表*/
StmList: {$$=NULL; }  
        | Stmt StmList  {$$=mknode(STM_LIST,$1,$2,NULL,yylineno);}
        ;

/*单条语句*/
Stmt: Exp SEMI    {$$=mknode(EXP_STMT,$1,NULL,NULL,yylineno);}//一条表达式
      | CompSt      {$$=$1;}      //另一个语句块
      | RETURN Exp SEMI   {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}//返回语句
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);} //if语句
      | IF LP Exp RP Stmt ELSE Stmt   {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}//if-else 语句
      | WHILE LP Exp RP Stmt {$$=mknode(WHILE,$3,$5,NULL,yylineno);}//while 语句
      ;

/*---------------------6.Local Definitions---------------------*/
/*一系列变量定义，如int a;float b;int d[10]*/
DefList: {$$=NULL; }
        | Def DefList {$$=mknode(DEF_LIST,$1,$2,NULL,yylineno);}
        ;

/*一个变量定义*/
Def: Specifier DecList SEMI {$$=mknode(VAR_DEF,$1,$2,NULL,yylineno);}
     ;
     
/*如int a,b,c*/
DecList: Dec  {$$=mknode(DEC_LIST,$1,NULL,NULL,yylineno);}
        | Dec COMMA DecList  {$$=mknode(DEC_LIST,$1,$3,NULL,yylineno);}
	;

/*变量名或者带初始化*/
Dec:   VarDec {$$=$1;}
       | VarDec ASSIGNOP Exp  {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
       ;



/*---------------------7.Expressions---------------------*/
/*运算表达式*/
Exp:  //二元运算
        Exp ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}//二元运算
      | Exp AND Exp   {$$=mknode(AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"AND");}//逻辑与
      | Exp OR Exp    {$$=mknode(OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"OR");}//逻辑或
      | Exp RELOP Exp {$$=mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}//关系表达式
      | Exp PLUS Exp  {$$=mknode(PLUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUS");}//四则运算
      | Exp MINUS Exp {$$=mknode(MINUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"MINUS");}
      | Exp STAR Exp  {$$=mknode(STAR,$1,$3,NULL,yylineno);strcpy($$->type_id,"STAR");}
      | Exp DIV Exp   {$$=mknode(DIV,$1,$3,NULL,yylineno);strcpy($$->type_id,"DIV");}
      //-------额外实现-------
      | Exp PLUSASS Exp  {$$=mknode(PLUSASS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUSASS");} //复合加
      | Exp SELFPLUS      {$$=mknode(USELFPLUS,$1,NULL,NULL,yylineno);strcpy($$->type_id,"SELFPLUS");}  //自增
      | SELFPLUS Exp     {$$=mknode(MSELFPLUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"SELFPLUS");}  //自增
      | Exp MINUSASS Exp {$$=mknode(MINUSASS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUSASS");} //复合减
      | Exp SLEFMINUS  {$$=mknode(USLEFMINUS,$1,NULL,NULL,yylineno);strcpy($$->type_id,"SLEFMINUS");} //自减
      | SLEFMINUS Exp {$$=mknode(MSLEFMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"SLEFMINUS");} //自减
      //-------以上-------

      //一元运算
      | LP Exp RP     {$$=$2;}//括号表达式
      | MINUS Exp %prec UMINUS   {$$=mknode(UMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"UMINUS");}//取负
      | NOT Exp       {$$=mknode(NOT,$2,NULL,NULL,yylineno);strcpy($$->type_id,"NOT");}//逻辑或
      
      //不包含运算符，较特殊的表达式
      | ID LP Args RP {$$=mknode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}  //函数调用(含参)
      | ID LP RP      {$$=mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数调用(无参)
      | ID LB List_INT RB  {$$=mknode(Array_Call,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//数组索引调用
      
      //最基本表达式
      | ID            {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//普通变量
      | INT           {$$=mknode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}      //整常数常量
      | FLOAT         {$$=mknode(FLOAT,NULL,NULL,NULL,yylineno);$$->type_float=$1;$$->type=FLOAT;} //浮点数常量
      | CHAR          {$$=mknode(CHAR,NULL,NULL,NULL,yylineno);$$->type_char=$1;$$->type=CHAR;} //字符常量
      ;

/*实参列表*/
Args:  Exp COMMA Args    {$$=mknode(ARGS,$1,$3,NULL,yylineno);}
       | Exp               {$$=mknode(ARGS,$1,NULL,NULL,yylineno);}
       ;
       
%%

int TypeCheck(char* type)
{
    if (!strcmp(type,"int"))
	{
		return INT;
	}
	if (!strcmp(type,"float"))
	{
		return FLOAT;
	}
	if (!strcmp(type,"char"))
	{
		return CHAR;
	}
}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}