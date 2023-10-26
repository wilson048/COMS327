#include <cmath>
#include <iostream>

using namespace std;

class shape {
    public:
    double perimeter() const = 0;
    double area() const = 0;
    ostream &print(ostream &o) const = 0;
} 

ostream &operator<<(ostream &o, const shape &s) {
    return s.print(o);
}

class rectangle : public shape {
    protected:
        double length, width;
    public:
        rectangle(double l, double w) {
            length = l;
            width = w;
        }
        double perimeter() const {
            return 2 * (length + width);
        }
        double area() const {
            return length * width;
        }
        ostream &print(ostream &o) const {

        }
}

int main(int argc, char *argv[]) {
    shape *s = square(4);
    cout << *s << ", perimiter=" << s->perimiter() 
        << ", area=" << s->area << endl;
    cout << (()(circle *) s)->circumference() << endl;
    delete s; 
    s = new circle(7);
    
}