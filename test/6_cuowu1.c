// 简单的错误提示
int main()
{
    int d;
    char b;
    int d;//变量重定义
    d = c;//变量未定义
    1++;//语法错误
    fun1();//函数未定义
    return 0;
}

int fun2()
{
    int m;
}

int fun2()//函数重定义
{
    int n;
}