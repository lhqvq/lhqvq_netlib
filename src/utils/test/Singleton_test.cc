#include "src/utils/Singleton.h"
#include "src/thread/Thread.h"
#include "src/thread/CurrentThread.h"

#include <cstdio>
#include <thread>

using namespace lhqvq;

class A {
public:
    A() : val_(0) {}
    ~A() = default;

    void set(int x) { val_ = x; }
    void print() {printf("val_ = %d, %p\n", val_, this);}
private:
    int val_;
};

void test() {
    printf("tid = %d, ", current_thread::Tid());
    Singleton<A>::GetInstance().print();
}

int main() {

    Singleton<A>::GetInstance().set(99);
    Thread t1(test, "t1");
    Thread t2(test, "t2");
    Thread t3(test, "t3");
    Thread t4(test, "t4");

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();

    return 0;
}