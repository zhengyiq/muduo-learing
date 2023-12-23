#include "Thread.h"

#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
namespace muduo
{
    namespace CurrentThread
    {
        /*__thread是GCC内置的线程局部存储设施。
        __thread变量每一个线程都有一份独立的实体，各个线程互不干扰。
        可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。*/
        __thread const char *t_threadName = "unnamedThread";
    }
}
/*对于一个命名空间，如果它没有名字，那么默认它的作用域等同于其父命名空间的作用域。*/
namespace
{
    __thread pid_t t_cachedTid = 0;

    void afterFork()
    {
        t_cachedTid = gettid();
        muduo::CurrentThread::t_threadName = "main";
    }
    class ThreadNameInitializer
    {
    public:
        ThreadNameInitializer()
        {
            muduo::CurrentThread::t_threadName = "main";
            pthread_atfork(NULL, NULL, &afterFork);
        }
    };

    ThreadNameInitializer init;

    struct ThreadData
    {
        typedef muduo::Thread::ThreadFunc ThreadFunc;
        ThreadFunc func_;
        std::string name_;
        boost::weak_ptr<pid_t> wkTid_;

        ThreadData(const ThreadFunc &func,
                   const std::string &name,
                   const boost::shared_ptr<pid_t> &tid)
            : func_(func),
              name_(name),
              wkTid_(tid)
        {
        }

        void runInThread()
        {
            pid_t tid = muduo::CurrentThread::tid();
            boost::shared_ptr<pid_t> ptid = wkTid_.lock();

            if (ptid)
            {
                *ptid = tid;
                ptid.reset();
            }

            if (!name_.empty())
                muduo::CurrentThread::t_threadName = name_.c_str();

            ::prctl(PR_SET_NAME, muduo::CurrentThread::t_threadName);
            func_();
            muduo::CurrentThread::t_threadName = "finished";
        }
    };

    void *startThread(void *obj)
    {
        ThreadData *data = static_cast<ThreadData *>(obj);
        data->runInThread();
        delete data;
        return NULL;
    }
}

using namespace muduo;
pid_t CurrentThread::tid()
{
    if(t_cachedTid == 0)
    {
        t_cachedTid = gettid();
    }
    return t_cachedTid;
}

const char* CurrentThread::name()
{
    return t_threadName;
}

bool CurrentThread::isMainThread()
{
    return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc &func, const std::string &n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(new pid_t[0]),
      func_(func),
      name_(n)
{
    numCreated_.increment();    
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    ThreadData* data = new ThreadData(func_, name_, tid_);
    if (pthread_create(&pthreadId_, NULL, startThread, data))
    {
        started_ = false;
        delete data;
        abort();
    }
}

void Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    pthread_join(pthreadId_, NULL);
}