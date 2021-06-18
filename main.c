#include "stdio.h"
#include "math.h"
#include "string.h"
#include "option.h"


extern FILE *yyin;
extern int yylineno;

int main(int argc, char *argv[])
{
    if(argc==2)
    {
        OPTION=' ';
        yyin=fopen(argv[1],"r");
    }
    else if(argc==3)
    {
        OPTION=argv[1][1];
        yyin=fopen(argv[2],"r");
    }
    else
    {
        printf("Usage:  minic -sym|ir|ast xxx.c\n");
        printf("Or      minic xxx.c\n");
        return 0;
    }

    // yyin=fopen(argv[1],"r");
    // yyin=fopen("./test/chushihua.c","r");//调试用
	if (!yyin) 
    {
        return 0;
    }
	yylineno=1;
	yyparse();

	return 0;
}