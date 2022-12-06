#include <iostream>
#include <fstream>
#include <string>
#include <set>
//#include <regex>
//#include <algorithm>

int get_marker_pos(std::string input) {
    for (int i = 0; i<input.size(); i++) {
        std::set<char> marker {input[i],input[i+1],input[i+2],input[i+3]};
        if (marker.size() == 4) {
            for(auto e : marker)
                std::cout << e << std::endl;
            return i+4;
        }
    }
    return 0;
}

int get_message_pos(std::string input) {
    for (int i = 0; i<input.size(); i++) {
        std::set<char> marker {
            input[i],input[i+1],input[i+2],input[i+3],
            input[i+4],input[i+5],input[i+6],input[i+7],
            input[i+8],input[i+9],input[i+10],input[i+11],
            input[i+12],input[i+13]
        };
        if (marker.size() == 14) {
            for(auto e : marker)
                std::cout << e << std::endl;
            return i+14;
        }
    }
    return 0;
}

auto main(int argc, char** argv) -> int {
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;

        if(std::getline(file, line)) {
            std::cout << get_marker_pos(line) <<  std::endl;
        }
    }
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;

        if(std::getline(file, line)) {
            std::cout << get_message_pos(line) <<  std::endl;
        }
    }
    return 0;
}
