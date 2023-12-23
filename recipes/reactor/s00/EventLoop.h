#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include "thread/Thread.h"

namespace muduo
{
    class EventLoop : boost::noncopyable
    {
        public:
            EventLoop();
            ~EventLoop();

            void loop();

            void assertInLoopThread()
            {
                if (!isInLoopThread())
                {
                    abortNotLoopThread();
                }
            }
            
            bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

        private:
            void abortNotLoopThread();

            bool looping_; /* atomic */
            const pid_t threadId_;
    };
}

#endif  // MUDUO_NET_EVENTLOOP_H