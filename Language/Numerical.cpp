//
// Created by Computer on 1/29/2023.
//

#include "Numerical.h"

set unite(const set &lhs, const set &rhs){
    set toReturn = lhs;
    for(size_t element : rhs)
        toReturn.insert(element);
    return toReturn;
}

ProgramState::ProgramState(size_t maxMemory, size_t maxTime) : MAX_MEMORY(maxMemory), MAX_TIME(maxTime) {}

size_t Numerical::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(ps.timeCounter > ps.MAX_TIME || ps.memoryCounter > ps.MAX_MEMORY)
        return NUM_ERROR;
    ps.MAX_TIME++;
    return 0;
}

Numerical::Numerical(const unordered_set<size_t> &occuringVariables, const unordered_set<size_t> &occuringArrays)
        : occuringVariables(occuringVariables), occuringArrays(occuringArrays) {}

const unordered_set<size_t> &Numerical::getOccuringVariables() const {
    return occuringVariables;
}

const unordered_set<size_t> &Numerical::getOccuringArrays() const {
    return occuringArrays;
}

Numerical::~Numerical() = default;

Constant::Constant(size_t value) : Numerical({},{}), value(value) {}

size_t Constant::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;

    return value;
}

string Constant::toString() const {
    return std::to_string(value);
}

Variable::Variable(size_t index) : Numerical({index}, {}), index(index) {}

size_t Variable::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;

    if(index>=variables.size())
        return NUM_ERROR;
    return variables[index];
}

string Variable::toString() const {
    return "x" + std::to_string(index);
}

Operation::Operation(char symbol, const Numerical *lhs, const Numerical *rhs) :
        Numerical(unite(lhs->getOccuringVariables(), rhs->getOccuringVariables()),
                  unite(lhs->getOccuringArrays(), rhs->getOccuringArrays())),
        symbol(symbol), lhs(lhs), rhs(rhs) {}

size_t Operation::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;

    size_t rhsResult = rhs->calculate(ps, variables, memory);
    if(rhsResult == NUM_ERROR)
        return NUM_ERROR;
    if(rhsResult == 0){
        if (symbol == '+' || symbol == '-')
            return lhs->calculate(ps, variables, memory);
        if(symbol == '*')
            return 0;
        return NUM_ERROR;
    }
    size_t lhsResult = lhs->calculate(ps, variables, memory);
    if(lhsResult == NUM_ERROR)
        return NUM_ERROR;
    switch (symbol) {
        case '+':
            return (lhsResult + rhsResult)%MAX_NUM;
        case '-':
            if(rhsResult > lhsResult)
                return NUM_ERROR;
            return (lhsResult - rhsResult)%MAX_NUM;
        case '*':
            return (lhsResult * rhsResult)%MAX_NUM;
        case '/':
            return lhsResult / rhsResult;
    }
    return NUM_ERROR;
}

string Operation::toString() const {
    return '(' + lhs->toString() + ' ' + symbol + ' ' + rhs->toString() + ')';
}
