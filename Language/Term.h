//
// Created by Computer on 1/29/2023.
//

#ifndef PROJEKATKVADRAT_TERM_H
#define PROJEKATKVADRAT_TERM_H

#include <vector>
#include <string>

typedef unsigned char byte;

const size_t NUM_ERROR = (size_t)1<<32;
const byte BOOL_ERROR = 2;

const size_t MAX_NUM = (size_t)1<<32;

using std::vector;
using std::string;
using std::pair;

struct ProgramState {
    size_t memoryCounter = 0;
    size_t timeCounter = 0;
    size_t MAX_MEMORY;
    size_t MAX_TIME;

    ProgramState(size_t maxMemory, size_t maxTime);
};

struct Term {
    virtual string toString() const = 0;
};

struct Numerical : public Term {
    virtual size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const;
    virtual string toString() const override = 0;
};

struct Constant : public Numerical {
    size_t value;

    Constant(size_t value);

    size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct Variable : public Numerical {
    size_t index;

    Variable(size_t index);

    size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct Operation : public Numerical {
    char symbol;
    Numerical* lhs;
    Numerical* rhs;

    Operation(char symbol, Numerical *lhs, Numerical *rhs);

    size_t calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct Boolean : public Term {
    virtual byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const;
    virtual string toString() const override = 0;
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

struct SizeOfArray : public Numerical {
    size_t index;

    SizeOfArray(size_t index);

    size_t
    calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;

    string toString() const override;
};

#endif //PROJEKATKVADRAT_TERM_H
