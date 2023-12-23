#ifndef MUDOU_BASE_ATOMIC_H
#define MUDOU_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace muduo
{
    namespace detail
    {
        template <typename T>
        class AtomicIntegerT : boost::noncopyable
        {
        public:
            AtomicIntegerT()
                : value_(0)
            {
            }

            /*uncomment if you need copying and assignment*/

            AtomicIntegerT(const AtomicIntegerT &rhs)
                : value_(rhs.get())
            {
            }

            AtomicIntegerT &operator=(const AtomicIntegerT &rhs)
            {
                getAndSet(rhs.get());
                return *this;
            }

            T get() const
            {
                return __sync_val_compare_and_swap(const_cast<volatile T *>(&value_), 0, 0);
            }

            T getAndAdd(T x)
            {
                return __sync_fetch_and_add(&value_, x);
            }

            T addAndGet(T x)
            {
                return getAndAdd(x) + x;
            }

            T incrementAndGet()
            {
                return addAndGet(1);
            }
            void add(T x)
            {
                getAndAdd(x);
            }

            void increment()
            {
                incrementAndGet();
            }

            void decrement()
            {
                getAndAdd(-1);
            }

            T getAndSet(T newValue)
            {
                return __sync_lock_test_and_set(&value_, newValue);
            }

        private:
            volatile T value_; /*当要求使用volatile声明变量的值的时候，系统总是从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据。*/
        };
    }

    typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
    typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif // MUDUO_BASE_ATOMIC_H