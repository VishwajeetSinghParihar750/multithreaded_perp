#pragma once

#include <queue>
#include <optional>
#include <string>
#include <mutex>
#include <condition_variable>

template <typename T>
class mpscQueue
{
    std::queue<T> q;

    std::mutex mtx;
    std::condition_variable cv;

public:
    T waitPop()
    {
        mtx.lock();

        cv.wait(mtx, [&]() -> bool
                { return !q.empty(); });

        auto toReturn = std::move(q.front());
        q.pop();

        mtx.unlock();

        return toReturn;
    }

    std::optional<T> pop()
    {
        mtx.lock();

        if (q.empty())
            return std::nullopt;

        auto toReturn = std::move(q.front());
        q.pop();

        mtx.unlock();

        return toReturn;
    }

    void push(T item)
    {
        mtx.lock();
        q.push(std::move(item));
        mtx.unlock();
    }
};