#include <iostream>

using namespace std;

typedef void (*fp2)();
typedef fp2 (*fp1)();

// C style multiple parens
void f2 () {
    cout << __PRETTY_FUNCTION__ << endl;
}

fp2 f1() {
    cout << __PRETTY_FUNCTION__ << endl;
    return f2;
}

fp1 f0 () {
    cout << __PRETTY_FUNCTION__ << endl;
    return f1;
}
// C++ style multiple parens
class infinite_parens {
    public:
        infinite_parens &operator()() {
            cout << __PRETTY_FUNCTION__ << endl;
            return *this;
        }
};

int main(int argc, char *argv[]) {
    f0()()();

    infinite_parens ip;

    ip()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()
    ()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()();
    return 0;
}