#include <iostream>
#include <graphics.h>
using namespace std;

void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = 2 * dy - dx;
    int ds = 2 * dy;
    int dt = 2 * (dy - dx);

    int x = x1, y = y1;

    initwindow(500, 500);
    putpixel(x, y, WHITE);

    cout << "x y   (x,y)" << endl;
    cout << x << " " << y << "  (" << x << "," << y << ")" << endl;

    while (x <= x2) {
        if (d < 0) {
            x++;
            d = d + ds;
        } else {
            x++;
            y++;
            d = d + dt;
        }
        if (x <= x2) {
            putpixel(x, y, WHITE);
            cout << x << " " << y << "  (" << x << "," << y << ")" << endl;
        }
    }

    delay(50000);
    closegraph();
}

int main() {
    int x1, y1, x2, y2;
    cout << "Enter the value of x1: ";
    cin >> x1;
    cout << "Enter the value of y1: ";
    cin >> y1;
    cout << "Enter the value of x2: ";
    cin >> x2;
    cout << "Enter the value of y2: ";
    cin >> y2;

    bresenhamLine(x1, y1, x2, y2);

    return 0;
}
