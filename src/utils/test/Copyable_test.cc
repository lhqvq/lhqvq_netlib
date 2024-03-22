#include "src/utils/Copyable.h"
#include "src/utils/Uncopyable.h"

#include <cstdio>
#include <cassert>

class Test1 : public lhqvq::Copyable {
public:
    Test1() : val_(0) {}
    Test1(int x) : val_(x) {}
    ~Test1() = default;

    int get() { return val_; }

private:
    int val_;
};

class Test2 : public lhqvq::Uncopyable {
public:
    Test2() : val_(0) {}
    Test2(int x) : val_(x) {}
    ~Test2() = default;

    int get() { return val_; }

private:
    int val_;
};
class Test3 : Test2 {};

int main() {

    // ====================  Test1  ====================
    Test1 t1(20);
    Test1 t2 = t1;
    Test1 t3;

    assert(t2.get() == 20);
    assert(t3.get() == 0);

    t3 = t2;
    assert(t3.get() == 20);
    // ====================  Test1  ====================


    // ====================  Test2  ====================
    Test2 t4(20);
    // Test2 t5(t4);
    Test2 t6;
    // t6 = t4;
    // ====================  Test2  ====================


    // ====================  Test3  ====================
    Test3 t7;
    // Test3 t8(t7);
    Test3 t9;
    // t9 = t7;
    // ====================  Test3  ====================

    printf("All Passed");

    return 0;
}