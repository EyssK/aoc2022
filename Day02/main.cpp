#include <iostream>
#include <fstream>

enum Op_choice { 
    Op_Stone = 'A',
    Op_Paper = 'B',
    Op_Scissors = 'C',
};

enum Strat_choice { 
    Strat_Stone = 'X',
    Strat_Paper = 'Y',
    Strat_Scissors = 'Z',
};

enum Strat_choice2 { 
    Strat_Lose = 'X',
    Strat_Draw = 'Y',
    Strat_Win = 'Z',
};

auto parse_entry(std::ifstream &file) {
    std::string line;
    if(!std::getline(file, line)) {
	throw 42;
    }
    if (Op_Stone == line[0]) {
        if (Strat_Stone == line[2])
	    return 4;
        else if (Strat_Paper == line[2])
	    return 8;
        else if (Strat_Scissors == line[2])
	    return 3;
    }
    else if (Op_Paper == line[0]) {
        if (Strat_Stone == line[2])
	    return 1;
        else if (Strat_Paper == line[2])
	    return 5;
        else if (Strat_Scissors == line[2])
	    return 9;
    }
    else if (Op_Scissors == line[0]) {
        if (Strat_Stone == line[2])
	    return 7;
        else if (Strat_Paper == line[2])
	    return 2;
        else if (Strat_Scissors == line[2])
	    return 6;
    }
}

auto parse_entry2(std::ifstream &file) {
    std::string line;
    if(!std::getline(file, line)) {
	throw 42;
    }
    if (Op_Stone == line[0]) {
        if (Strat_Draw == line[2])
	    return 4;
        else if (Strat_Win == line[2])
	    return 8;
        else if (Strat_Lose == line[2])
	    return 3;
    }
    else if (Op_Paper == line[0]) {
        if (Strat_Lose == line[2])
	    return 1;
        else if (Strat_Draw == line[2])
	    return 5;
        else if (Strat_Win == line[2])
	    return 9;
    }
    else if (Op_Scissors == line[0]) {
        if (Strat_Win == line[2])
	    return 7;
        else if (Strat_Lose == line[2])
	    return 2;
        else if (Strat_Draw == line[2])
	    return 6;
    }
}

auto main() -> int {
    {
        std::ifstream myfile {"input"}; 
        int tot = 0;
    
        while (true) {
            try {
                tot += parse_entry(myfile);
            }
            catch (...) {
                std::cout << "Total = " << tot << std::endl;
                break;
            }
        }
    }

    {
        std::ifstream myfile {"input"}; 
        int tot = 0;
    
        while (true) {
            try {
                tot += parse_entry2(myfile);
            }
            catch (...) {
                std::cout << "Total = " << tot << std::endl;
                break;
            }
        }
    }
}
