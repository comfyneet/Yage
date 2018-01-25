#include "core/Timer.h"
#include "core/Win32Common.h"

namespace ye
{
    double Timer::s_frequency = 0.0;
    bool Timer::s_timerInitialized = false;

    double Timer::GetTime()
    {
        if (!s_timerInitialized)
        {
            //s_epoch = std::chrono::high_resolution_clock::now();

            LARGE_INTEGER li{};
            QueryPerformanceFrequency(&li);

            s_frequency = double(li.QuadPart);
            s_timerInitialized = true;
        }

        LARGE_INTEGER li{};
        QueryPerformanceCounter(&li);

        return double(li.QuadPart) / s_frequency;

        //return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - s_epoch).count() / 1000000000.0;
    }

    void Timer::Sleep(const int32_t milliseconds)
    {
        ::Sleep(milliseconds);

        // std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}
