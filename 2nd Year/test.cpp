#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <cmath>

// Class for Energy Ball
class EnergyBall {
private:
    int x, y;        // Ball position
    int radius;      // Ball size
    bool isActive;   // Flag to control ball movement
    int speed;       // Ball movement speed

public:
    EnergyBall(int startX, int startY, int r, int s)
        : x(startX), y(startY), radius(r), speed(s), isActive(false) {}

    void activate() {
        isActive = true;
    }

    void move() {
        if (isActive) {
            x += speed; // Move ball to the right
        }
    }

    void draw() {
        if (isActive) {
            setcolor(MAGENTA);
            setfillstyle(SOLID_FILL, MAGENTA);
            fillellipse(x, y, radius, radius);
        }
    }

    bool offScreen(int screenWidth) {
        return x - radius > screenWidth; // Check if ball is off-screen
    }

    void reset(int startX, int startY) {
        x = startX;
        y = startY;
        isActive = false;
    }

    int getX() const { return x; }
    int getY() const { return y; }
};

// Class for Stickman
class Stickman {
private:
    int baseX, baseY; // Base position
    int size;         // Size factor
    int armAngle;     // Throwing arm angle

public:
    Stickman(int x, int y, int s)
        : baseX(x), baseY(y), size(s), armAngle(0) {}

    void draw() {
        // Head
        setcolor(WHITE);
        circle(baseX, baseY - 3 * size, size);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(baseX, baseY - 3 * size, size, size);

        // Body
        line(baseX, baseY - 2 * size, baseX, baseY);

        // Left arm
        line(baseX, baseY - size, baseX - 2 * size, baseY - size - size / 2);

        // Throwing arm (right arm)
        int handX = baseX + 2 * size * cos(armAngle * 3.14 / 180);
        int handY = baseY - size + 2 * size * sin(armAngle * 3.14 / 180);
        line(baseX, baseY - size, handX, handY);

        // Legs
        line(baseX, baseY, baseX - size, baseY + 2 * size);
        line(baseX, baseY, baseX + size, baseY + 2 * size);
    }

    void updateThrowingArm(int angle) {
        armAngle = angle;
    }

    int getHandX() const {
        return baseX + 2 * size * cos(armAngle * 3.14 / 180);
    }

    int getHandY() const {
        return baseY - size + 2 * size * sin(armAngle * 3.14 / 180);
    }
};

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    int screenWidth = getmaxx();
    int screenHeight = getmaxy();

    // Initialize stickman and energy ball
    Stickman stickman(100, screenHeight - 200, 20);
    EnergyBall energyBall(stickman.getHandX(), stickman.getHandY(), 10, 8);

    bool isThrowing = false; // To track throw motion
    int throwAngle = 0;      // Arm angle for animation

    while (true) {
        cleardevice(); // Clear the screen

        // Update the stickman's arm for throwing motion
        if (throwAngle < 45 && !isThrowing) {
            throwAngle += 2; // Arm moves backward
        } else {
            isThrowing = true;
            throwAngle -= 4; // Arm moves forward
        }

        // Reset throw motion
        if (throwAngle <= 0) {
            isThrowing = false;
            energyBall.activate(); // Throw the ball
        }

        stickman.updateThrowingArm(throwAngle);
        stickman.draw();

        // Move and draw the energy ball
        if (energyBall.getX() == stickman.getHandX() && !isThrowing) {
            energyBall.reset(stickman.getHandX(), stickman.getHandY());
        }

        energyBall.move();
        energyBall.draw();

        // Reset the ball if it goes off the screen
        if (energyBall.offScreen(screenWidth)) {
            energyBall.reset(stickman.getHandX(), stickman.getHandY());
        }

        delay(30); // Control animation speed
    }

    getch();
    closegraph();
    return 0;
}
