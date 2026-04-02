#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

int main()
{
    int frame = 0;
    auto start = std::chrono::steady_clock::now();

    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        double t = std::chrono::duration<double>(now - start).count();

        int x = static_cast<int>(320 + 200 * std::sin(t));
        int y = static_cast<int>(240 + 100 * std::cos(t));

        std::cout << frame << " " << x << " " << y << std::endl;

        frame++;

        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }

    return 0;
}