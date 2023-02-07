//
// Created by Computer on 2/1/2023.
//

#ifndef PROJEKATKVADRAT_INSTRUCTION_H
#define PROJEKATKVADRAT_INSTRUCTION_H

#include "Term.h"

enum Termination {
    EXIT,
    RETURN,
    BREAK,
    CONTINUE,
    DEFAULT
};

string offset(size_t indentation);

struct Instruction {
    virtual string toString(size_t indentation) const = 0;
    virtual Termination run(ProgramState& ps, vector<size_t>& variables, vector<vector<byte> *> &memory) const;

    virtual ~Instruction();
};

struct DeclareVariable : public Instruction {
    const Numerical* value;

    DeclareVariable(const Numerical *value);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct NumericAssignment : public Instruction {
    size_t index;
    const Numerical* value;

    NumericAssignment(size_t index, const Numerical *value);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct DeclareArray : public Instruction {
    const Numerical* size;

    DeclareArray(const Numerical *size);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct BooleanAssignment : public Instruction {
    size_t metaIndex;
    const Numerical* index;
    const Boolean* value;

    BooleanAssignment(size_t metaIndex, const Numerical *index, const Boolean *value);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct Block : public Instruction {
    vector<const Instruction*> instructions;
    Termination termination;

    Block(const vector<const Instruction *> &instructions, Termination termination);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
private:
    static void clean(ProgramState &ps, vector<vector<byte> *> &memory, const vector<size_t>& oldSizes);
};

struct IfElse : public Instruction {
    const Boolean* condition;
    const Block* ifBlock;
    const Block* elseBlock;

    IfElse(const Boolean *condition, const Block* ifBlock, const Block* elseBlock);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct While : public Instruction {
    const Boolean* condition;
    const Block* block;

    While(const Boolean *condition, const Block *block);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct Function {
    string name;
    size_t numOfParameters;
    size_t numOfArrays;
    const Block* block;

    Function(const string &name, size_t numOfParameters, size_t numOfArrays, const Block *block);

    string toString() const;
};

struct FunctionApplication : public Instruction {
    const Function* function;
    vector<size_t> parameters;
    vector<size_t> arrayReferences;

    FunctionApplication(const Function *function, const vector<size_t> &parameters,
                        const vector<size_t> &arrayReferences);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct WriteNumerical : public Instruction {
    size_t index;

    WriteNumerical(size_t index);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct Write : public Instruction {
    size_t index;

    Write(size_t index);

    string toString(size_t indentation) const override;
    Termination run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const override;
};

struct Program {
    const Block* main;
    vector<Function> functions;

    Program(const Block *main, const vector<Function> &functions);
    ProgramState run(size_t MAX_MEMORY, size_t MAX_TIME) const;

    string toString() const;
};

//todo add modifies and uses

#endif //PROJEKATKVADRAT_INSTRUCTION_H
