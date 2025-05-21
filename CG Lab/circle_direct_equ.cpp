#include <iostream>
#include <graphics.h>
#include <cmath>

using namespace std;

void plotCirclePoints(int xc, int yc, int x, int y) {
    putpixel(xc + x, yc + y, WHITE); // (x, y)
    putpixel(xc - x, yc + y, WHITE); // (-x, y)
    putpixel(xc + x, yc - y, WHITE); // (x, -y)
    putpixel(xc - x, yc - y, WHITE); // (-x, -y)
    putpixel(xc + y, yc + x, WHITE); // (y, x)
    putpixel(xc - y, yc + x, WHITE); // (-y, x)
    putpixel(xc + y, yc - x, WHITE); // (y, -x)
    putpixel(xc - y, yc - x, WHITE); // (-y, -x)
}

void directCircle(int xc, int yc, int r) {
    initwindow(500, 500);

    for (int x = 0; x <= r; x++) {
        int y = round(sqrt(r * r - x * x)); // Calculate y from circle equation
        plotCirclePoints(xc, yc, x, y); // Plot all symmetric points
    }

    delay(5000);
    closegraph();
}

int main() {
    int xc, yc, r;
    cout << "Enter center (xc, yc): ";
    cin >> xc >> yc;
    cout << "Enter radius (r): ";
    cin >> r;

    directCircle(xc, yc, r);

    return 0;
}

