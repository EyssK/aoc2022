#include <iostream>
#include <fstream>
#include <string>

auto compares_sack(const std::string &sack) {
    const unsigned int half_len = sack.size()/2;
    for(auto e = sack.begin(); e < sack.begin() + half_len; e++) {
        if (sack.find(*e, half_len) != std::string::npos) {
            return *e;
        }
    }
}

auto find_badge(const std::string &sack1, const std::string &sack2, const std::string &sack3) {
    for(auto e : sack1) {
        if (sack2.find(e) != std::string::npos && sack3.find(e) != std::string::npos) {
            return e;
        }
    }
}

int evaluate_priority(const char e) {
    if(e >= 'a' && e <= 'z')
        return e-'a'+1;
    else if (e >= 'A' && e <= 'Z')
        return e-'A'+27;
    else
        throw "Not a letter";
    return 0;
}

auto main() -> int {
    {
        std::ifstream file {"input"}; 
        std::string line;
        int total = 0;
    
        while(true) {
            if(std::getline(file, line)) {
                total += evaluate_priority(compares_sack(line));
            } else {
                std::cout << "Finito " << total << std::endl;
                break;
            }
        }
    }

    {
        std::ifstream file {"input"}; 
        std::string line1;
        std::string line2;
        std::string line3;
        int total = 0;
    
        while(true) {
            if(std::getline(file, line1)) {
                std::getline(file, line2);
                std::getline(file, line3);
                total += evaluate_priority(find_badge(line1,line2,line3));
            } else {
                std::cout << "Finito " << total << std::endl;
                break;
            }
        }
    }
}
