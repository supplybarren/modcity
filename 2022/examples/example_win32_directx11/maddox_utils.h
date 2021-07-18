#pragma once

template <typename T>
class ABSingleton
{
protected:
    ABSingleton() { }
    ~ABSingleton() { }

    ABSingleton(const ABSingleton&) = delete;
    ABSingleton& operator=(const ABSingleton&) = delete;

    ABSingleton(ABSingleton&&) = delete;
    ABSingleton& operator=(ABSingleton&&) = delete;
public:
    static T& Get()
    {
        static T pInstance{ };
        return pInstance;
    }
};
