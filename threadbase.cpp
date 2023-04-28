#include "threadbase.h"


using namespace std;

ThreadBase::ThreadBase(callback func, void *userdata)
    : _thread(nullptr),
      _pauseFlag(false),
      _stopFlag(false),
      _state(Stopped)
    , m_callback(func)
    , m_userdata(userdata)
{

}

ThreadBase::~ThreadBase()
{
    stop();
}

ThreadBase::State ThreadBase::state() const
{
    return _state;
}

void ThreadBase::start()
{
    if (_thread == nullptr)
    {
        _state = Starting;
        _thread = new thread(&ThreadBase::run, this);
        _pauseFlag = false;
        _stopFlag = false;
        _state = Running;
    }

    m_framecnt = 0;
}

void ThreadBase::stop()
{
    if (_thread != nullptr)
    {
        _state = Stopping;
        _pauseFlag = false;
        _stopFlag = true;
        _condition.notify_all();  // Notify one waiting thread, if there is one.
        _thread->join(); // wait for thread finished
        delete _thread;
        _thread = nullptr;
        _state = Stopped;
    }
}

void ThreadBase::pause()
{
    if (_thread != nullptr)
    {
        _state = Pausing;
        _pauseFlag = true;
        _state = Paused;
    }
}

void ThreadBase::resume()
{
    if (_thread != nullptr)
    {
        _state = Resuming;
        _pauseFlag = false;
        _condition.notify_all();
        _state = Running;
    }
}

void ThreadBase::run()
{
    while (!_stopFlag)
    {
        if (_pauseFlag)
        {
            unique_lock<mutex> locker(_mutex);
            while (_pauseFlag)
            {
                _condition.wait(locker); // Unlock _mutex and wait to be notified
            }
            locker.unlock();
        }
        else
        {
            process();
            //sleep_ms(10);
        }
    }

    _pauseFlag = false;
    _stopFlag = false;
}

void ThreadBase::process()
{
    if(nullptr != m_callback)
    {
        (*m_callback)(m_userdata);
    }
}

void ThreadBase::registerCallback(callback func, void *userdata)
{
    int status = state();
    if(Running == status) stop();
    m_callback = func;
    m_userdata = userdata;
    if(Running == status) start();
}

