#pragma once

template <typename T>

class Singleton
{
public:
    Singleton() {};
    virtual ~Singleton() {};

    static T* GetInstance()
    {
        static T* inst = new T;
        return inst;
    }
};