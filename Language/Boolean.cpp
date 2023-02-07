//
// Created by Computer on 2/7/2023.
//

#include "Boolean.h"

byte Boolean::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(ps.timeCounter > ps.MAX_TIME || ps.memoryCounter > ps.MAX_MEMORY)
        return BOOL_ERROR;
    ps.MAX_TIME++;
    return false;
}

SizeOfArray::SizeOfArray(size_t index) : Numerical({},{index}), index(index) {}

size_t SizeOfArray::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(Numerical::calculate(ps,variables,memory) == NUM_ERROR)
        return NUM_ERROR;
    if(index>=memory.size())
        return NUM_ERROR;
    return memory[index]->size();
}

string SizeOfArray::toString() const {
    return "A" + std::to_string(index) + ".size";
}

Boolean::Boolean() = default;
Boolean::~Boolean() = default;

Relation::Relation(char symbol, const Numerical *lhs, const Numerical *rhs) : symbol(symbol), lhs(lhs), rhs(rhs) {}

byte Relation::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
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

NotStatement::NotStatement(const Boolean *statement) : statement(statement) {}

byte NotStatement::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
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

AndStatement::AndStatement(const Boolean *lhs, const Boolean *rhs) : lhs(lhs), rhs(rhs) {}

byte AndStatement::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(Boolean::calculate(ps,variables,memory) == BOOL_ERROR)
        return BOOL_ERROR;

    byte lhsResult = lhs->calculate(ps, variables, memory);
    if(lhsResult == BOOL_ERROR)
        return BOOL_ERROR;
    if(!lhsResult)
        return false;
    return rhs->calculate(ps, variables, memory);
}

string AndStatement::toString() const {
    return '(' + lhs->toString() + " and " + rhs->toString() + ')';
}

ArrayIndex::ArrayIndex(size_t metaIndex, const Numerical *index) : metaIndex(metaIndex), index(index) {}

byte ArrayIndex::calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const {
    if(Boolean::calculate(ps,variables,memory) == BOOL_ERROR)
        return BOOL_ERROR;

    if(metaIndex >= memory.size())
        return BOOL_ERROR;
    size_t indexResult = index->calculate(ps, variables, memory);
    if(indexResult >= memory[metaIndex]->size())
        return BOOL_ERROR;
    return (*memory[metaIndex])[indexResult];
}

string ArrayIndex::toString() const {
    return "A" + std::to_string(metaIndex) + "[" + index->toString() + "]";
}
