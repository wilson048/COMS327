#include <string>

#ifndef FILE_HANDLER_H
/* The required default path*/
#define DEFAULT_PATH_ONE "/share/cs327/pokedex/pokedex/data/csv/"
/* My hard path to my local database */
#define HARD_PATH "../pokedex/pokedex/data/csv/"
#define FILE_HANDLER_H
    int confirm_file_name(const char* file_name);

    int parse_file(const char* file_name);

    void read_file(std::string file_name);
#endif