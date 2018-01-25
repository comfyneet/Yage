#ifndef YAGE_CORE_TIMER_H
#define YAGE_CORE_TIMER_H

#include "core/Config.h"

namespace ye
{
    class YE_API Timer
    {
    public:
        static double GetTime();

        static void Sleep(int32_t milliseconds);

        Timer() = delete;

    private:
        static double s_frequency;
        static bool s_timerInitialized;
        // static std::chrono::high_resolution_clock::time_point s_epoch;
    };
}

#endif
