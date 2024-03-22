#include "src/utils/Singleton.h"

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
    printf("tid = %u \n", std::this_thread::get_id());
    Singleton<A>::GetInstance().print();
}

int main() {

    Singleton<A>::GetInstance().set(20);
    std::thread t1(test);
    std::thread t2(test);
    std::thread t3(test);
    std::thread t4(test);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}