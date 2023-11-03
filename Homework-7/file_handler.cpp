#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <bits/stdc++.h>

#include "file_handler.h"

using namespace std;
// Global to keep track of the array file to load and read
int file_index;
// Array of valid CSV files to read
const char *correct_files[10] = {"pokemon", "pokemon_moves", "pokemon_species", "pokemon_stats", "pokemon_types", "moves", "experience", "type_names", "stats"};
/* Class for pokemon stats */
class pokemon_stats
{
public:
    int id;
    int stat_id;
    int base_stat;
    int effort;
};

/* Class for stats */
class stats
{
public:
    string id;
    string damage_class_id;
    string identifier;
    string is_battle_only;
    string game_index;
};

/* Class for names of pokemon types */
class type_names
{
public:
    int type_id;
    int language_id;
    string name;
};

/* Class for experience stats */
class experience
{
public:
    int growth_rate_id;
    int level;
    int experience;
};

/* Class for move attributes */
class moves
{
public:
    int id;
    string identifier;
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int prioirty;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
};

/* Class for what moves a single pokemon has */
class pokemon_moves
{
public:
    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;
};

/* Class for what intricacies a pokemon has*/
class pokemon_species
{
public:
    int id;
    string identifier;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;
};

/* Class for what type a pokemon is*/
class pokemon_types
{
public:
    int pokemon_id;
    int type_id;
    int slot;
};

/* Class for what a pokemon generally represents */
class pokemon
{
public:
    int id;
    string identifier;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;
};
int confirm_file_name(const char *file_name)
{
    int isValid = 0;
    int i;
    for (i = 0; i < 9; i++)
    {
        if (!(strcmp(file_name, correct_files[i])))
        {
            isValid = 1;
            file_index = i;
            break;
        }
    }
    return isValid;
}

