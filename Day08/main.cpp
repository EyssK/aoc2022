#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#define W 99
#define H 99
//#define W 5
//#define H 5

void fill_forest(std::ifstream &file, char f[]) {
    std::string line;
    int h = 0;
    while(!file.eof()) {
        std::getline(file, line);
        int w = 0;
//        if (w == 0 && h == 0)
//            std::cout << "Line size = " << line.size() << std::endl; 
        for (char l : line) {
            f[w+h*W] = l;
            w++;
        }
        h++;
    }
}

void display_forest(const char f[]) {
    for(int h=0; h<H; h++){
        for(int w=0; w<W; w++){
            std::cout << f[w+h*W] << " ";
        }
        std::cout << std::endl;
    }
}

// return true if element is the bigger of col / row
bool biggest_row_side(const char f[], int h, int w, bool left) {
    int start = left ? 0: w+1;
    int end = left ? w: W;
    for(int idx=start; idx<end; idx++){
        if(f[w+h*W] <= f[idx+h*W])
            return false;
    }
    return true;
}

bool biggest_col_side(const char f[], int h, int w, bool up) {
    int start = up ? 0: h+1;
    int end = up ? h: H;
    for(int idx=start; idx<end; idx++){
        if(f[w+h*W] <= f[w+idx*W])
            return false;
    }
    return true;
}

int count_visible(const char f[]) {
    int visible = 2*W+2*H-4;;
    for(int h=1; h<(H-1); h++){
        for(int w=1; w<(W-1); w++){
            bool l = biggest_row_side(f, h, w, true); 
            bool r = biggest_row_side(f, h, w, false);
            bool u = biggest_col_side(f, h, w, true);
            bool d = biggest_col_side(f, h, w, false);
            if ( l || r || u || d) {
                visible++;
                // DEBUG
                //std::cout << w << " " << h << "\t" << l << r << d << u << std::endl;
            }
        }
    }
    return visible;
}

// number of visible tree from position h, w
int number_of_visible_left(const char f[], int h, int w) {
    for(int idx=w-1; idx>=0; idx--){
        if(f[w+h*W] <= f[idx+h*W])
            return w - idx;
    }
    return w;
}
int number_of_visible_right(const char f[], int h, int w) {
    for(int idx=w+1; idx<W; idx++){
        if(f[w+h*W] <= f[idx+h*W])
            return idx-w;
    }
    return W-w-1;
}
int number_of_visible_up(const char f[], int h, int w) {
    for(int idx=h-1; idx>=0; idx--){
        if(f[w+h*W] <= f[w+idx*W])
            return h - idx;
    }
    return h;
}
int number_of_visible_down(const char f[], int h, int w) {
    for(int idx=h+1; idx<H; idx++){
        if(f[w+h*W] <= f[w+idx*W])
            return idx-h;
    }
    return H-h-1;
}
int get_score(const char f[], int h, int w) {
    //std::cout << "score " << w << " " << h;
    int l = number_of_visible_left(f,h,w);
    int r = number_of_visible_right(f,h,w);
    int u = number_of_visible_up(f,h,w);
    int d = number_of_visible_down(f,h,w);
    //std::cout << "\t" << u << l << r << d << " tot " << l*r*u*d << std::endl;
    return l*r*u*d;
}

void find_best_score(const char f[]) {
    int best_score = 0;
    for(int h=0; h<H; h++){
        for(int w=0; w<W; w++){
            int score = get_score(f, h, w);
            best_score = score > best_score ? score : best_score;
        }
    }
    std::cout << "best scenic score : " << best_score << std::endl;
}

auto main(int argc, char** argv) -> int {
    {
        std::string filename {argv[1]};
        std::ifstream file {filename}; 

        char f[W*H];
        fill_forest(file, f);
        //display_forest(f);
        std::cout << "Part1 : " << count_visible(f) << std::endl;

        find_best_score(f);
    }
}
