//
// Created by Computer on 2/6/2023.
//

#include "Universe.h"

Universe::Universe(size_t maxCost) : maxCost(maxCost) {
    numericals.resize(maxCost);
    booleans.resize(maxCost);
    instructions.resize(maxCost);
}

Universe::~Universe() {
    for (auto &nums: numericals)
        for (auto ptr: nums)
            delete ptr;
    for (auto &bools: booleans)
        for (auto ptr: bools)
            delete ptr;
    for (auto &ins : instructions)
        for (auto ptr: ins)
            delete ptr;
}

void Universe::insertNumetical(const Numerical *numerical) {
}

void Universe::insertBoolean(const Boolean *boolean) {
}

void Universe::insertInstruction(const Instruction *instruction) {

}
#include <iostream>
void Universe::constructNumericals() {
    //constants
    numericals[1].push_back(new Constant(1));
    numericals[1].push_back(new Constant(2));

    //variables
    for(size_t index = 0;index + 1<maxCost;index++)
        numericals[index + 1].push_back(new Variable(index));

    //size of array
    for(size_t index = 0;index + 1<maxCost;index++)
        numericals[index + 1].push_back(new SizeOfArray(index));

    //operation
    for(size_t c1 = 1; 1 +c1 < maxCost;c1++){
        for(auto n1 : numericals[c1]){
            auto o1 = dynamic_cast<const Operation*>(n1);
            for(size_t c2 = 1; 1 + c1 + c2 < maxCost;c2++){
                for(auto n2 : numericals[c2]){
                    auto o2 = dynamic_cast<const Operation*>(n2);
                    if(o1 == nullptr || o1->symbol != '-')
                        numericals[1 + c1 + c2].push_back(new Operation('+', n1, n2));
                    if(o2 == nullptr || o2->symbol != '+')
                        numericals[1 + c1 + c2].push_back(new Operation('-', n1, n2));
                    if(o1 == nullptr || o1->symbol != '/')
                        numericals[1 + c1 + c2].push_back(new Operation('*', n1, n2));
                    if(o2 == nullptr || o2->symbol != '*')
                        numericals[1 + c1 + c2].push_back(new Operation('/', n1, n2));
                }
            }
        }
    }

    numericals[1].push_back(new Constant(0));

    //for (size_t i = 1;i<maxCost;i++)
    //    std::cout<<"COST "<<i<<": "<<numericals[i].size()<<std::endl;

    for (size_t i = 1;i<maxCost;i++) {
        for (auto ptr: numericals[i]) {
            std::cout << ptr->toString() << " ( ";
            auto vars = ptr->getOccuringVariables();
            for(auto f : vars)
                std::cout<<f<<" ";
            std::cout<<")\n";
        }
    }
}

void Universe::constructInstructions() {

}

