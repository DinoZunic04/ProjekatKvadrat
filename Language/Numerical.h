//
// Created by Computer on 1/29/2023.
//

#ifndef PROJEKATKVADRAT_NUMERICAL_H
#define PROJEKATKVADRAT_NUMERICAL_H

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

typedef std::unordered_set<size_t> set;

set unite(const set &lhs, const set &rhs);

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


    const set &getOccuringVariables() const;

    const set &getOccuringArrays() const;

    virtual ~Numerical();

private:
    set occuringVariables;
    set occuringArrays;
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

#endif //PROJEKATKVADRAT_NUMERICAL_H
