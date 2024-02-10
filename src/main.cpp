#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>

const int SCREEN_WIDTH = 120;
const int SCREEN_HEIGHT = 40;
const double PI = 3.14159265;

// TODO: Read user input to change the components of the sinusoidal curve.

/**
 * Draws a sinusoidal curve on the screen.
 * 
 * This function clears the screen and draws a sinusoidal curve on it.
 * The curve is drawn using the formula f(x) = a * sin(bx + c) + d.
 *
 * @param a The amplitude of the curve.
 * @param b The frequency of the curve.
 * @param c The phase shift of the curve.
 * @param d The vertical shift of the curve.
 * @param screen The screen to draw the curve on.
 */
void drawSin(double a, double b, double c, double d, wchar_t* screen);

int main()
{
    wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    double angle = 0.0f;
    bool running = true;

    auto tStart = std::chrono::high_resolution_clock::now();
    while (running)
    {
        auto tEnd = std::chrono::high_resolution_clock::now();
        double elapsedTimeMs = std::chrono::duration<double, std::milli>(tEnd - tStart).count() / 1000.0;
        tStart = tEnd;

        angle += 3.0 * elapsedTimeMs;
        if (angle > 2 * PI)     angle -= 2 * PI;

        double period = SCREEN_WIDTH / 4;
        double a = SCREEN_HEIGHT / 2;
        double b = 2 * PI / period;
        double c = angle;
        double d = SCREEN_HEIGHT / 2;
        drawSin(a, b, c, d, screen);    // f(x) = a * sin(bx + c) + d

        screen[SCREEN_WIDTH * SCREEN_HEIGHT - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &dwBytesWritten);
    }

    delete[] screen;
    CloseHandle(hConsole);

    return 0;
}

void drawSin(double a, double b, double c, double d, wchar_t* screen)
{
    double y = 0.0f;
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            screen[j * SCREEN_WIDTH + x] = L' ';
        }

        if (x < SCREEN_WIDTH / 1.5)
        {
            y = a * sin(b * x + c) + d;
            screen[int(y) * SCREEN_WIDTH + x] = L'#';
            if (x > 0)              screen[int(y) * SCREEN_WIDTH + x - 1] = L'#';
            if (x < SCREEN_WIDTH)   screen[int(y) * SCREEN_WIDTH + x + 1] = L'#';
        }
    }

    // Draw an indicator on the right side of the screen.
    screen[int(y) * SCREEN_WIDTH + SCREEN_WIDTH - 2] = L'>';
    screen[int(y) * SCREEN_WIDTH + SCREEN_WIDTH - 1] = L'O';
}