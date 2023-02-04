//
// Created by Computer on 1/29/2023.
//

#include "Term.h"

ProgramState::ProgramState(size_t maxMemory, size_t maxTime) : MAX_MEMORY(maxMemory), MAX_TIME(maxTime) {}

size_t Numerical::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(ps.timeCounter > ps.MAX_TIME || ps.memoryCounter > ps.MAX_MEMORY)
        return NUM_ERROR;
    ps.MAX_TIME++;
    return 0;
}

Constant::Constant(size_t value) : value(value) {}

size_t Constant::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;

    return value;
}

string Constant::toString() const {
    return std::to_string(value);
}

Variable::Variable(size_t index) : index(index) {}

size_t Variable::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;

    if(index>=variables.size())
        return NUM_ERROR;
    return variables[index];
}

string Variable::toString() const {
    return "x_" + std::to_string(index);
}

Operation::Operation(char symbol, Numerical *lhs, Numerical *rhs) : symbol(symbol), lhs(lhs), rhs(rhs) {}

size_t Operation::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
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
            return lhsResult + rhsResult;
        case '-':
            return lhsResult - rhsResult;
        case '*':
            return lhsResult * rhsResult;
        case '/':
            return lhsResult / rhsResult;
    }
    return NUM_ERROR;
}

string Operation::toString() const {
    return '(' + lhs->toString() + ' ' + symbol + ' ' + rhs->toString() + ')';
}

byte Boolean::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(ps.timeCounter > ps.MAX_TIME || ps.memoryCounter > ps.MAX_MEMORY)
        return BOOL_ERROR;
    ps.MAX_TIME++;
    return false;
}

Relation::Relation(char symbol, Numerical *lhs, Numerical *rhs) : symbol(symbol), lhs(lhs), rhs(rhs) {}

byte Relation::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Boolean::calculate(ps,variables,memory) == BOOL_ERROR)
        return BOOL_ERROR;
    
    size_t lhsResult = lhs->calculate(ps, variables, memory);
    if(lhsResult == NUM_ERROR)
        return BOOL_ERROR;
    size_t rhsResult = rhs->calculate(ps, variables, memory);
    if(rhsResult == NUM_ERROR)
        return BOOL_ERROR;

    switch (symbol) {
        case '=':
            return lhsResult == rhsResult;
        case '<':
            return lhsResult < rhsResult;
    }
    return BOOL_ERROR;
}

string Relation::toString() const {
    return '(' + lhs->toString() + ' ' + symbol + ' ' + rhs->toString() + ')';
}

NotStatement::NotStatement(Boolean *statement) : statement(statement) {}

byte NotStatement::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Boolean::calculate(ps,variables,memory) == BOOL_ERROR)
        return BOOL_ERROR;

    byte result = statement->calculate(ps, variables, memory);
    if (result == BOOL_ERROR)
        return BOOL_ERROR;
    return !result;
}

string NotStatement::toString() const {
    return '!' + statement->toString();
}

AndStatement::AndStatement(Boolean *lhs, Boolean *rhs) : lhs(lhs), rhs(rhs) {}

byte AndStatement::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Boolean::calculate(ps,variables,memory) == BOOL_ERROR)
        return BOOL_ERROR;

    bool lhsResult = lhs->calculate(ps, variables, memory);
    if(lhsResult == BOOL_ERROR)
        return BOOL_ERROR;
    if(!lhsResult)
        return false;
    return rhs->calculate(ps, variables, memory);
}

string AndStatement::toString() const {
    return '(' + lhs->toString() + " and " + rhs->toString() + ')';
}

ArrayIndex::ArrayIndex(size_t metaIndex, Numerical *index) : metaIndex(metaIndex), index(index) {}

byte ArrayIndex::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Boolean::calculate(ps,variables,memory) == BOOL_ERROR)
        return BOOL_ERROR;

    if(metaIndex >= memory.size())
        return BOOL_ERROR;
    size_t indexResult = index->calculate(ps, variables, memory);
    if(indexResult >= memory[metaIndex].size())
        return BOOL_ERROR;
    return memory[metaIndex][indexResult];
}

string ArrayIndex::toString() const {
    return "A_" + std::to_string(metaIndex) + "[" + index->toString() + "]";
}

SizeOfArray::SizeOfArray(size_t index) : index(index) {}

size_t
SizeOfArray::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<bool>> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;
    if(index>=memory.size())
        return NUM_ERROR;
    return memory[index].size();
}

string SizeOfArray::toString() const {
    return "A_" + std::to_string(index) + ".size";
}
