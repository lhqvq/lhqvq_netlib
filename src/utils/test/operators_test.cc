#include "src/utils/operators.h"

#include <cstdio>
#include <cassert>

class Test : public lhqvq::LessThanComparable<Test>,
             public lhqvq::EqualityComparable<Test> {
public:
    Test() : x_(0) {}
    Test(int x) : x_(x) {}
    ~Test() = default;

    int get_x() const { return x_; }

private:
    int x_;
};

bool operator<(const Test &lhs, const Test &rhs) {
    return lhs.get_x() < rhs.get_x();
}

bool operator==(const Test &lhs, const Test &rhs) {
    return lhs.get_x() == rhs.get_x();
}

int main() {
    Test x(10);
    Test y(20);
    assert(y > x);
    assert(y >= x);
    assert(x <= y);
    assert(x != y);

    printf("All passed.\n");
    return 0;
}