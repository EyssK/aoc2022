
#define MONKEY_NB 4

// Hardcoded is ugly but much easier
std::vector<unsigned int> items[MONKEY_NB] = {
    {79, 98},
    {54, 65, 75, 74},
    {79, 60, 97},
    {74}
};

const unsigned int PPCM = 23*19*13*17;

std::function<unsigned int(unsigned int)> operations[MONKEY_NB] = {
    [](int i) { return (i*19)%PPCM; },
    [](int i) { return (i+6)%PPCM; },
    [](int i) { return ((unsigned long long)i*i)%PPCM; },
    [](int i) { return (i+3)%PPCM; },
};

std::function<int(unsigned int)> test[MONKEY_NB] = {
    [](int i) { return i%23==0 ? 2: 3; },
    [](int i) { return i%19==0 ? 2: 0; },
    [](int i) { return i%13==0 ? 1: 3; },
    [](int i) { return i%17==0 ? 0: 1; },
};

unsigned int activity[MONKEY_NB] = {0,0,0,0};
