#pragma once

#include <queue>
#include <optional>
#include <string>

template <typename T>
class spscQueue
{
    std::queue<T> q;

    //
public:
    T waitPop()
    {
        //
    }

    std::optional<T> pop()
    {
    }

    void push(T item)
    {
        //
    }
};