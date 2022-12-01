#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <algorithm>

auto get_elf_calories(std::ifstream &file) {
    std::vector<int> elf_calories_out;
    std::string line;

    do {
        if(std::getline(file, line) && size(line) > 0 ) {
            elf_calories_out.push_back(std::stoi(line));
        }
        else
            break;
    } while (true);

    return elf_calories_out;
} 

auto main() -> int {
   
    std::vector<int> elf_calories_out;
    std::vector<int> elves;
    //std::ifstream myfile {"test.txt"}; 
    std::ifstream myfile {"input"}; 

    do {
        elf_calories_out = get_elf_calories(myfile);
        
//        for(auto e: elf_calories_out) {
//            std::cout << e << std::endl;
//        }

        auto elf_tot_cal = std::accumulate(elf_calories_out.begin(), elf_calories_out.end(), 0);

        if(elf_tot_cal != 0) {
            // std::cout << "Total: " <<  elf_tot_cal << std::endl;
            elves.push_back(elf_tot_cal);
        }
        else
            break;
    } while(1);

    auto max_elem = std::max_element( elves.begin(), elves.end() );

    std::cout << "Max: " <<  *max_elem << std::endl;

    std::sort(elves.begin(), elves.end(), std::greater<int>() );

    std::cout << "Top three: " <<  elves[0] + elves[1] + elves[2] << std::endl;
    
    return 0;
}
