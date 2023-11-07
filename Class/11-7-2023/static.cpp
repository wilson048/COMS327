#include <iostream>

using namespace std;

class static_demo {
    private:
        static int num_instances;
    public:
        static_demo() { num_instances++; }
        virtual ~static_demo() {}
        static int get_num_instances() { return num_instances; }
};
int static_demo::num_instances = 0;

int main(int argc, char *argv[]) {
    static_demo d1;
    static_demo *d;
    int i;

    cout << d1.get_num_instances() << endl;
    for(i = 0; i < 1000; i++) {
        d = new static_demo();
    }
    cout << static_demo::get_num_instances() << endl;
    cout << d->get_num_instances() << endl;
    return 0;
}