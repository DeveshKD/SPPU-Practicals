#include <graphics.h>
#include <conio.h>
#include <vector>

// Class for Train
class Train {
private:
    int x, y;              // Starting position
    int speed;             // Movement speed
    int carCount;          // Number of cars
    std::vector<int> smokeX, smokeY; // Smoke coordinates

public:
    Train(int startX, int startY, int trainSpeed, int numCars)
        : x(startX), y(startY), speed(trainSpeed), carCount(numCars) {}

    void drawSmoke() {
        for (size_t i = 0; i < smokeX.size(); ++i) {
            setcolor(LIGHTGRAY);
            setfillstyle(SOLID_FILL, LIGHTGRAY);
            fillellipse(smokeX[i], smokeY[i], 10, 10);
            smokeY[i] -= 2; // Smoke moves up
            if (smokeY[i] < 0) {
                smokeX.erase(smokeX.begin() + i);
                smokeY.erase(smokeY.begin() + i);
                i--;
            }
        }
    }

    void drawEngine() {
        int engineWidth = 120;

        // Engine Body
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        rectangle(x, y, x + engineWidth, y + 70);
        floodfill(x + 10, y + 10, RED);

        // Chimney
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        rectangle(x + 20, y - 30, x + 40, y);
        floodfill(x + 25, y - 10, DARKGRAY);

        // Add smoke
        if (rand() % 5 == 0) {
            smokeX.push_back(x + 30);
            smokeY.push_back(y - 40);
        }

        // Windows
        setcolor(CYAN);
        setfillstyle(SOLID_FILL, CYAN);
        rectangle(x + 50, y + 10, x + 80, y + 40);
        floodfill(x + 60, y + 20, CYAN);

        // Headlight
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + 110, y + 35, 10, 10);

        // Wheels
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse(x + 20, y + 80, 15, 15);
        fillellipse(x + 100, y + 80, 15, 15);
    }

    void drawCar(int offsetX) {
        // Car Body
        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        rectangle(x - offsetX, y, x - offsetX + 100, y + 70);
        floodfill(x - offsetX + 10, y + 10, BLUE);

        // Windows
        setcolor(LIGHTCYAN);
        setfillstyle(SOLID_FILL, LIGHTCYAN);
        rectangle(x - offsetX + 10, y + 20, x - offsetX + 40, y + 50);
        floodfill(x - offsetX + 15, y + 25, LIGHTCYAN);

        rectangle(x - offsetX + 60, y + 20, x - offsetX + 90, y + 50);
        floodfill(x - offsetX + 65, y + 25, LIGHTCYAN);

        // Wheels
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse(x - offsetX + 20, y + 80, 15, 15);
        fillellipse(x - offsetX + 80, y + 80, 15, 15);
    }

    void drawTracks() {
        setcolor(BROWN);
        setfillstyle(SOLID_FILL, BROWN);
        rectangle(0, y + 90, getmaxx(), y + 100); // Tracks as a rectangle
        floodfill(10, y + 95, BROWN);
    }

    void draw() {
        // Draw smoke
        drawSmoke();

        // Draw train cars to the left of the engine
        for (int i = 1; i <= carCount; i++) {
            drawCar(i * 130);
        }

        // Draw engine at the rightmost position
        drawEngine();

        // Draw tracks below the train
        drawTracks();
    }

    void move() {
        x += speed; // Move train to the right
        if (x > getmaxx() + carCount * 130) { // Reset position
            x = -120;
        }
    }
};

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int screenHeight = getmaxy();

    // Create a train object
    Train train(-200, screenHeight - 150, 5, 3);

    while (true) {
        cleardevice(); // Clear screen

        train.draw(); // Draw train
        train.move(); // Move train

        delay(50); // Animation delay
    }

    getch();
    closegraph();
    return 0;
}
