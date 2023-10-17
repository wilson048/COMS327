#include <iostream>

using namespace std;
// C style swap
void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int main(int argc, char *argv[]) {
    int i = 0, j = 1;

    std::cout << "i: " << i << "\tj: " << j << std::endl; // Scope resolution operator look inside std for variables
    swap(&i, &j);
    std::cout << "i: " << i << "\tj: " << j << std::endl; // Scope resolution operator look inside std for variables
    return 0;
}