#include <iostream>

using namespace std;

class A {
    public:
        A() { cout << __PRETTY_FUNCTION__ << endl; }
        A(const A &a) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~A() { cout << __PRETTY_FUNCTION__ << endl; }
        void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class B : virtual public A {
    public:
        B() { cout << __PRETTY_FUNCTION__ << endl; }
        B(const B &b) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~B() { cout << __PRETTY_FUNCTION__ << endl; }
        void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class C : virtual public A {
    public:
        C() { cout << __PRETTY_FUNCTION__ << endl; }
        C(const C &c) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~C() { cout << __PRETTY_FUNCTION__ << endl; }
        void print() { cout << __PRETTY_FUNCTION__ << endl; }
};
// Use virtual to stop making copies of superclasses
class D : virtual public B, virtual public C {
    public:
        D() { cout << __PRETTY_FUNCTION__ << endl; }
        D(const D &d) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~D() { cout << __PRETTY_FUNCTION__ << endl; }
        void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

int main(int argc, char *argv[]) {
    D a;
    // a.B::A::print();
    // ((B *) &a)->A::print();
    a.print();
    a.C::print();
    a.B::print();
    a.A::print();
    return 0;
}