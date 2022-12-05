#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

template<int STACK_NB>
auto display_stacks(std::vector<char> stacks[]) {
    for(int col_idx = 0; col_idx < STACK_NB; col_idx++) {
        const auto &col = stacks[col_idx];
        for(auto e : col) {
            std::cout << e;
        }
        std::cout << '\n';
    }
}

template<int STACK_NB>
auto get_start(std::ifstream &file, std::vector<char> stacks[]) {
    std::string line;
    bool cont = true;

    while (cont) {
        if(std::getline(file, line)) {
            for (int col = 0; col < STACK_NB; col++) {
                char idx = line[1+col*4];
                if (idx == '1') {
                    cont = false;
                    break;
                }
                if (idx != ' ')
                    stacks[col].push_back(idx);
            }
        }
    }
    for(int i = 0; i< STACK_NB; i++) {
        std::reverse(stacks[i].begin(), stacks[i].end());
    }

    return 0;
}

void move_stack(int nb, int from, int to, std::vector<char> stacks[]) {
    for(int i = 0; i<nb; i++) {
        stacks[to-1].push_back(stacks[from-1].back());
        stacks[from-1].pop_back();
    }
}

void move_stack2(int nb, int from, int to, std::vector<char> stacks[]) {
    std::vector<char> crane;
    for(int i = 0; i<nb; i++) {
        crane.push_back(stacks[from-1].back());
        stacks[from-1].pop_back();
    }
    for(int i = 0; i<nb; i++) {
        stacks[to-1].push_back(crane.back());
        crane.pop_back();
    }
}

auto execute_crane(std::ifstream &file, std::vector<char> stacks[]) {
    const std::regex regexp("move ([0-9]+) from ([0-9]+) to ([0-9]+)");
    std::smatch match_base;
    std::string line;

    while (true) {
        if(std::getline(file, line)) {
            if (std::regex_search(line, match_base, regexp)) {
                if (match_base.size() == 4) {
                    int nb = stoi(match_base[1].str());
                    int from = stoi(match_base[2].str());
                    int to = stoi(match_base[3].str());
                    move_stack(nb, from, to, stacks);
                }
            }
        }
        else {
            break;
        }
    }
}

auto execute_crane2(std::ifstream &file, std::vector<char> stacks[]) {
    const std::regex regexp("move ([0-9]+) from ([0-9]+) to ([0-9]+)");
    std::smatch match_base;
    std::string line;

    while (true) {
        if(std::getline(file, line)) {
            if (std::regex_search(line, match_base, regexp)) {
                if (match_base.size() == 4) {
                    int nb = stoi(match_base[1].str());
                    int from = stoi(match_base[2].str());
                    int to = stoi(match_base[3].str());
                    move_stack2(nb, from, to, stacks);
                }
            }
        }
        else {
            break;
        }
    }
}

auto main(int argc, char** argv) -> int {
    {
        const int STACK_NB = 9;
        std::string filename {argv[1]};
        std::ifstream file {filename};
        std::vector<char> stacks[STACK_NB];

        get_start<STACK_NB>(file, stacks);
        execute_crane(file, stacks);

        std::cout << "Result : ";
        for(int idx = 0; idx < STACK_NB; idx++) {
            std::cout << stacks[idx].back();
        }
        std::cout << std::endl;
    }
    {
        const int STACK_NB = 9;
        std::string filename {argv[1]};
        std::ifstream file {filename};
        std::vector<char> stacks[STACK_NB];

        get_start<STACK_NB>(file, stacks);
        execute_crane2(file, stacks);

        std::cout << "Result : ";
        for(int idx = 0; idx < STACK_NB; idx++) {
            std::cout << stacks[idx].back();
        }
        std::cout << std::endl;
    }
}
