#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

//#define LOG_FILE std::cout
#define LOG_FILE log_file
std::ofstream log_file {"log.txt"}; 

class File {
public:
    const int size;
    const std::string name;
    File(std::string name, int size): size{size}, name{name} {
        LOG_FILE << "Building File " << name << std::endl;
    }
    size_t get_size() const { return this->size; }
};

class Directory {
public:
    std::map<std::string,Directory> dirs;
    std::vector<File*> files;
    const std::string name;
    Directory(std::string name): name{name} {
        LOG_FILE << "Building Directory " << name << std::endl;
    }
    void add_dir(Directory e) {
        this->dirs.insert({e.name, e});
    }
    void add_file(File e) {
        File* n = new File{e};
        this->files.push_back(n);
    }
    size_t get_size() const {
        size_t size = 0;
        for(const auto [name,dir]: dirs) {
           size += dir.get_size(); 
        }
        for(auto e: files) {
           size += e->get_size(); 
        }
        return size;
    }
    size_t get_sub100000_dirs() {
        size_t all_size = 0;
        size_t size = get_size();
        if (size < 100000)
            all_size += size;
        for (auto [name,dir] : this->dirs) {
            all_size += dir.get_sub100000_dirs();
        }
        return all_size;
    }
    void get_free_dirs(int size_to_free, std::vector<size_t>&big_enough){
        size_t size = get_size();
        if (size > size_to_free) {
            LOG_FILE << name << " " << size << "\n";
            big_enough.push_back(size);
        }
        for (auto [name,dir] : this->dirs) {
            dir.get_free_dirs(size_to_free,big_enough);
        }
    }

    Directory *cd(const std::string &name) {
        if (!this->dirs.contains(name)) {
            std::cout << "cd error, dir "<<name<<" not found " << std::endl << "\t";
            for (auto [n,d] : dirs) {
                std::cout << n << " ";
            }
            std::cout << "\n";
            throw ("Error cd range");
        }
        return &(dirs.at(name));
    }
};

class Pointer {
    Directory* position;
    std::vector<Directory*> upper;
    Directory* root;
    void ls() const{
        for(const auto [name,dir] : position->dirs)
            std::cout << "Dir " <<  name << std::endl;
        for(const auto e : position->files)
            std::cout << "File " <<  e->name << std::endl;
    }
public:
    Pointer(Directory& d) : position{&d}, upper{&d}, root{&d} {}
    void cd(const std::string &name) {
        if (name.compare("..") == 0) {
            position = upper.back();
            upper.pop_back();
        }
        else if (name.compare("/") == 0) {
            position = root;
            upper.erase(upper.begin(), upper.end());
            upper.push_back(root);
        }
        else {
            upper.push_back(position);
            position = position->cd(name);
        }
        LOG_FILE << "Moving to " << position->name << "\n";
    }
    void add_dir(Directory e) const {
        position->add_dir(e);
        LOG_FILE << "Add dir " << e.name << "\n";
    }
    void add_file(File e) const {
        position->add_file(e);
        LOG_FILE << "Add file " << e.name << "\n";
    }
    void print() const {
        std::cout << "Pointer at " << position->name << std::endl;
        ls();
    }
};

auto test() {
    Directory n{"/"};
    n.add_dir(Directory{"a"});
    n.add_file(File{"b",10});
    std::cout << "Size of n " << n.get_size() << std::endl;

    Pointer p{n};
    p.print();
    p.cd("a");
    p.print();

    p.add_file(File{"d",100});

    p.print();

    
    std::cout << "Size of n " << n.get_size() << std::endl;
}

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

// return true if the cmd is ls
bool parse_cmd(Pointer& p, const std::string& cmd) {
    std::vector<std::string> cmd_vec = split(cmd);
    if(cmd_vec[1].compare("ls") == 0) {
        LOG_FILE << "CMD: ls" << std::endl;
        return true;
    }
    else if (cmd_vec[1].compare("cd") == 0) {
        LOG_FILE << "CMD: cd " << cmd_vec[2] << std::endl;
        p.cd(cmd_vec[2]);
        return false;
    }
    else 
        throw ("CMD ERROR");
}

void parse_ls(Pointer& p, const std::vector<std::string> &out_lines) {
    for(auto line : out_lines) {
        auto line_vec = split(line);
        if (line_vec[0].compare("dir") == 0) {
            p.add_dir(Directory{line_vec[1]});
        }
        else {
            p.add_file(File{line_vec[1],stoi(line_vec[0])});
        }
    }
}

auto parse_lines(Directory &root, std::ifstream &file) {
    Pointer p{root};
    bool get_cmd = true;
    std::string line;

    while(file.eof() == false) {
        std::vector<std::string> out_lines;

        if(get_cmd) {
            std::getline(file, line);
        }
        if( parse_cmd(p, line)) {
            // we are in "ls" command
            while (std::getline(file, line) && line[0] != '$') {
                out_lines.push_back(line);
            }
            parse_ls(p, out_lines);
            // last get line returned the next cmd, no need to get it again
            get_cmd = false;
        }
        else {
            get_cmd = true;
        }
    }
}

void launch_CLI(Directory& root){
    // CLI
    std::string line;
    Pointer p{root};
    while(true) {
        std::getline(std::cin, line);
        std::vector<std::string> cmd = split(line);
        if(cmd[0].compare("ls") == 0) {
            p.print();
        }
        else if (cmd[0].compare("cd") == 0 && cmd.size() > 1) {
            p.cd(cmd[1]);
        }
        else {
            std::cout << "unknown command" << std::endl;
        }
    }
}

auto main(int argc, char** argv) -> int {
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        Directory root{"/"};
        parse_lines(root, file);

        // launch_CLI(root);
        std::cout << root.get_sub100000_dirs() << std::endl;
        
        int remaining_size = 70000000 - root.get_size();
        int size_to_free = 30000000 - remaining_size;
        LOG_FILE << "size to free " << size_to_free << " " << remaining_size  <<std::endl;
        std::vector<size_t> big_enough;
        root.get_free_dirs(size_to_free, big_enough);
        std::sort(big_enough.begin(), big_enough.end());
        std::cout << "Smaller to remove " << big_enough[0] << std::endl;
    }
}
