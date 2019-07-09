#ifndef timer_mgr_h
#define	timer_mgr_h
#include <iostream>
#include <functional>
#include <vector>
#include <thread>

using namespace std;

class TimerManager;

class MyTimer
{
public:
    enum class TimerType {ONCE=0, CIRCLE=1};

    MyTimer (TimerManager& manager);
    ~MyTimer ();
    //����һ����ʱ��
    template<typename Func>
    void   start (Func func, unsigned int ms, TimerType type);
    //��ֹһ����ʱ��
    void   stop ();
private:
    //ִ��
    void on_timer(unsigned long long now);
private:
    friend class TimerManager;
    TimerManager& manager_;
    //���ú����������º���
    std::function<void(void)> m_timerfunc;
    TimerType timerType_;
    //���
    unsigned int m_nInterval;
    //����
    unsigned long long  m_nExpires;
    int  m_nHeapIndex;

};

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();
    //��ȡ��ǰ������
    static unsigned long long get_current_millisecs();
    //̽��ִ��
    void detect_timers();

private:
    friend class MyTimer;
    //���һ����ʱ��
    void add_timer(MyTimer* timer);
    //�Ƴ�һ����ʱ��
    void remove_timer(MyTimer* timer);
    //��ʱ�ϸ�
    void up_heap(size_t index);
    //��ʱ�³�
    void down_heap(size_t index);
    //��������timer����
    void swap_heap(size_t index1, size_t index2);
private:
    struct HeapEntry {
        unsigned long long time;
        MyTimer* timer;
    };
    std::thread     detectTh_;
    bool            shutdown_;
    std::vector<HeapEntry> heap_;
};

template <typename Func>
inline void  MyTimer::start(Func fun, unsigned int interval, TimerType timetpe)
{
    m_nInterval = interval;
    m_timerfunc = fun;
    m_nExpires = interval + TimerManager::get_current_millisecs();
    std::cout << "interval: " << interval << std::endl;
    manager_.add_timer(this);
    timerType_= timetpe;
}

#endif