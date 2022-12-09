#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum class Direction {
    UP, DOWN, RIGHT, LEFT
};

char display(Direction d) {
    switch(d) {
        case Direction::UP:
            return 'U';
        case Direction::DOWN:
            return 'D';
        case Direction::RIGHT:
            return 'R';
        case Direction::LEFT:
            return 'L';
    }
}

class Position {
protected:
    int c; // col
    int l; // line
public:
    Position(int c, int l): c{c}, l{l} {}
    int getc() const { return c; }
    int getl() const { return l; }
    void move(Direction d) {
        switch(d) {
            case Direction::UP:
                l++;
                break;
            case Direction::DOWN:
                l--;
                break;
            case Direction::RIGHT:
                c++;
                break;
            case Direction::LEFT:
                c--;
                break;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Position& h) {
    os << "{" << h.getc() << "," << h.getl() << "}";
    return os;
}

bool operator==(const Position& lhs, const Position& rhs) {
    if ((lhs.getc() == rhs.getc()) && (lhs.getl() == rhs.getl()))
        return true;
    return false;
}

class Head: public Position {
public:
        Head(): Position{0, 0} {}
};

class Tail: public Position {
    std::vector<Position> visited;
public:
    Tail(): Position{0, 0} {
        visited.push_back(Position{0, 0});
    }
    void close_to(Position &h) {
        int c_d = h.getc() - c;
        int l_d = h.getl() - l;
        if(c==1 && l==0)
            std::cout << c_d << " " << l_d << std::endl;
        if (std::abs(c_d) > 1) {
            c_d > 0 ? move(Direction::RIGHT) : move(Direction::LEFT);
            // check diagonal
            if (std::abs(l_d) > 0) {
                l_d > 0 ? move(Direction::UP) : move(Direction::DOWN);
            }
        }
        else if (std::abs(l_d) > 1) {
            l_d > 0 ? move(Direction::UP) : move(Direction::DOWN);
            // check diagonal
            if (std::abs(c_d) > 0) {
                c_d > 0 ? move(Direction::RIGHT) : move(Direction::LEFT);
            }
        }
        // check if not already visited
        if (std::find(visited.begin(), visited.end(), Position{c,l}) == visited.end() ) { 
            visited.push_back(Position{c, l});
        }
    }
    int number_visited() const { return visited.size(); }
};


auto main(int argc, char** argv) -> int {
    std::string filename {argv[1]};
    std::ifstream file {filename}; 
    std::string line;
    Head h;
    Tail rope[9];
    while(std::getline(file, line)) {
        Direction dir;
        int dist = std::stoi(line.substr(2,line.size()));
        switch(line[0]) {
            case 'R':
                dir = Direction::RIGHT;
                break;
            case 'D':
                dir = Direction::DOWN;
                break;
            case 'L':
                dir = Direction::LEFT;
                break;
            case 'U':
                dir = Direction::UP;
                break;
        }
//      std::cout << display(dir) << " " << dist << std::endl;
        while(dist !=0) {
            h.move(dir);
            rope[0].close_to(h);
            for (int i = 1; i < 9; i++ ) {
                rope[i].close_to(rope[i-1]);
            }
            dist--;
//          std::cout << "h: " << h ;
//          for (int i = 0; i < 9; i++ ) {
//              std::cout <<  " t[" <<i+1<< "]: " << rope[i];
//          }
//          std::cout << std::endl;
        }
    }
    std::cout << "P1 " << rope[0].number_visited() << std::endl;
    std::cout << "P2 " << rope[8].number_visited() << std::endl;
    return 0;
}
