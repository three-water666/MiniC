//用于测试普通文法

int a;

int main()
{
    int b,c;
    int d[10];
    b=10;
    c=0;
    a=0;
    b++;
    b=b+c;
    while(b>0)
    {
        int a;
        a=1;
        if(c>3)
        {
            break;
        }
        b=b-1;
        if(a>5)
        {
            continue;
        }
        b=b+1;
    }
}