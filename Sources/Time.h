#pragma once

class Time : public Singleton<Time>
{
public:
    DWORD curTime;
    DWORD prevTime;
    float deltaTime = 0.f;
    float fixedDeltaTime = 0.f;
    float timeScale = 1.f;

    void Tick(void(*FncPtr)(float))
    {
        curTime = timeGetTime();
        static bool isFirst = true;
        static DWORD stepMiliSecond = 17;
        static float step = stepMiliSecond / 1000.f;

        if (!isFirst)
        {
            DWORD tick = curTime - prevTime;
            int loopCount = tick / stepMiliSecond;
            prevTime += loopCount * stepMiliSecond;
            fixedDeltaTime = step;
            while (loopCount--)
            {
                deltaTime = step * timeScale;
                FncPtr(deltaTime);
            }
        }
        else
        {
            prevTime = curTime;
            isFirst = false;
        }
    }
};