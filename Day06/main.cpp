#include <iostream>
#include <fstream>
#include <string>
#include <set>

template <int DEPTH>
int get_marker_pos(std::string input) {
    for (int i = 0; i<input.size(); i++) {
        std::set<char> marker;
        for (int d = 0; d<DEPTH; d++) {
            marker.insert(input[i+d]);
        }
        if (marker.size() == DEPTH) {
            for(auto e : marker)
                std::cout << e << std::endl;
            return i+DEPTH;
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
            std::cout << get_marker_pos<4>(line) <<  std::endl;
        }
    }
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;

        if(std::getline(file, line)) {
            std::cout << get_marker_pos<14>(line) <<  std::endl;
        }
    }
    return 0;
}
