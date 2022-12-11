#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

//#define TEST

auto main(int argc, char** argv) -> int {
    {
#ifdef TEST
#include "test.h"
#else
#include "main.h"
#endif
        for(int round_nb = 0; round_nb < 20; round_nb++) {
            for(int monkey_idx = 0; monkey_idx<MONKEY_NB; monkey_idx++) {
                for(auto item : items[monkey_idx]) {
                    item = operations[monkey_idx](item); 
                    item /= 3;
                    int monkey_to_send = test[monkey_idx](item);
                    items[monkey_to_send].push_back(item);
                    activity[monkey_idx]++;
                }
                items[monkey_idx].clear();
            }
            //            std::cout << "After round " << round_nb+1 << ":" << std::endl;
            //            for(int monkey_idx = 0; monkey_idx<MONKEY_NB; monkey_idx++) {
            //                std::cout << "\tMonkey " << monkey_idx << ": ";
            //                for(auto item : items[monkey_idx]) {
            //                    std::cout << item << " ";
            //                }
            //                std::cout << "\n";
            //            }
        }
        std::sort(activity, activity+MONKEY_NB, std::greater<int>());
        std::cout << "P1: " << activity[0]*activity[1] << std::endl;
    }
    {
#ifdef TEST
#include "test.h"
#else
#include "main.h"
#endif
        for(int round_nb = 0; round_nb < 10000; round_nb++) {
            for(int monkey_idx = 0; monkey_idx<MONKEY_NB; monkey_idx++) {
                for(auto item : items[monkey_idx]) {
                    item = operations[monkey_idx](item); 
                    int monkey_to_send = test[monkey_idx](item);
                    items[monkey_to_send].push_back(item);
                    activity[monkey_idx]++;
                }
                items[monkey_idx].clear();
            }
//            if (round_nb==0 || round_nb == 19 || (round_nb+1) % 1000 == 0){
//                std::cout << "After round " << round_nb+1 << ":" << std::endl;
//                for(int monkey_idx = 0; monkey_idx<MONKEY_NB; monkey_idx++)
//                    std::cout << "\tMonkey " << monkey_idx << " : " << activity[monkey_idx] << std::endl;
//            }
        }
        std::sort(activity, activity+MONKEY_NB, std::greater<int>());
        std::cout << "P2: " << (unsigned long long)(activity[0])*activity[1] << std::endl;
    }
}

