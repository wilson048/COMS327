#include <iostream>

using namespace std;

class singleton {
    private:
        int value;
        static singleton *instance;
        singleton() { value = 5; }
        virtual ~singleton() {}
    public:
        static singleton *get_instance() { 
            if(!instance) {
                instance = new singleton();
            }
            return instance;
        }
        void print() {
            cout << value << endl;
        }
};

singleton *singleton::instance; // Required for any static data

int main(int argc, char *argv[]) {
    singleton *s, *t;
    s = singleton::get_instance();
    s->print();
    t = singleton::get_instance();
    t->print();
    cout << s << " " << t << " " << singleton::get_instance() << endl;
    t->print();
    return 0;
}