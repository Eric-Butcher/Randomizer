#include <iostream>
#include <getopt.h>

// Extremely rudimentary help message for now. TODO: replace later

struct Range {
    int min;
    int max;
};

Range get_range_from_args(int argc, char **argv){
    if (argc != 3){
        throw std::invalid_argument("Incorrect number of arguments. ");
    }

    std::string min_str = std::string(argv[1]);
    std::string max_str = std::string(argv[2]);

    // can throw an exception
    Range range;
    range.min = std::stoi(min_str);
    range.max = std::stoi(max_str);
    return range;
}

void print_help_message(){
    std::string_view help = "Usage: randomizer <min> <max>\n";
    std::cout << help << std::endl;
}


int main(int argc, char *argv[]){
    Range range;
    try {
         range = get_range_from_args(argc, argv);
    } catch (std::invalid_argument& e){
        print_help_message();
        exit(1);
    } catch (std::out_of_range& e){
        print_help_message();
        exit(1);
    }

    return 0;
    
}