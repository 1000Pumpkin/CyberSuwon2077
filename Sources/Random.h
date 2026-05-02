#pragma once

class Random : public Singleton<Random>
{
public:
    int rand(int min, int max)
    {
        random_device rd;
        mt19937_64 rnd(rd());

        uniform_int_distribution<int> range(min, max);

        return range(rnd);
    }
};