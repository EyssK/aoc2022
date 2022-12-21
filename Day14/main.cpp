#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <thread>
#include <chrono>

// #define DISPLAY
using namespace std::chrono_literals;

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

class Point {
protected:
    int c; // col
    int l; // line
public:
    Point(int c, int l): c{c}, l{l} {}
    Point(std::string s){
        std::vector<std::string> coord = split(s, ',');
        c = std::stoi(coord[0]);
        l = std::stoi(coord[1]);
    }
    void move(int c, int l) {
        this->c = c;
        this->l = l;
    }
    void move(const Point& p) {
        this->c = p.getc();
        this->l = p.getl();
    }
    int getc() const { return c; }
    int getl() const { return l; }
    bool operator==(const Point &p) const {
        return ((p.getc() == c) && (p.getl() == l));
    }
    void print() const {
        std::cout << "[" << c << "," << l << "]" << std::endl;
    }
};

std::ostream& operator<< (std::ostream& os, const Point& p){
    os << "[" << p.getc() << "," << p.getl() << "]";
    return os;
}

std::tuple<int,int> minmax(int a, int b) {
    return a > b ? std::make_tuple(b,a) : std::make_tuple(a,b);
}

class Grid {
    std::vector<Point> rocks;
    std::vector<Point> sands;
    int floor;
public:
    Grid() : floor{-1} {}
    void addRock(Point p) {
        if (isFree(p))
            rocks.push_back(p);
    }
    void addSand(Point p) {
        if (isFree(p))
            sands.push_back(p);
    }
    void addLine(Point p1, Point p2) {
        if (p1.getc() == p2.getc()) {
            int c = p1.getc();
            auto [low,high] = minmax(p1.getl(),p2.getl());
            if (low!=high) {
                for (int i = low; i<=high; i++)
                    addRock(Point{c,i});
            }
            else
                addRock(p1);
            
        }
        else if (p1.getl() == p2.getl()) {
            int l = p1.getl();
            auto [low,high] = minmax(p1.getc(),p2.getc());
            if (low!=high) {
                for (int i = low; i<=high; i++)
                    addRock(Point{i,l});
            }
            else
                addRock(p1);
        }
        // else, points are not on a line
    }
    auto getSand(const Point &p) {
        return std::find(sands.begin(), sands.end(), p);
    }
    auto getRock(const Point &p) {
        return std::find(rocks.begin(), rocks.end(), p);
    }
    bool isFree(const Point &p) {
        return (p.getl()!=floor && getRock(p) == std::end(rocks) && getSand(p) == std::end(sands)); 
    }
    void print() const {
        std::cout << "grid ";
        for (auto p:rocks){
            std::cout << p;
        }
        for (auto p:sands){
            std::cout << p;
        }
        std::cout << std::endl;
    }
    void display(const Point &topleft, const Point &bottomright) {
        for (int l= topleft.getl(); l<=bottomright.getl(); l++){
            for (int c= topleft.getc(); c<=bottomright.getc(); c++){
                Point p{c,l};
                if (isFree(p)) {
                    std::cout << ".";
                }
                else if (getRock(p) != std::end(rocks)) {
                    std::cout << "#";
                }
                else {
                    std::cout << "+";
                }
            }
            std::cout << std::endl;
        }
    }
    void addBottomLine() {
        int lowest_point = 0;
        for (auto r : rocks) {
            lowest_point = r.getl() > lowest_point ? r.getl() : lowest_point;
        }
        floor = lowest_point+2;
    }
    int getFloor() const {
        return floor;
    }
};

Grid parse(char* path) {
    std::ifstream file {path};
    std::string line;
    Grid g;
    while(std::getline(file, line)) {
        std::regex rock_regex("[0-9]+,[0-9]+");
        auto pos_begin = 
            std::sregex_iterator(line.begin(), line.end(), rock_regex);
        auto pos_end = std::sregex_iterator();
    
        Point prevp{-1,-1};
        for (std::sregex_iterator i = pos_begin; i != pos_end; ++i) {
            std::smatch match = *i;
            Point p { match.str() };
            g.addLine(p,prevp);
            prevp = p;
        }
    }
    return g;
}

