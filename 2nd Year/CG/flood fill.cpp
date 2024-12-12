#include <stdlib.h>
#include <iostream>
#include <graphics.h>
using namespace std;
void ffill(int x, int y, int o_col, int n_col) {
     int current = getpixel(x, y);
    if (current==o_col) {
        putpixel(x, y, n_col);
        delay(1); // Small delay for better visualization
        ffill(x + 1, y, o_col, n_col);
        ffill(x - 1, y, o_col, n_col);
        ffill(x, y + 1, o_col, n_col);
        ffill(x, y - 1, o_col, n_col);
    }
}

int main(){
    int x1, y1, x2, y2, x3, y3, xavg, yavg;
    int gdriver = DETECT, gmode;
    cout << "Enter the points of the triangle (x1 y1 x2 y2 x3 y3): "; 
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    // Initialize graphics 
    initgraph(&gdriver, &gmode, NULL);
    // Draw the triangle 
    setcolor (WHITE); 
    line(x1,y1, x2, y2); 
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);
    // Calculate the average point for filling 
    xavg = (x1 + x2 + x3) / 3;
    yavg = (y1 + y2 + y3) / 3;
    // Fill the triangle 
    ffill(xavg, yavg, BLACK, RED);
    // Wait for a key press
    cout << "Press any key to exit..." << endl; 
    getch();
    // Close the graphics mode 
    closegraph();
    return 0;
}