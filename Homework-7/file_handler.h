#include <string>

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
    int confirm_file_name(const char* file_name);

    int parse_file(const char* file_name);

    void read_file(std::string file_name);
#endif