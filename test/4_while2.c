// 带有break，和continue的while语句
int main()
{
    int a=10;
    int b=0;
    while(a>=0)
    {
        if(b>=3)
        {
            break;
        }
        a=a-1;
        if(a>5)
        {
            continue;
        }
        b=b+1;
    }
}