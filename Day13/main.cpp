#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>

//union E;
struct E{
    std::vector<E> v;
    int i;
    bool is_vec;
    int get_first_value() const {
        if (is_vec)
            return v[0].get_first_value();
        else
            return i;
    }
};

std::ostream& operator<<(std::ostream& os, const E& obj) {
    if (obj.is_vec) {
        os << '[';
        for (auto e : obj.v) {
            os << e; 
        }
        os << ']';
    }
    else {
        os << obj.i << ',';
    }
    return os;
}

std::tuple<std::vector<E>,int> parse (const std::string &line){
    //std::cout << "PARSING " << line << std::endl;
    std::vector<E> res;
    E tmp;
    for (int idx=0; idx<line.size(); idx++) {
        char l=line[idx];
        switch(l) {
            case '[':
                {
                    tmp.is_vec = true;
                    auto [v, n_idx] = parse(line.substr(idx+1));
                    tmp.v = v;
                    idx += n_idx +1;
                    res.push_back(tmp);
                    break;
                }
            case ']':
                //std::cout << "POPING " << l << " " << idx << std::endl;
                return {res, idx};
                break;
            case ',':
                break;
            default:
                tmp.is_vec = false;
                tmp.i= l - '0';
                res.push_back(tmp);
                break;
        }
    }
    return {res,line.size()};
}

bool compare(const std::vector<E> &ve1, const std::vector<E> &ve2) {
    for(int idx=0; idx<ve1.size(); idx++) {
        if (idx >= ve2.size())
            return false;
        const E &e1 = ve1[idx];
        const E &e2 = ve2[idx];
        if (e1.is_vec && e2.is_vec){
            if (!compare(e1.v, e2.v))
                return false;
        }
        else if (e1.is_vec){
            if (e1.get_first_value() > e2.i)
                return false;
        }
        else if (e2.is_vec){
            if (e1.i > e2.get_first_value())
                return false;
        }
        else {
            std::cout << "Compare " << e1.i << " and " << e2.i << std::endl;
            if(e1.i > e2.i)
                return false;
        }
    }   
    return true;
}

auto main(int argc, char** argv) -> int {
    std::string filename {argv[1]};
    std::ifstream file {filename}; 
    std::string line1, line2;
    int p1 = 0, pair_nb = 1;

    while(std::getline(file, line1)) {
        std::getline(file, line2);

        auto [e1,_n1] = parse(line1);
        auto [e2,_n2] = parse(line2);
        std::cout << "line1 " << line1 << " parsed " << std::endl;
        for(auto e: e1) {
            std::cout << e << std::endl;
        }
        std::cout << "line2 " << line2 << " parsed " << std::endl;
        for(auto e: e2) {
            std::cout << e << std::endl;
        }

        bool res = compare(e1, e2);
        if (res) {
            std::cout << "lines " << line1 << " and " << line2 << " are in order" << std::endl;
            p1 += pair_nb;  
        }
        else {
            std::cout << "lines " << line1 << " and " << line2 << " are NOT in order" << std::endl;
        }
        pair_nb++;
        // remove empty line
        std::getline(file, line2);
    }
    std::cout << "p1 " << p1 << std::endl;
}
