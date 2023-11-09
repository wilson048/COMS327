#include <algorithm>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

bool is_42(int i) {
    return i == 42;
}

bool is_67(int i) {
    return i == 67;
}

class is_equal {
    private:
        int value;
    public:
        is_equal(int i) : value(i) {}
        bool operator(int)(int i) { return i == value; }
}
int main(int argc, char *argv[]) {
    vector<int> v;
    vector<int>::iterator vi, s, e;
    int i;
    is_equal ie50(50);
    
    srand(time(NULL));

    for(i = 0; i < 1000; i++) {
        v.push_back(rand() % 100);
    }

    for(s = v.begin(), e = v.end(); s != e;) {
        s = find_if(s, e, ie50);
        if(s != v.end()) {
            cout << *s << endl;
            s++;
        }
    }

    // for(s = v.begin(), e = v.end(); s != e;) {
    //     s = find_if(s, e, is_67);
    //     if(s != v.end()) {
    //         cout << *s << endl;
    //         s++;
    //     }
    // }
    // vi = find_if(v.begin(), v.end(), is_42);
    
    // else {
    //     cout << "Not found." << endl;
    // }
    return 0;
}