void part1(Grid &g) {
    // start simulation
    bool not_falling=true;
    int nb_sand = 0;
    while(not_falling) {
        Point s{500,0},prevs{500,0};
        g.addSand(s);
        while(not_falling) {
            Point under {s.getc(), s.getl()+1};
            Point underleft {s.getc()-1, s.getl()+1};
            Point underright {s.getc()+1, s.getl()+1};
            bool display = false;
            if (g.isFree(under)) {
                s.move(under);
                g.getSand(prevs)->move(under);
                display = true;
            }
            else if (g.isFree(underleft)) {
                s.move(underleft);
                g.getSand(prevs)->move(underleft);
                display = true;
            }
            else if (g.isFree(underright)) {
                s.move(underright);
                g.getSand(prevs)->move(underright);
                display = true;
            }
            else {
                // stuck
                nb_sand++;
                break;
            }
            if (s.getl() > 500) {
                // too low
                not_falling=false;
            }
            prevs = s;
#ifdef DISPLAY
            if (display) {
                std::cout << "\033[0;0H" << std::flush;
                // example
                //g.display(Point{494,0}, Point{503,9});
                // proper input
                g.display(Point{450,10}, Point{550,130});
                std::this_thread::sleep_for(1ms);
            }
#endif //DISPLAY
        }
        //std::cout << "Resting at " << s << std::endl;
    }
    // g.print();
    std::cout << "P1 " << nb_sand << std::endl;
#ifdef DISPLAY
    std::cout << "\033[0;0H" << std::flush;
    // proper input
    g.display(Point{450,10}, Point{550,130});
    // show cursor
    std::cout << "\033[?25h" << std::flush;
#endif //DISPLAY
}

void part2(Grid &g) {
    // find bottom line 
    g.addBottomLine();   
    // start simulation
    bool stuck=false;
    int nb_sand = 0;
    while(!stuck) {
        Point s{500,0},prevs{500,0};
        if (!g.isFree(s))
            stuck=true;
        g.addSand(s);
        while(!stuck) {
            Point under {s.getc(), s.getl()+1};
            Point underleft {s.getc()-1, s.getl()+1};
            Point underright {s.getc()+1, s.getl()+1};
            bool display = false;
            if (g.isFree(under)) {
                s.move(under);
                g.getSand(prevs)->move(under);
                display = true;
            }
            else if (g.isFree(underleft)) {
                s.move(underleft);
                g.getSand(prevs)->move(underleft);
                display = true;
            }
            else if (g.isFree(underright)) {
                s.move(underright);
                g.getSand(prevs)->move(underright);
                display = true;
            }
            else {
                // stuck
                nb_sand++;
                break;
            }
            prevs = s;
#ifdef DISPLAY
            if (display) {
                std::cout << "\033[0;0H" << std::flush;
                // example
                //g.display(Point{494,0}, Point{503,9});
                // proper input
                g.display(Point{450,10}, Point{550,130});
                std::this_thread::sleep_for(1ms);
            }
#endif //DISPLAY
        }
    }
    std::cout << "P2 " << nb_sand << std::endl;
#ifdef DISPLAY
    std::cout << "\033[0;0H" << std::flush;
    // proper input
    g.display(Point{450,10}, Point{550,130});
    // show cursor
    std::cout << "\033[?25h" << std::flush;
#endif //DISPLAY
}


auto main(int argc, char** argv) -> int {


#ifdef DISPLAY
    // clear screen
    std::cout << "\033[2J" << std::flush;
    // erase cursor
    std::cout << "\033[?25l" << std::flush;
#endif //DISPLAY

    // parse input
    Grid g = parse(argv[1]);
    part1(g);

    Grid g2 = parse(argv[1]);
    part2(g2);
}
