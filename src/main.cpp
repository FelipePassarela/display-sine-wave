#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>

const int SCREEN_WIDTH = 120;
const int SCREEN_HEIGHT = 40;
const double PI = 3.14159265;

// TODO: Add CMakelists.txt file to compile the project using CMake.

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

/**
 * @brief Draws the components information on the screen.
 * 
 * This function shows the amplitude, frequency, phase shift, and vertical shift of the 
 * sinusoidal curve on the top of the screen. 
 * 
 * @param a The amplitude of the curve.
 * @param b The frequency of the curve.
 * @param c The phase shift of the curve.
 * @param d The vertical shift of the curve.
 * @param screen The screen buffer to draw the information on.
 */
void drawComponentsInfo(double a, double b, double c, double d, wchar_t* screen);

/**
 * Reads user input to change the components of the sinusoidal curve.
 * 
 * The user can change the amplitude, frequency, phase shift, and vertical shift of the curve.
 * 
 * @param a The amplitude of the curve.
 * @param b The frequency of the curve.
 * @param c The phase shift of the curve.
 * @param d The vertical shift of the curve.
 * @param elapsedTimeMs The time elapsed since the last frame.
 */
void readInput(double& a, double& b, double& c, double& d, double elapsedTimeMs);

int main()
{
    wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    double angle = 0.0f;
    double period = SCREEN_WIDTH / 4;
    double a = SCREEN_HEIGHT / 2;
    double b = 2 * PI / period;
    double d = SCREEN_HEIGHT / 2;

    bool running = true;
    auto tStart = std::chrono::high_resolution_clock::now();
    
    while (running)
    {
        auto tEnd = std::chrono::high_resolution_clock::now();
        double elapsedTimeMs = std::chrono::duration<double, std::milli>(tEnd - tStart).count() / 1000.0;
        tStart = tEnd;

        readInput(a, b, angle, d, elapsedTimeMs);

        angle += 3.0 * elapsedTimeMs;
        if (angle > 2 * PI) angle -= 2 * PI;

        drawComponentsInfo(a, b, angle, d, screen);
        drawSin(a, b, angle, d, screen);    // f(x) = a * sin(bx + c) + d

        screen[SCREEN_WIDTH * SCREEN_HEIGHT - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &dwBytesWritten);
    }

    delete[] screen;
    CloseHandle(hConsole);

    return 0;
}

void drawComponentsInfo(double a, double b, double c, double d, wchar_t* screen)
{
    wchar_t* componentsInfo = new wchar_t[SCREEN_WIDTH];
    swprintf_s(componentsInfo, SCREEN_WIDTH, L"Amplitude: %5.2f, Frequency: %5.2f, Phase Shift: %5.2f, Vertical Shift: %5.2f", a, b, c, d);
    for (int i = 0; i < wcslen(componentsInfo); i++)
    {
        screen[i] = componentsInfo[i];
    }
    delete[] componentsInfo;
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

            if (y > SCREEN_HEIGHT - 1)  y = SCREEN_HEIGHT - 1;
            if (y < 0)                  y = 0;

            screen[int(y) * SCREEN_WIDTH + x] = L'#';
            if (x > 0)              screen[int(y) * SCREEN_WIDTH + x - 1] = L'#';
            if (x < SCREEN_WIDTH)   screen[int(y) * SCREEN_WIDTH + x + 1] = L'#';
        }
    }

    // Draw an indicator on the right side of the screen.
    screen[int(y) * SCREEN_WIDTH + SCREEN_WIDTH - 2] = L'>';
    screen[int(y) * SCREEN_WIDTH + SCREEN_WIDTH - 1] = L'O';
}

void readInput(double& a, double& b, double& c, double& d, double elapsedTimeMs)
{
    if (GetAsyncKeyState('W') & 0x8000)         a += 20.0 * elapsedTimeMs;
    if (GetAsyncKeyState('S') & 0x8000)         a -= 20.0 * elapsedTimeMs;
    if (GetAsyncKeyState('A') & 0x8000)         b -= 0.1 * elapsedTimeMs;
    if (GetAsyncKeyState('D') & 0x8000)         b += 0.1 * elapsedTimeMs;
    if (GetAsyncKeyState('Q') & 0x8000)         c -= 6.0 * elapsedTimeMs;
    if (GetAsyncKeyState('E') & 0x8000)         c += 3.0 * elapsedTimeMs;
    if (GetAsyncKeyState(VK_UP) & 0x8000)       d -= 20.0 * elapsedTimeMs;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)     d += 20.0 * elapsedTimeMs;
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)    c -= 3.0 * elapsedTimeMs;   // Stop the wave. This have to be the same value of c increment
    
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)   exit(0);

    if (a > SCREEN_HEIGHT / 2)                  a = SCREEN_HEIGHT / 2;
    if (a < -SCREEN_HEIGHT / 2)                 a = -SCREEN_HEIGHT / 2;
    if (b > PI / 10)                            b = PI / 10;
    if (b < 0)                                  b = 0;
    if (c > 2 * PI)                             c -= 2 * PI;
    if (c < 0)                                  c += 2 * PI;
    if (d > 3 * SCREEN_HEIGHT / 2)              d = 3 * SCREEN_HEIGHT / 2;
    if (d < -SCREEN_HEIGHT / 2)                 d = -SCREEN_HEIGHT / 2;
}
