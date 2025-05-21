#include<graphics.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>

using namespace std;
int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm,"");

    int A=getmaxx(), B=getmaxy();
    setcolor(RED);
    line(0,B/2,A,B/2);
    setcolor(RED);
    line(A/2,0,A/2,B);
    A=A/2;
    B=B/2;


    int x1,x2,y1,y2,X,Y;
    float m,b;
    cout<< "Enter x1: ";
    cin>>x1;
    cout<< "Enter y1: ";
    cin>>y1;
    cout<< "Enter x2: ";
    cin>>x2;
    cout<< "Enter y2: ";
    cin>>y2;

    m = (float)(y2-y1)/(float)(x2-x1);
    b = y1-m*x1;
    cout<<m<<"\n";

    if(abs(m)<=1)
    {
        if(x1>x2)
        {
            int t1=x1;
            x1=x2;
            x2=t1;
            int t2=y1;
            y1=y2;
            y2=t2;
        }
        X=x1;
        Y=y1;
        putpixel(X+A,B-Y,YELLOW);
        while(X<x2)
        {
            X++;
            Y=m*X+b;
            putpixel(X+A,B-Y,YELLOW);
            cout<<X<<" "<<Y<<"\n";
        }
    }
    else
    {
        X=x1;
        Y=y1;
        while(Y<=y2)
        {
            putpixel(X+A,Y-B,YELLOW);
            Y++;
            X=(Y-b)/m;
            cout<<X<<" "<<Y<<"\n";
        }
    }

    getch();
    closegraph();
    return 0;
}

