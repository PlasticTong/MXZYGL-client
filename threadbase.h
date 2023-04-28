#ifndef THREADBASE_H
#define THREADBASE_H

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

inline void sleep_ms(long long ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline void sleep_until_ms(std::chrono::system_clock::time_point time)
{
    std::this_thread::sleep_until(time);
}

typedef void(*callback)(void*);

class ThreadBase
{
public:
    ThreadBase(callback func=nullptr, void *userdata=nullptr);
    virtual ~ThreadBase();

    enum State
    {
        Stopping,
        Stopped,     ///<停止状态，包括从未启动过和启动后被停止
        Starting,
        Running,    ///<运行状态
        Pausing,
        Paused,      ///<暂停状态
        Resuming
    };

    State state() const;

    void start();
    virtual void stop();
    void pause();
    void resume();

    void registerCallback(callback func, void *userdata=nullptr);

    int m_framecnt;

protected:
    virtual void process();

private:
    void run();

private:
    std::thread* _thread;
    std::mutex _mutex;
    std::condition_variable _condition;
    std::atomic_bool _pauseFlag;   ///<暂停标识
    std::atomic_bool _stopFlag;   ///<停止标识
    State _state;

    callback    m_callback;
    void        *m_userdata;
};

#endif // THREADBASE_H
