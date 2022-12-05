#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

template <int STACK_NB>
class Stacks {
    std::vector<char> stacks[STACK_NB];
    std::ifstream file;

    auto get_start() {
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

public:
    Stacks(std::string filename) {
        file.open(filename);
        get_start();
    }

    auto display_stacks() const {
        for(int col_idx = 0; col_idx < STACK_NB; col_idx++) {
            const auto &col = stacks[col_idx];
            for(auto e : col) {
                std::cout << e;
            }
            std::cout << '\n';
        }
    }

    auto execute_CrateMover9000() {
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

    auto execute_CrateMover9001() {
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

    void result() {
        std::cout << "Result : ";
        for(int idx = 0; idx < STACK_NB; idx++) {
            std::cout << stacks[idx].back();
        }
        std::cout << std::endl;
    }
};

auto main(int argc, char** argv) -> int {
    Stacks<9> stacks {argv[1]};
    stacks.execute_CrateMover9000();
    stacks.result();

    Stacks<9> stacks2 {argv[1]};
    stacks2.execute_CrateMover9001();
    stacks2.result();
}
