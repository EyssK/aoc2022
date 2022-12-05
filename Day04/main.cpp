#include <iostream>
#include <fstream>
#include <string>
#include <regex>

auto get_ranges(const std::string &ranges) {
    const std::regex regexp("([0-9]+)-([0-9]+),([0-9]+)-([0-9]+)");
    std::smatch match_base;

    if (std::regex_search(ranges, match_base, regexp)) {
        if (match_base.size() == 5) {
            return std::tuple<int,int,int,int>{
                stoi(match_base[1].str()),
                stoi(match_base[2].str()),
                stoi(match_base[3].str()),
                stoi(match_base[4].str())
            };
        } else {
            throw std::string{"Regex didn't find the 4 values"};
        }
    } else {
        throw std::string{"Regex didn't find the pattern"};
    }
    return std::tuple<int,int,int,int>{0,0,0,0};
}

auto main(int argc, char** argv) -> int {
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;
        int total = 0;
    
        while(true) {
            if(std::getline(file, line)) {
                //std::cout << "####" << std::endl << line << std::endl;
                try {
                    auto [s1, e1, s2, e2] = get_ranges(line);
                    if ( s1 >= s2 && e1 <= e2 ) {
                        // 1 fully contained in 2
                        total += 1;
                    } else if ( s2 >= s1 && e2 <= e1 ){
                        // 2 fully contained in 1
                        total += 1;
                    }
                }
                catch (std::string msg) {
                    std::cout << "get_ranges error : " << line << "\n\t" << msg << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Finito " << total << std::endl;
                break;
            }
        }
    }
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;
        int total = 0;
    
        while(true) {
            if(std::getline(file, line)) {
                //std::cout << "####" << std::endl << line << std::endl;
                try {
                    auto [s1, e1, s2, e2] = get_ranges(line);
                    if ( e1 < s2 || e2 < s1 )
                        // no overlap
                        continue;
                    total += 1;
                }
                catch (std::string msg) {
                    std::cout << "get_ranges error : " << line << "\n\t" << msg << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Finito " << total << std::endl;
                break;
            }
        }
    }
}
