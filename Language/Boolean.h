//
// Created by Computer on 2/7/2023.
//

#ifndef PROJEKATKVADRAT_BOOLEAN_H
#define PROJEKATKVADRAT_BOOLEAN_H

#include "Numerical.h"

struct Boolean {
    Boolean();

    virtual byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const;
    virtual string toString() const = 0;

    virtual ~Boolean();
};


struct Relation : public Boolean{
    char symbol;
    const Numerical* lhs;
    const Numerical* rhs;

    Relation(char symbol, const Numerical *lhs, const Numerical *rhs);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct NotStatement : public Boolean {
    const Boolean* statement;

    explicit NotStatement(const Boolean *statement);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct AndStatement : public Boolean {
    const Boolean* lhs;
    const Boolean* rhs;

    AndStatement(const Boolean *lhs, const Boolean *rhs);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

struct ArrayIndex : public Boolean {
    //index of the array itself, known in compile-time
    size_t metaIndex;
    //expression for the index of the element
    const Numerical* index;

    ArrayIndex(size_t metaIndex, const Numerical *index);

    byte calculate(ProgramState &ps, const vector<size_t> &variables, const vector<vector<byte> *> &memory) const override;
    string toString() const override;
};

#endif //PROJEKATKVADRAT_BOOLEAN_H
