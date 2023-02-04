//
// Created by Computer on 2/1/2023.
//

#ifndef PROJEKATKVADRAT_INSTRUCTION_H
#define PROJEKATKVADRAT_INSTRUCTION_H

#include "Term.h"

enum ResultType {
    EXIT,
    RETURN,
    BREAK,
    CONTINUE,
    DEFAULT
};

struct Result {
    ResultType type;
    size_t value;

    Result(ResultType type, size_t value = 0);
    Result(size_t value);
    operator ResultType() const;
};

string offset(size_t indentation);

struct Instruction {
    virtual string toString(size_t indentation) const = 0;
    virtual Result run(ProgramState& ps, vector<size_t>& variables, vector<vector<bool>>& memory) const;
};

struct DeclareVariable : public Instruction {
    const Numerical* value;

    DeclareVariable(const Numerical *value);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct NumericAssignment : public Instruction {
    size_t index;
    const Numerical* value;

    NumericAssignment(size_t index, const Numerical *value);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct DeclareArray : public Instruction {
    const Numerical* size;

    DeclareArray(const Numerical *size);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct BooleanAssignment : public Instruction {
    size_t metaIndex;
    const Numerical* index;
    const Boolean* value;

    BooleanAssignment(size_t metaIndex, const Numerical *index, const Boolean *value);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct ContinueCommand : public Instruction {
    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct BreakCommand : public Instruction {
    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct ReturnCommand : public Instruction {
    const Numerical* value;

    ReturnCommand(const Numerical *value);

    string toString(size_t indentation) const override;

    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct Block : public Instruction {
    vector<const Instruction*> instructions;

    Block(const vector<const Instruction*> &instructions);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
private:
    static void clean(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory, size_t oldVariablesSize, const vector<size_t>& oldMemorySizes);
};

struct IfElse : public Instruction {
    const Boolean* condition;
    const Block* ifBlock;
    const Block* elseBlock;

    IfElse(const Boolean *condition, const Block* ifBlock, const Block* elseBlock);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

struct While : public Instruction {
    const Boolean* condition;
    const Block* block;

    While(const Boolean *condition, const Block *block);

    string toString(size_t indentation) const override;
    Result run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const override;
};

#endif //PROJEKATKVADRAT_INSTRUCTION_H
