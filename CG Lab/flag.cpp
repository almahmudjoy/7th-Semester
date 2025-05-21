#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

void FloodFill(int x, int y, int original,int fill_clr)
{

    int clr = getpixel(x,y);
    if(clr==original)
    {
        putpixel(x,y,fill_clr);

        FloodFill(x+1,y,original,fill_clr);
        FloodFill(x-1,y,original,fill_clr);
        FloodFill(x,y+1,original,fill_clr);
        FloodFill(x,y-1,original,fill_clr);
    }
}

void bresenham(int x,int r,int c,int d)
{
    int y,p;
    p=3-2*r;
    y=r;
    while( x<=y)
    {
        putpixel(x+c,-y+d,RED);
        putpixel(x+c,d+y,RED);
        putpixel(-x+c,d+y,RED);
        putpixel(-x+c,d-y,RED);
        putpixel(y+c,d+x,RED);
        putpixel(y+c,d-x,RED);
        putpixel(-y+c,d+x,RED);
        putpixel(-y+c,d-x,RED);

        if(p>=0)
        {
            p=p+4*(x-y)+10;
            x=x+1;
            y=y-1;
        }
        else
        {
            p=p+4*x+6;
            x=x+1;
        }
    }
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    int r=50;
    float th=0;
    int A = getmaxx();
    int B = getmaxy();
    int c,d,x,y,original,fill_clr;

    setcolor(10);

    line(-200+A/2,-100+B/2,200+A/2,-100+B/2);
    line(200+A/2,-100+B/2,200+A/2,100+B/2);
    line(200+A/2,100+B/2,-200+A/2,100+B/2);
    line(-200+A/2,-100+B/2,-200+A/2,100+B/2);


    bresenham(0,50,A/2,B/2);
    FloodFill(A/2, B/2,BLACK,RED);
    FloodFill(A/2+51,B/2+51,BLACK,GREEN);

    getch();
    closegraph();

    return 0;


}
