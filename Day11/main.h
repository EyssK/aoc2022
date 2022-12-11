
#define MONKEY_NB 8

// Hardcoded is ugly but much easier
std::vector<unsigned int> items[MONKEY_NB] = {
    {93, 54, 69, 66, 71},
    {89, 51, 80, 66},
    {90, 92, 63, 91, 96, 63, 64},
    {65, 77},
    {76, 68, 94},
    {86, 65, 66, 97, 73, 83},
    {78},
    {89, 57, 59, 61, 87, 55, 55, 88},
};

const unsigned int PPCM = 2*3*5*7*11*13*17*19;

std::function<unsigned int(unsigned int)> operations[MONKEY_NB] = {
    [](int i) { return (i*3)%PPCM; },
    [](int i) { return (i*17)%PPCM; },
    [](int i) { return (i+1)%PPCM; },
    [](int i) { return (i+2)%PPCM; },
    [](int i) { return ((unsigned long long)i*i)%PPCM; },
    [](int i) { return (i+8)%PPCM; },
    [](int i) { return (i+6)%PPCM; },
    [](int i) { return (i+7)%PPCM; },
};

std::function<int(unsigned int)> test[MONKEY_NB] = {
    [](int i) { return i%7==0 ? 7: 1; },
    [](int i) { return i%19==0 ? 5: 7; },
    [](int i) { return i%13==0 ? 4: 3; },
    [](int i) { return i%3==0 ? 4: 6; },
    [](int i) { return i%2==0 ? 0: 6; },
    [](int i) { return i%11==0 ? 2: 3; },
    [](int i) { return i%17==0 ? 0: 1; },
    [](int i) { return i%5==0 ? 2: 5; },
};

unsigned int activity[MONKEY_NB] = {0,0,0,0,0,0,0,0};
