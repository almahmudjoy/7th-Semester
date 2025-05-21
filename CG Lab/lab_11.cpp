#include <graphics.h>
#include <conio.h>
#include<bits/stdc++.h>
#include<windows.h>

using namespace std;

int main()
{

    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");


    int x =getmaxx(), y=getmaxy();
    //cout<<"x y "<<x<<" "<<y<<"\n";

    setcolor(GREEN);
    line(0, y/2, x, y/2);

    setcolor(RED);
    line(x/2, y/2, x, y/2);

    setcolor(BLUE);
    line(x/2, 0, x/2, y);

     setcolor(GREEN);
     circle(x/2, y/2, 100);



    getch();
    closegraph();
    return 0;
}
