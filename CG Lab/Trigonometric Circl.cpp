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

void trigonometricCircle(int xc, int yc, int r) {
    initwindow(500, 500);

    for (double theta = 0; theta <= 45; theta += 0.1) { // θ from 0 to 45 degrees
        int x = round(r * cos(theta * M_PI / 180)); // Compute x using cosine
        int y = round(r * sin(theta * M_PI / 180)); // Compute y using sine
        plotCirclePoints(xc, yc, x, y); // Reflect to all 8 octants
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

    trigonometricCircle(xc, yc, r);

    return 0;
}

