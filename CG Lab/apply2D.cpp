#include <iostream>
#include<graphics.h>
#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
#include<math.h>
using namespace std;

void tri(int xc, int yc)
{
    int x1 = -50, y1 = 50;
    int x2 = 50, y2 = 50;
    int x3 = 0, y3 = -50;

    setcolor(WHITE);

    line(x1+xc,y1+yc,x2+xc,y2+yc);
    delay(50);
    line(x2+xc,y2+yc,x3+xc,y3+yc);
    delay(50);
    line(x3+xc,y3+yc,x1+xc,y1+yc);
    delay(50);

    // rotate
    float t = 60*(3.1416/180);
    float rx1 = x1*cos(t) - y1*sin(t);
    float ry1 = x1*sin(t) + y1*cos(t);

    float rx2 = x2*cos(t) - y2*sin(t);
    float ry2 = x2*sin(t) + y2*cos(t);

    float rx3 = x3*cos(t) - y3*sin(t);
    float ry3 = x3*sin(t) + y3*cos(t);


    setcolor(YELLOW);
    line(rx1+xc,ry1+yc,rx2+xc,ry2+yc);
    delay(50);
    line(rx2+xc,ry2+yc,rx3+xc,ry3+yc);
    delay(50);
    line(rx3+xc,ry3+yc,rx1+xc,ry1+yc);
    delay(50);


    // scaleing

    float sx1 = 2*rx1, sy1 = 2*ry1;
    float sx2 = 2*rx2, sy2 = 2*ry2;
    float sx3 = 2*rx3, sy3 = 2*ry3;

    setcolor(GREEN);
    line(sx1+xc,sy1+yc,sx2+xc,sy2+yc);
    delay(50);
    line(sx2+xc,sy2+yc,sx3+xc,sy3+yc);
    delay(50);
    line(sx3+xc,sy3+yc,sx1+xc,sy1+yc);
    delay(50);

}


int main()
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"");

    int x=getmaxx(),y=getmaxy();
    int xc = x/2, yc = y/2;
    setcolor(RED);
    line(0,yc,x,yc);
    setcolor(RED);
    line(xc,0,xc,y);

    tri(xc,yc);


    getch();
    closegraph();
    return 0;
}

