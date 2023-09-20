#include <stdio.h>
// typedef extant_type new_type; // typedef syntax
typedef enum {
    write_text; // First enum has value 0
    read_text; // Each subsequent enum has value 1 + previous
    write_binary;
    read_binary;
} operation;


int main(int argc, char *argv[]) {
    operation op;
    if(op == write_text) {
        ;
    }
    return 0;
}