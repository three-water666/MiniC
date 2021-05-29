%{
#include "minic_yacc.h"
#include "string.h"
#include "option.h"
int yycolumn=1;
#define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno; \
yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;//每次匹配之前执行的动作，yylloc表示当前词法单元所对应的位置信息
typedef union 
{
	int type_int;
	float type_float;
	char type_char;
	char type_id[32];
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL
void comment();
%}
%option yylineno//行数自动加一

id   [A-Za-z][A-Za-z0-9]*  
int    [0-9]+
float  ([0-9]*\.[0-9]+)|([0-9]+\.)
char [A-Z]|[a-z]|"!"|"@"|"#"|"$"|"%"|"^"|"&"|"*"|"("|")"|"_"|"-"|"+"|"="|"\\"|"{"|"}"|"["|"]"|":"|";"|"'"|"<"|">"|","|"."|"?"|"/"|"~"|"`"

%%
{int}        {yylval.type_int=atoi(yytext); return INT;}
{float}      {yylval.type_float=atof(yytext); return FLOAT;}
"'"{char}"'" {yylval.type_char=yytext[1]; return CHAR;}
"int"        {strcpy(yylval.type_id, yytext);return TYPE;}
"float"      {strcpy(yylval.type_id, yytext);return TYPE;}
"char"       {strcpy(yylval.type_id, yytext);return TYPE;}

"return"     {return RETURN;}
"if"         {return IF;}
"else"       {return ELSE;}
"while"      {return WHILE;}
"for"        {return FOR;}

{id}       {strcpy(yylval.type_id,  yytext); return ID;/*由于关键字的形式也符合表示符的规则，所以把关键字的处理全部放在标识符的前面，优先识别*/}
";"			{return SEMI;}
","			{return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" {strcpy(yylval.type_id, yytext);return RELOP;}
"="			{return ASSIGNOP;}
"+"			{return PLUS;}
"+="        {return PLUSASS;}
"++"       {return SELFPLUS;}
"-"			{return MINUS;}
"-="        {return MINUSASS;}
"--"        {return SLEFMINUS;}
"*"			{return STAR;}
"/"			{return DIV;}
"&&"        {return AND;}
"||"        {return OR;}
"!"			{return NOT;}
"("			{return LP;}
")"			{return RP;}
"{"			{return LC;}
"}"			{return RC;}
"["         {return LB;}
"]"         {return RB;}
"//"[^\n]*\n  ;
"/*"        comment();
[\n]        {yycolumn=1;}   
[ \r\t]     {}   
.			{printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}
%%
int yywrap()
{
	return 1;
}

void comment()
{
    char c, c1;
loop:
    while ((c = input()) != '*' && c != 0)
        putchar(c);

    if ((c1 = input()) != '/' && c != 0)
    {
        unput(c1);
        goto loop;
    }
}
