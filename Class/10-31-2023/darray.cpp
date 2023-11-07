#include <iostream>

#include "darray.h"

using namespace std;

int main(int argc, char *argv[]) {
    darray<int> ai;
    darray<string> as;
    int i;
    for(i = 0; i < 100; i++) {
        ai.add(i);
    }
    cout << ai << endl;
    ai.remove(42);

    ai.set(10, 25);
    ai[5] = 50;
    ai.get(75) = 25;

    cout << ai << endl;
    as.add("Hello0");
    cout << as << endl;
    try {
        for(i = 0;; i++) {
            ai[i] = i;
            cout << ai[i] << endl;
        }
    }
    catch(const char *s) {
        cout << s << endl;
    }
    catch (...) {  // Catches anything, must be last
        cout << "Caught an exception" << endl;
    }
    return 0;
}