#include <iostream>
#include "../recipes/thread/Thread.h"

void Threadprocess()
{
    std::cout << "this is a thread" << std::endl;
}

int main()
{
    muduo::Thread t1(Threadprocess);
    std::cout << "this is a main thread" << std::endl;

}