//
// Created by Computer on 1/29/2023.
//

#ifndef PROJEKATKVADRAT_TERM_H
#define PROJEKATKVADRAT_TERM_H

#include <vector>
#include <string>
#include <unordered_set>

typedef unsigned char byte;

const size_t NUM_ERROR = (size_t)1<<32;
const byte BOOL_ERROR = 2;

const size_t MAX_NUM = (size_t)1<<32;

using std::vector;
using std::string;
using std::unordered_set;

template<typename T>
unordered_set<T> unorderedUnion(const unordered_set<T> &lhs, const unordered_set<T> &rhs) {
    auto toReturn = lhs;
    for(const auto& element : rhs)
        toReturn.insert(element);
    return toReturn;
}

//todo remove outNumerical
struct ProgramState {
    size_t memoryCounter = 0;
    size_t timeCounter = 0;
    size_t MAX_MEMORY;
    size_t MAX_TIME;
    vector<size_t> outNumerical{};
    vector<byte> outBoolean{};

    ProgramState(size_t maxMemory, size_t maxTime);
};

struct Numerical {
    Numerical(const unordered_set<size_t> &occuringVariables, const unordered_set<size_t> &occuringArrays);

    virtual size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const;
    virtual string toString() const = 0;


    const unordered_set<size_t> &getOccuringVariables() const;

    const unordered_set<size_t> &getOccuringArrays() const;

    virtual ~Numerical();

private:
    unordered_set<size_t> occuringVariables;
    unordered_set<size_t> occuringArrays;
};

struct Constant : public Numerical {
    size_t value;

    explicit Constant(size_t value);

    size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct Variable : public Numerical {
    size_t index;

    explicit Variable(size_t index);

    size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct Operation : public Numerical {
    char symbol;
    const Numerical* lhs;
    const Numerical* rhs;

    Operation(char symbol, const Numerical *lhs, const Numerical *rhs);

    size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct SizeOfArray : public Numerical {
    size_t index;

    explicit SizeOfArray(size_t index);

    size_t
    calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;

    string toString() const override;
};

struct Boolean {
    Boolean();

    virtual byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const;
    virtual string toString() const = 0;

    virtual ~Boolean();
};


struct Relation : public Boolean{
    char symbol;
    Numerical* lhs;
    Numerical* rhs;

    Relation(char symbol, Numerical *lhs, Numerical *rhs);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct NotStatement : public Boolean {
    Boolean* statement;

    explicit NotStatement(Boolean *statement);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct AndStatement : public Boolean {
    Boolean* lhs;
    Boolean* rhs;

    AndStatement(Boolean *lhs, Boolean *rhs);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct ArrayIndex : public Boolean {
    //index of the array itself, known in compile-time
    size_t metaIndex;
    //expression for the index of the element
    Numerical* index;

    ArrayIndex(size_t metaIndex, Numerical *index);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

#endif //PROJEKATKVADRAT_TERM_H
