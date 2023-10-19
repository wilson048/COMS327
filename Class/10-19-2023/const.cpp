#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    const int i = 0;
    cout << i << endl;
    *(int *) &i = 1;
    cout << i << endl;
    return 0;
}