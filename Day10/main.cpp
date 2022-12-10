#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char del = ' ') {
    std::string::size_type pos = str.find(del);
    std::string::size_type posprev = 0;
    std::vector<std::string> res;
    while ( pos != std::string::npos) {
        if (pos-posprev > 0)
            res.push_back(str.substr(posprev, pos-posprev));
        posprev = pos+1;
        pos = str.find(del, posprev);
    }
    res.push_back(str.substr(posprev, std::string::npos));
    return res;
}

enum class Cmd {
    NOOP,
    ADDX
};

auto main(int argc, char** argv) -> int {
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;
        int cycle_cnt = 1;
        int regx = 1;
        int sig_stren = 0;

        while(std::getline(file, line)) {
            Cmd cmd;
            int increment;
            // parse input
            std::vector<std::string> cmd_split =  split(line);
            if (cmd_split[0].compare("noop") == 0) {
                cmd = Cmd::NOOP;
                increment = 1;
            }
            else if (cmd_split[0].compare("addx") == 0) {
                cmd = Cmd::ADDX;
                increment = 2;
            }
            // spend cycles and check barriers
            for (int i = 0; i < increment; i++) {
                if ((cycle_cnt-20) % 40 == 0 ) {
                    sig_stren += regx * cycle_cnt;
                    // std::cout << "Cycle " << cycle_cnt << ": " << "X = " << regx << " sig stren = " << regx * cycle_cnt << std::endl;
                }
                cycle_cnt++;
            }
            // execute command
            if (cmd == Cmd::ADDX) {
                regx += std::stoi(cmd_split[1]);
            }
        }
        std::cout << "P1 : " << sig_stren << std::endl;
    }
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;
        int cycle_cnt = 0;
        int regx = 1;
        int sig_stren = 0;
        bool crt[6][40];

        while(std::getline(file, line)) {
            Cmd cmd;
            int increment;
            // parse input
            std::vector<std::string> cmd_split =  split(line);
            if (cmd_split[0].compare("noop") == 0) {
                cmd = Cmd::NOOP;
                increment = 1;
            }
            else if (cmd_split[0].compare("addx") == 0) {
                cmd = Cmd::ADDX;
                increment = 2;
            }
            // spend cycles and check sprite
            for (int i = 0; i < increment; i++) {
                int col = cycle_cnt%40;
                int line = cycle_cnt/40;
                if ( col >= regx-1 && col <= regx+1)
                    crt[line][col] = true;
                else
                    crt[line][col] = false;
                cycle_cnt++;
            }
            // execute command
            if (cmd == Cmd::ADDX) {
                regx += std::stoi(cmd_split[1]);
            }
        }
        // display screen
        std::cout << "P2 :" << std::endl;
        for (int l = 0; l < 6; l++) {
            for (int c = 0; c < 40; c++) {
                if (crt[l][c]) 
                    std::cout << "▓";
                else
                    std::cout << "░";
            }
            std::cout << "\n";
        }
    }
}
