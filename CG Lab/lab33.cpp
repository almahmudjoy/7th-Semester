#include <bits/stdc++.h>
#include <graphics.h>
#include <windows.h>
#include <conio.h>
using namespace std;

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    //int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    //int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    //initwindow(screenWidth, screenHeight, "");

    int x,y,x1, y1, x2, y2, x3, y3, x4, y4;

    x=getmaxx();
    y=getmaxx();

    line(0, y/2, x, y/2);
    line(x/2, 0, x/2, y);

    int xCenter= x/2;
    int yCenter= y/2;

    cout << "Enter the value of x1: ";
    cin >> x1;
    cout << "Enter the value of y1: ";
    cin >> y1;
    cout << "Enter the value of x2: ";
    cin >> x2;
    cout << "Enter the value of y2: ";
    cin >> y2;
    cout << "Enter the value of x3: ";
    cin >> x3;
    cout << "Enter the value of y3: ";
    cin >> y3;
    cout << "Enter the value of x4: ";
    cin >> x4;
    cout << "Enter the value of y4: ";
    cin >> y4;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = 2 * dy - dx;
    int ds = 2 * dy;
    int dt = 2 * (dy - dx);

    int dx1 = x4 - x3;
    int dy1 = y4 - y3;
    int d1 = 2 * dy1 - dx1;
    int ds1 = 2 * dy1;
    int dt1 = 2 * (dy1 - dx1);

    x = x1;
    y = y1;

    while (x <= x2)
    {
        putpixel(xCenter + x, yCenter - y, WHITE);
        cout << x << " " << y << "  (" << x << "," << y << ")" << endl;
        if (d < 0)
        {
            x++;
            d = d + ds;
        }
        else
        {
            x++;
            y++;
            d = d + dt;
        }
    }
    x = x3;
    y = y3;
    while (x <= x4)
    {
        putpixel(xCenter + x, yCenter - y, WHITE);
        cout << x << " " << y << "  (" << x << "," << y << ")" << endl;
        if (d1 < 0)
        {
            x++;
            d1 = d1 + ds1;
        }
        else
        {
            x++;
            y++;
            d1 = d1 + dt1;
        }
    }
    getch();
    closegraph();

    return 0;
}
