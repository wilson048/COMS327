#include <iostream>

using namespace std;

class singleton {
    private:
        static singleton *instance;
        singleton() {}
    public:
        static singleton *get_instance() { 
            if(!instance) {
                instance = new singleton();
            }
            return instance;
        }
};

int main(int argc, char *argv[]) {

    return 0;
}