int parse_file(const char *file_name)
{
    string f = file_name;

    // HOME path, default path is stored in file_handler.h
    string alt_path = getenv("HOME");
    alt_path += "/.poke327/pokedex/pokedex/data/csv/";

    struct stat sb;
    // ~/share/cs327/pokedex/pokedex/data/csv/
    if (!(stat(DEFAULT_PATH_ONE, &sb)))
    {
        read_file(DEFAULT_PATH_ONE + f + ".csv");
    }
    // ~/HOME/.poke327/pokedex/pokedex/data/csv/
    else if (!(stat(strcat(getenv("HOME"), "/.poke327/pokedex/pokedex/data/csv/"), &sb)))
    {
        read_file(alt_path + f + ".csv");
    }
    // ../pokedex/pokedex/data/csv/
    else if (!(stat(HARD_PATH, &sb)))
    {
        read_file(HARD_PATH + f + ".csv");
    }
    else
    {
        cout << "Database not located" << endl;
    }
    return 0;
}
void read_file(string file_name)
{
    // Load CSV
    ifstream csv_file;
    csv_file.open(file_name);

    std::string segment;
    std::vector<std::string> list;

    getline(csv_file, segment);
    // Read stats.csv
    if (!(strcmp(correct_files[file_index], "stats")))
    {
        std::vector<stats> stats_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            stats stats_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:
                    stats_obj.id = token;
                    break;
                case 1:
                    stats_obj.damage_class_id = token;
                    break;
                case 2:
                    stats_obj.identifier = token;
                    break;
                case 3:
                    stats_obj.is_battle_only = token;
                    break;
                }
                element_index++;
            }
            stats_obj.game_index = element;
            stats_vector.push_back(stats_obj);
            // Print out everything
            for (auto &element : stats_vector)
            {
                cout << "ID: " << element.id << endl;
                cout << "Damage Class ID: " << element.damage_class_id << endl;
                cout << "Identifier: " << element.identifier << endl;
                cout << "Is Battle Only: " << element.is_battle_only << endl;
                cout << "Game Index: " << element.game_index << endl;
                cout << "\n";
            }
        }
    }
    // read type_names.csv
    if (!(strcmp(correct_files[file_index], "type_names")))
    {
        std::vector<type_names> type_names_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            type_names type_names_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:
                    type_names_obj.type_id = stoi(token);
                    break;
                case 1:
                    type_names_obj.language_id = stoi(token);
                    break;
                }
                element_index++;
            }
            type_names_obj.name = element;
            type_names_vector.push_back(type_names_obj);
            // Print out everything
            for (auto &element : type_names_vector)
            {
                cout << "Type ID: " << element.type_id << endl;
                cout << "Language ID: " << element.language_id << endl;
                cout << "Name of Type: " << element.name << endl;
                cout << "\n";
            }
        }
    }
    // read expereince.csv
    if (!(strcmp(correct_files[file_index], "experience")))
    {
        std::vector<experience> experience_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            experience experience_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:
                    experience_obj.growth_rate_id = stoi(token);
                    break;
                case 1:
                    experience_obj.level = stoi(token);
                    break;
                }
                element_index++;
            }
            experience_obj.experience = stoi(element);
            experience_vector.push_back(experience_obj);
            // Print out everything
            for (auto &element : experience_vector)
            {
                cout << "Growth Rate ID: " << element.growth_rate_id << endl;
                cout << "Level: " << element.level << endl;
                cout << "Experience : " << element.experience << endl;
                cout << "\n";
            }
        }
    }
    if (!(strcmp(correct_files[file_index], "moves")))
    {
        std::vector<moves> moves_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            moves moves_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:
                    // Run regularly
                    try
                    {
                        moves_obj.id = stoi(token);
                    }
                    // Place INT_MAX in place of empty entry
                    catch (exception &e)
                    {
                        moves_obj.id = INT_MAX;
                    }
                    break;
                case 1:
                    // Run regularly
                    try
                    {
                        moves_obj.identifier = token;
                    }
                    // Place empty string in place of empty entry
                    catch (exception &e)
                    {
                        moves_obj.identifier = "";
                    }

                    break;
                case 2:
                    try
                    {
                        moves_obj.generation_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.generation_id = INT_MAX;
                    }
                    break;
                case 3:
                    try
                    {
                        moves_obj.type_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.type_id = INT_MAX;
                    }
                    break;
                case 4:
                    try
                    {
                        moves_obj.power = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.power = INT_MAX;
                    }
                    break;
                case 5:
                    try
                    {
                        moves_obj.pp = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.pp = INT_MAX;
                    }
                    break;
                case 6:
                    try
                    {
                        moves_obj.accuracy = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.accuracy = INT_MAX;
                    }
                    break;
                case 7:
                    try
                    {
                        moves_obj.prioirty = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.prioirty = INT_MAX;
                    }
                    break;
                case 8:
                    try
                    {
                        moves_obj.target_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.target_id = INT_MAX;
                    }
                    break;
                case 9:
                    try
                    {
                        moves_obj.damage_class_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.damage_class_id = INT_MAX;
                    }
                    break;
                case 10:
                    try
                    {
                        moves_obj.effect_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.effect_id = INT_MAX;
                    }
                    break;
                case 11:
                    try
                    {
                        moves_obj.effect_chance = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.effect_chance = INT_MAX;
                    }
                    break;
                case 12:
                    try
                    {
                        moves_obj.contest_type_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.contest_type_id = INT_MAX;
                    }
                    break;
                case 13:
                    try
                    {
                        moves_obj.contest_effect_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        moves_obj.contest_effect_id = INT_MAX;
                    }
                    break;
                }
                element_index++;
            }
            try
            {
                moves_obj.super_contest_effect_id = stoi(element);
            }
            catch (exception &e)
            {
                moves_obj.super_contest_effect_id = INT_MAX;
            }

            moves_vector.push_back(moves_obj);
            // Print out everything
            for (auto &element : moves_vector)
            {
                cout << "ID: " << (element.id == INT_MAX ? "" : std::to_string(element.id)) << endl;
                cout << "Identifier: " << element.identifier << endl;
                cout << "Generation ID: " << (element.generation_id == INT_MAX ? "" : std::to_string(element.generation_id)) << endl;
                cout << "Type ID: " << (element.type_id == INT_MAX ? "" : std::to_string(element.type_id)) << endl;
                cout << "Power: " << (element.power == INT_MAX ? "" : std::to_string(element.power)) << endl;
                cout << "PP: " << (element.pp == INT_MAX ? "" : std::to_string(element.pp)) << endl;
                cout << "Accuracy: " << (element.accuracy == INT_MAX ? "" : std::to_string(element.accuracy)) << endl;
                cout << "Prioirty: " << (element.prioirty == INT_MAX ? "" : std::to_string(element.prioirty)) << endl;
                cout << "Target ID: " << (element.target_id == INT_MAX ? "" : std::to_string(element.target_id)) << endl;
                cout << "Damage Class ID: " << (element.damage_class_id == INT_MAX ? "" : std::to_string(element.damage_class_id)) << endl;
                cout << "Effect ID: " << (element.effect_id == INT_MAX ? "" : std::to_string(element.effect_id)) << endl;
                cout << "Effect Chance: " << (element.effect_chance == INT_MAX ? "" : std::to_string(element.effect_chance)) << endl;
                cout << "Contest Type ID: " << (element.contest_type_id == INT_MAX ? "" : std::to_string(element.contest_type_id)) << endl;
                cout << "Contest Effect ID: " << (element.contest_effect_id == INT_MAX ? "" : std::to_string(element.contest_effect_id)) << endl;
                cout << "Super Contest Type ID: " << (element.super_contest_effect_id == INT_MAX ? "" : std::to_string(element.super_contest_effect_id)) << endl;
                cout << "\n";
            }
        }
    }
    if (!(strcmp(correct_files[file_index], "pokemon_moves")))
    {
        std::vector<pokemon_moves> pokemon_moves_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            pokemon_moves pokemon_moves_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:
                    try
                    {
                        pokemon_moves_obj.pokemon_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_moves_obj.pokemon_id = INT_MAX;
                    }
                    break;
                case 1:
                    try
                    {
                        pokemon_moves_obj.version_group_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_moves_obj.version_group_id = INT_MAX;
                    }
                    break;
                case 2:
                    try
                    {
                        pokemon_moves_obj.move_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_moves_obj.move_id = INT_MAX;
                    }
                    break;
                case 3:
                    try
                    {
                        pokemon_moves_obj.pokemon_move_method_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_moves_obj.pokemon_move_method_id = INT_MAX;
                    }
                    break;
                case 4:
                    try
                    {
                        pokemon_moves_obj.level = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_moves_obj.level = INT_MAX;
                    }
                }
                element_index++;
            }
            try
            {
                pokemon_moves_obj.order = stoi(token);
            }
            catch (exception &e)
            {
                pokemon_moves_obj.order = INT_MAX;
            }
            pokemon_moves_vector.push_back(pokemon_moves_obj);
            // Print out everything
            for (auto &element : pokemon_moves_vector)
            {
                cout << "Pokemon ID: " << (element.pokemon_id == INT_MAX ? "" : std::to_string(element.pokemon_id)) << endl;
                cout << "Version Group ID: " << (element.version_group_id == INT_MAX ? "" : std::to_string(element.version_group_id)) << endl;
                cout << "Move ID: " << (element.move_id == INT_MAX ? "" : std::to_string(element.move_id)) << endl;
                cout << "Move Method ID: " << (element.pokemon_move_method_id == INT_MAX ? "" : std::to_string(element.pokemon_move_method_id)) << endl;
                cout << "Level: " << (element.level == INT_MAX ? "" : std::to_string(element.level)) << endl;
                cout << "Order: " << (element.order == INT_MAX ? "" : std::to_string(element.order)) << endl;
                cout << "\n";
            }
        }
    }
    if (!(strcmp(correct_files[file_index], "pokemon_stats")))
    {
        std::vector<pokemon_stats> pokemon_stats_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            pokemon_stats pokemon_stats_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:

                    pokemon_stats_obj.id = stoi(token);
                    break;
                case 1:
                    pokemon_stats_obj.stat_id = stoi(token);
                    break;
                case 2:
                    pokemon_stats_obj.base_stat = stoi(token);
                    break;
                }
                element_index++;
            }
            try
            {
                pokemon_stats_obj.effort = stoi(token);
            }
            catch (exception &e)
            {
                pokemon_stats_obj.effort = INT_MAX;
            }
            pokemon_stats_vector.push_back(pokemon_stats_obj);
            // Print out everything
            for (auto &element : pokemon_stats_vector)
            {
                cout << "ID: " << (element.id == INT_MAX ? "" : std::to_string(element.id)) << endl;
                cout << "Stat ID: " << (element.stat_id == INT_MAX ? "" : std::to_string(element.stat_id)) << endl;
                cout << "Base Stat: " << (element.base_stat == INT_MAX ? "" : std::to_string(element.base_stat)) << endl;
                cout << "Effort: " << (element.effort == INT_MAX ? "" : std::to_string(element.effort)) << endl;
                cout << "\n";
            }
        }
    }
    if (!(strcmp(correct_files[file_index], "pokemon_species")))
    {
        std::vector<pokemon_species> pokemon_species_vector;
        // Put all lines of file into list
        while (getline(csv_file, segment))
        {
            list.push_back(segment);
        }

        // For all lines from the file, divide each CSV into individual values
        for (auto &element : list)
        {
            pokemon_species pokemon_species_obj;
            // Comma separator
            std::string comma_sep = ",";

            size_t pos = 0;
            std::string token;

            int element_index = 0;

            while ((pos = element.find(comma_sep)) != std::string::npos)
            {
                token = element.substr(0, pos);
                element.erase(0, pos + comma_sep.length());
                // Assign data to correct areas
                switch (element_index)
                {
                case 0:
                    // Run regularly
                    try
                    {
                        pokemon_species_obj.id = stoi(token);
                    }
                    // Place INT_MAX in place of empty entry
                    catch (exception &e)
                    {
                        pokemon_species_obj.id = INT_MAX;
                    }
                    break;
                case 1:
                    // Run regularly
                    try
                    {
                        pokemon_species_obj.identifier = token;
                    }
                    // Place empty string in place of empty entry
                    catch (exception &e)
                    {
                        pokemon_species_obj.identifier = "";
                    }

                    break;
                case 2:
                    try
                    {
                        pokemon_species_obj.generation_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.generation_id = INT_MAX;
                    }
                    break;
                case 3:
                    try
                    {
                        pokemon_species_obj.evolves_from_species_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.evolves_from_species_id = INT_MAX;
                    }
                    break;
                case 4:
                    try
                    {
                        pokemon_species_obj.evolution_chain_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.evolution_chain_id = INT_MAX;
                    }
                    break;
                case 5:
                    try
                    {
                        pokemon_species_obj.color_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.color_id = INT_MAX;
                    }
                    break;
                case 6:
                    try
                    {
                        pokemon_species_obj.shape_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.shape_id = INT_MAX;
                    }
                    break;
                case 7:
                    try
                    {
                        pokemon_species_obj.habitat_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.habitat_id = INT_MAX;
                    }
                    break;
                case 8:
                    try
                    {
                        pokemon_species_obj.gender_rate = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.gender_rate = INT_MAX;
                    }
                    break;
                case 9:
                    try
                    {
                        pokemon_species_obj.capture_rate = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.capture_rate = INT_MAX;
                    }
                    break;
                case 10:
                    try
                    {
                        pokemon_species_obj.base_happiness = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.base_happiness = INT_MAX;
                    }
                    break;
                case 11:
                    try
                    {
                        pokemon_species_obj.is_baby = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.is_baby = INT_MAX;
                    }
                    break;
                case 12:
                    try
                    {
                        pokemon_species_obj.hatch_counter = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.hatch_counter = INT_MAX;
                    }
                    break;
                case 13:
                    try
                    {
                        pokemon_species_obj.has_gender_differences = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.has_gender_differences = INT_MAX;
                    }
                    break;
                case 14:
                    try
                    {
                        pokemon_species_obj.growth_rate_id = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.growth_rate_id = INT_MAX;
                    }
                    break;
                case 15:
                    try
                    {
                        pokemon_species_obj.forms_switchable = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.forms_switchable = INT_MAX;
                    }
                    break;
                case 16:
                    try
                    {
                        pokemon_species_obj.is_legendary = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.is_legendary = INT_MAX;
                    }
                    break;
                case 17:
                    try
                    {
                        pokemon_species_obj.is_mythical = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.is_mythical = INT_MAX;
                    }
                    break;
                case 18:
                    try
                    {
                        pokemon_species_obj.order = stoi(token);
                    }
                    catch (exception &e)
                    {
                        pokemon_species_obj.order = INT_MAX;
                    }
                    break;
                }
                element_index++;
            }
            try
            {
                pokemon_species_obj.conquest_order = stoi(element);
            }
            catch (exception &e)
            {
                pokemon_species_obj.conquest_order = INT_MAX;
            }

            pokemon_species_vector.push_back(pokemon_species_obj);
            // Print out everything
            for (auto &element : pokemon_species_vector)
            {
                cout << "ID: " << (element.id == INT_MAX ? "" : std::to_string(element.id)) << endl;
                cout << "Identifier: " << element.identifier << endl;
                cout << "Generation ID: " << (element.generation_id == INT_MAX ? "" : std::to_string(element.generation_id)) << endl;
                cout << "Evolves From Species ID: " << (element.evolves_from_species_id == INT_MAX ? "" : std::to_string(element.evolves_from_species_id)) << endl;
                cout << "Evolution Chain ID: " << (element.evolution_chain_id == INT_MAX ? "" : std::to_string(element.evolution_chain_id)) << endl;
                cout << "Color ID: " << (element.color_id == INT_MAX ? "" : std::to_string(element.color_id)) << endl;
                cout << "Shape ID: " << (element.shape_id == INT_MAX ? "" : std::to_string(element.shape_id)) << endl;
                cout << "Habitat ID: " << (element.habitat_id == INT_MAX ? "" : std::to_string(element.habitat_id)) << endl;
                cout << "Gender Rate: " << (element.gender_rate == INT_MAX ? "" : std::to_string(element.gender_rate)) << endl;
                cout << "Capture Rate: " << (element.capture_rate == INT_MAX ? "" : std::to_string(element.capture_rate)) << endl;
                cout << "Base Happiness: " << (element.base_happiness == INT_MAX ? "" : std::to_string(element.base_happiness)) << endl;
                cout << "Is Baby: " << (element.is_baby == INT_MAX ? "" : std::to_string(element.is_baby)) << endl;
                cout << "Hatch Counter: " << (element.hatch_counter == INT_MAX ? "" : std::to_string(element.hatch_counter)) << endl;
                cout << "Has Gender Differences: " << (element.has_gender_differences == INT_MAX ? "" : std::to_string(element.has_gender_differences)) << endl;
                cout << "Growth Rate ID: " << (element.growth_rate_id == INT_MAX ? "" : std::to_string(element.growth_rate_id)) << endl;
                cout << "Forms Switchable: " << (element.forms_switchable == INT_MAX ? "" : std::to_string(element.forms_switchable)) << endl;
                cout << "Is Legendary: " << (element.is_legendary == INT_MAX ? "" : std::to_string(element.is_legendary)) << endl;
                cout << "Is Mythical: " << (element.is_mythical == INT_MAX ? "" : std::to_string(element.is_mythical)) << endl;
                cout << "Order: " << (element.order == INT_MAX ? "" : std::to_string(element.order)) << endl;
                cout << "Conquest Order: " << (element.conquest_order == INT_MAX ? "" : std::to_string(element.conquest_order)) << endl;
                cout << "\n";
            }
        }
    }
}