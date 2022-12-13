#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cmath>

using namespace std::chrono_literals;

#define COL_NB 174
#define ROW_NB 41

struct Point{
    int row, col;
    Point ( ) {}
    Point ( int r,int c) {
        if (r >= 0 && r < ROW_NB && c >= 0 && c < COL_NB) {
            row = r;
            col = c;
        }
        else {
            row = 20;
            col = 0;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    os << " {" << p.col << ","<< p.row << "} ";
    return os;
}

bool operator==(const Point& lh, const Point& rh) {
    if ((lh.row == rh.row) && (rh.col == lh.col))
        return true;
    return false;
}

class Map {
    char map[ROW_NB][COL_NB];
public:
    char& operator()(const Point &p) {
        return map[p.row][p.col];
    }
    void display() const {
        for(int r=0; r < ROW_NB; r++) {
            for(int c=0; c < COL_NB; c++) {
                std::cout << map[r][c];
            }
            std::cout << '\n';
        }
    }
};

class Dst {
    int dst[ROW_NB][COL_NB];
public:
    Dst()  {
        for(int r=0; r < ROW_NB; r++) {
            for(int c=0; c < COL_NB; c++) {
                dst[r][c] = INT_MAX;
            }
        }
    }
    int& operator()(const Point &p) {
        return dst[p.row][p.col];
    }
    void display() const {
        std::cout << "\x1b[42F";
        for(int r=0; r < ROW_NB; r++) {
            for(int c=0; c < COL_NB; c++) {
                if (dst[r][c] == INT_MAX)
                    std::cout << " ";
                else {
                    int r,g,b;
                    int s = dst[r][c];
                    // normalize between 0 and 1
                    float f = ((float)s)/490;
                    // cut in 5
                    float a = (1.0-f)/0.2;
                    int X = std::floor(a); // floor
                    int Y= std::floor((255*(a-X))) ; // remainder from 0 to 255
                    switch(X)
                    {
                        case 0: r=255;g=Y;b=0;break;
                        case 1: r=255-Y;g=255;b=0;break;
                        case 2: r=0;g=255;b=Y;break;
                        case 3: r=0;g=255-Y;b=255;break;
                        case 4: r=0;g=0;b=255;break;
                    }
                    std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m" << "▓";
                }
            }
            std::cout << '\n';
        }
    }
};

void prepare_display() {
        // clear all screen
        std::cout << "\x1b[2J";
        // remove cursor
        std::cout << "\x1b[?25l";
}

void restore_display() {
        // move down
        std::cout << "\x1b[" << ROW_NB <<";0" <<'H';
        // display cursor
        std::cout << "\x1b[?25h";
        // reset all attributes
        std::cout << "\x1b[0m";
}

void display_rainbow( const Point &p, int val, int scale){

    // move to the point
    std::cout << "\x1b[" << p.row <<";" << p.col <<'H';
    int r,g,b;
    int s = val;
    // normalize between 0 and 1
    float f = ((float)s)/scale;
    // cut in 5
    float a = (1.0-f)/0.2;
    int X = std::floor(a); // 1 to 5
    int Y= std::floor((255*(a-X))) ; // rational part from 0 to 255
    switch(X)
    {
        case 0: r=255;g=Y;b=0;break;
        case 1: r=255-Y;g=255;b=0;break;
        case 2: r=0;g=255;b=Y;break;
        case 3: r=0;g=255-Y;b=255;break;
        case 4: r=0;g=0;b=255;break;
    }
    std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m" << "▓"<< std::flush;
}

auto main(int argc, char** argv) -> int {
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 
        std::string line;

        Map map;
        Dst dst;
        Point S, E;

        // Parsing
        for(int r=0; r < ROW_NB; r++) {
            std::getline(file, line);
            for(int c=0; c < COL_NB; c++) {
                char l = line[c];
                if (l=='S') {
                    S = {r,c};
                    map(S) = 'a';
                    dst(S) = 0;
                }
                else if (l=='E') {
                    E = {r,c};
                    map(E) = 'z';
                }
                else {
                    map(Point{r,c}) = l;
                }
            }
        }
        prepare_display();

        // Breadth First Search BFS
        std::vector<Point> previous_points = {S};
        int step = 1;
        bool not_found = true;
        int step_found;
        while (not_found) {
            std::vector<Point> current_points;
            // continue from previous interesting points
            for (Point pp : previous_points) {
                const Point neighbours[4] = {
                    {pp.row-1,pp.col},
                    {pp.row+1,pp.col},
                    {pp.row,pp.col+1},
                    {pp.row,pp.col-1}};
                for (Point p : neighbours) {
                    // test if p not already examined
                    if (dst(p) > step) {
                        // test if p is accessible from previous interesting point
                        if ( map(p) <= (map(pp)+1)) {
                            if (p == E) {
                                not_found = false;
                                step_found = step;
                            }
                            else {
                                dst(p) = step;
                                display_rainbow(p,dst(p), 490); // 490 is my P1 answer
                                current_points.push_back(p);
                            }
                        }
                    }
                }
            }
            step++;
            previous_points.swap(current_points);
            std::this_thread::sleep_for(50ms);
        }

        restore_display();
        std::cout << "P1 " << step_found << std::endl;
    }
    return 0;
}
