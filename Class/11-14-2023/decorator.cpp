#include <iostream>

using namespace std;

class shape {
    public:
        virtual ~shape() {}
        virtual void draw() = 0;
};

class circle : public shape {
    public:
        void draw() {
            cout << "()" << endl;
        }
};

class square : public shape {
    public:
        void draw() {
            cout << "[]" << endl;
        }
};

class green_circle : public circle {
    public:
        void draw() {
            cout << "green-()" << endl;
        }
};

class large_circle : public circle {
    public:
        void draw() {
            cout << "large-()" << endl;
        }
};
// Decorators are especially useful when there is no access to the base class
// Allows the user to use a has-a relationship to add functinality to the base and all derived classes by wrapping
// that functionality to the base class, rather than adding it driectly within the class. Obviously, its usually (always?) better
// to add the functionality directly to the base class if that option is available
class shape_decorator : public shape {
    private:
        shape *s;
    public:
        shape_decorator(shape *s) : s(s) {}
        ~shape_decorator() { delete s; }
        void draw() { s->draw(); }
};
class green_shape : public shape_decorator {
    public:
        green_shape(shape *s) : shape_decorator(s) {}
        void draw() {
            cout << "green-";
            shape_decorator::draw();
        }
};

class large_shape : public shape_decorator {
    public:
        large_shape(shape *s) : shape_decorator(s) {}
        void draw() {
            cout << "large-";
            shape_decorator::draw();
        }
};
int main(int argc, char *argv[]) {
    green_shape gc(new circle);
    green_shape gs(new square);
    large_shape lgc(new large_shape(new green_shape(new green_shape(new green_circle))));
    gs.draw(); 
    gc.draw();
    lgc.draw();
    return 0;
}