#include "src/utils/Timestamp.h"
#include <cassert>
#include <cstdio>

using lhqvq::Timestamp;

void PassedByValue(Timestamp time) {
    printf("%s, %p\n", time.ToString(true).c_str(), &time);
}

void PassedByReference(const Timestamp &time) {
    printf("%s, %p\n", time.ToString(true).c_str(), &time);
}

int main() {
    Timestamp t1;
    assert(t1.GetMicroseconds() == 0);
    printf("t1: %s\n", t1.ToString().c_str());

    t1 = Timestamp::Now();
    printf("t1: %s, %p\n", t1.ToString().c_str(), &t1);

    PassedByValue(t1);
    PassedByReference(t1);

    Timestamp t2 = Timestamp::Now();
    Timestamp t3 = t2 + Timestamp(3600);
    assert(t1 < t2);
    assert(t1 != t2);
    assert(t3 > t2);
    printf("t2: %s\n", t2.ToString().c_str());
    printf("t3: %s\n", t3.ToString().c_str());

    Timestamp t4 = t3.GetDay();
    printf("t4: %s\n", t4.ToString(true).c_str());

    Timestamp t5 = Timestamp(100);
    Timestamp t6 = Timestamp(200);
    t5.swap(t6);
    assert(t5 == Timestamp(200));
    assert(t6 == Timestamp(100));

    return 0;
}