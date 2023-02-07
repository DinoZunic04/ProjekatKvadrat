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
    for(size_t cost1 = 1; 2 + cost1 < maxCost; cost1++){
        for(auto n1 : numericals[cost1]){
            for(size_t cost2 = 1; 1 + cost1 + cost2 < maxCost; cost2++){
                size_t cost = 1 + cost1 + cost2;
                for(auto n2 : numericals[cost2]){
                    auto o1 = dynamic_cast<const Operation*>(n1);
                    auto o2 = dynamic_cast<const Operation*>(n2);

                    /*
                    if(dynamic_cast<const Constant*>(n1) && dynamic_cast<const Constant*>(n2)){
                        size_t result;
                        ProgramState ps(100, 10);
                        vector<size_t> variables;
                        vector<vector<byte>*> memory;

                        result = Operation('+', n1, n2).calculate(ps,variables,memory);
                        if(result != NUM_ERROR)
                            numericals[cost].push_back(new Constant(result));

                        result = Operation('-', n1, n2).calculate(ps,variables,memory);
                        if(result != NUM_ERROR)
                            numericals[cost].push_back(new Constant(result));

                        result = Operation('*', n1, n2).calculate(ps,variables,memory);
                        if(result != NUM_ERROR)
                            numericals[cost].push_back(new Constant(result));

                        result = Operation('/', n1, n2).calculate(ps,variables,memory);
                        if(result != NUM_ERROR)
                            numericals[cost].push_back(new Constant(result));

                        continue;
                    }
                     */

                    if(cost2>=cost1 && (o1 == nullptr || o1->symbol != '-'))
                        numericals[cost].push_back(new Operation('+', n1, n2));
                    if(o2 == nullptr || o2->symbol != '+')
                        numericals[cost].push_back(new Operation('-', n1, n2));
                    if(cost2>=cost1 && (o1 == nullptr || o1->symbol != '/'))
                        numericals[cost].push_back(new Operation('*', n1, n2));
                    if(o2 == nullptr || o2->symbol != '*')
                        numericals[cost].push_back(new Operation('/', n1, n2));
                }
            }
        }
    }

    numericals[1].push_back(new Constant(0));

    for (size_t i = 1;i<maxCost;i++)
        std::cout<<"COST "<<i<<": "<<numericals[i].size()<<std::endl;
/*
    for (size_t i = 1;i<maxCost;i++) {
        for (auto ptr: numericals[i]) {
            std::cout << ptr->toString() << " ( ";
            auto vars = ptr->getOccuringVariables();
            for(auto f : vars)
                std::cout<<f<<" ";
            std::cout<<")\n";
        }
    }
    */
}

//todo: dodati additive and multiplicative, ukloniti prosledjivanje promenljivih

void Universe::constructBooleans() {
    //array index
    for(size_t index = 0;index + 2 < maxCost;index++){
        for(size_t numCost = 1;index + 1 + numCost<maxCost;numCost++){
            for(auto n1 : numericals[numCost]){
                booleans[index + 1 + numCost].push_back(new ArrayIndex(index, n1));
            }
        }
    }
    //relation
    for(size_t cost1 = 1; 2 + cost1 < maxCost; cost1++){
        for(auto n1 : numericals[cost1]){
            for(size_t cost2 = 1; 1 + cost1 + cost2 < maxCost; cost2++){
                size_t cost = 1 + cost1 + cost2;
                for(auto n2 : numericals[cost2]){
                    booleans[cost].push_back(new Relation('=', n1, n2));
                    booleans[cost].push_back(new Relation('<', n1, n2));
                }
            }
        }
    }
    //and statement and not statement
    for(size_t cost1 = 1; 1 + cost1 < maxCost; cost1++){
        for(auto b1 : booleans[cost1]){
            //not statement
            booleans[1 + cost1].push_back(new NotStatement(b1));

            //and is commutative, WLOG cost2 >= cost1
            for(size_t cost2 = cost1; cost1 + cost2 < maxCost; cost2++){
                size_t cost = cost1 + cost2;
                for(auto b2 : booleans[cost2]){
                    booleans[cost].push_back(new AndStatement(b1, b2));
                }
            }
        }
    }
}

void Universe::constructInstructions() {

}

