#include <graphics.h>
#include <conio.h>
#include <iostream>

using namespace std;

const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;

// Window boundaries
int x_min, y_min, x_max, y_max;

// Function to compute the region code of a point
int computeCode(int x, int y) {
    int code = 0;
    if (x < x_min) // To the left of rectangle
        code |= LEFT;
    else if (x > x_max) // To the right of rectangle
        code |= RIGHT;
    if (y < y_min) // Below the rectangle
        code |= BOTTOM;
    else if (y > y_max) // Above the rectangle
        code |= TOP;

    return code;
}

// Cohen-Sutherland line clipping algorithm
void cohenSutherlandClip(int x1, int y1, int x2, int y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // Both points are inside the rectangle
            accept = true;
            break;
        } else if (code1 & code2) {
            // Both points share an outside region (trivial rejection)
            break;
        } else {
            // Some segment of the line is inside the rectangle
            int code_out;
            int x, y;

            // Pick point outside the rectangle
            if (code1 != 0) {
                code_out = code1;
            } else {
                code_out = code2;
            }

            // Find intersection point
            if (code_out & TOP) {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            } else if (code_out & BOTTOM) {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            } else if (code_out & RIGHT) {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            } else if (code_out & LEFT) {
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            // Replace the outside point with the intersection point
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept) {
        // Drawing the clipped line
        setcolor(WHITE);
        line(x1, y1, x2, y2);
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    // Define the clipping window
    x_min = 100, y_min = 100, x_max = 300, y_max = 300;

    // Draw the clipping window
    rectangle(x_min, y_min, x_max, y_max);

    // Input the coordinates of the line
    int x1, y1, x2, y2;
    cout << "Enter the coordinates of the line (x1 y1 x2 y2): " << endl;
    cin >> x1 >> y1 >> x2 >> y2;

    // Draw the original line in red
    setcolor(RED);
    line(x1, y1, x2, y2);
    
    // Perform Cohen-Sutherland clipping
    cohenSutherlandClip(x1, y1, x2, y2);

    getch();
    closegraph();
    return 0;
}
