//
// Created by Computer on 2/1/2023.
//

#include "Instruction.h"

string offset(size_t indentation){
    string toReturn;
    for(size_t i = 0;i<indentation;i++)
        toReturn+="    ";
    return toReturn;
}

Termination Instruction::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    ps.timeCounter++;
    if(ps.timeCounter > ps.MAX_TIME || ps.memoryCounter > ps.MAX_MEMORY)
        return EXIT;
    return DEFAULT;
}

Instruction::~Instruction() = default;

DeclareVariable::DeclareVariable(const Numerical *value) : value(value) {}

string DeclareVariable::toString(size_t indentation) const {
    return offset(indentation) + "decl " + value->toString();
}

Termination DeclareVariable::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    ps.memoryCounter+=4;
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    size_t valueResult = value->calculate(ps, variables, memory);
    if(valueResult == NUM_ERROR)
        return EXIT;

    variables.push_back(valueResult);
    return DEFAULT;
}

NumericAssignment::NumericAssignment(size_t index, const Numerical *value) : index(index), value(value) {}

string NumericAssignment::toString(size_t indentation) const {
    return offset(indentation) + "x" + std::to_string(index) + " = " + value->toString();
}

Termination NumericAssignment::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    if(index >= variables.size())
        return EXIT;
    size_t valueResult = value->calculate(ps, variables, memory);
    if(valueResult == NUM_ERROR)
        return EXIT;

    variables[index] = valueResult;
    return DEFAULT;
}


BooleanAssignment::BooleanAssignment(size_t metaIndex, const Numerical *index, const Boolean *value) : metaIndex(metaIndex),
                                                                                           index(index), value(value) {}

string BooleanAssignment::toString(size_t indentation) const {
    return offset(indentation) + "A" + std::to_string(metaIndex) + "[" + index->toString() + "] = " + value->toString();
}

Termination BooleanAssignment::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    if(metaIndex >= memory.size())
        return EXIT;
    size_t indexResult = index->calculate(ps, variables, memory);
    if(indexResult>=memory[metaIndex]->size())
        return EXIT;
    byte valueResult = value->calculate(ps, variables, memory);
    if(valueResult == BOOL_ERROR)
        return EXIT;

    (*memory[metaIndex])[indexResult] = valueResult;
    return DEFAULT;
}

DeclareArray::DeclareArray(const Numerical *size) : size(size) {}

string DeclareArray::toString(size_t indentation) const {
    return offset(indentation) + "declare["+size->toString()+"]";
}

Termination DeclareArray::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    size_t sizeResult = size->calculate(ps, variables, memory);
    if(sizeResult == NUM_ERROR)
        return EXIT;
    if(ps.memoryCounter + sizeResult + 8 > ps.MAX_MEMORY)
        return EXIT;
    ps.memoryCounter += sizeResult + 8;

    memory.push_back(new vector<byte>(sizeResult));
    return DEFAULT;
}

Block::Block(const vector<const Instruction *> &instructions, Termination termination) : instructions(instructions),
                                                                                         termination(termination) {}

string Block::toString(size_t indentation) const {
    string toReturn;
    for(auto instruction:instructions)
        toReturn += instruction->toString(indentation) + '\n';
    switch (termination) {
        case EXIT:
            toReturn += offset(indentation) + "exit\n";
            break;
        case RETURN:
            toReturn += offset(indentation) + "return\n";
            break;
        case BREAK:
            toReturn += offset(indentation) + "break\n";
            break;
        case CONTINUE:
            toReturn += offset(indentation) + "continue\n";
            break;
    }
    return toReturn;
}

Termination Block::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;

    vector<size_t> oldSizes(variables.size());
    for(int i=0;i<variables.size();i++)
        oldSizes[i] = memory.size();

    for(auto instruction : instructions) {
        auto result = instruction->run(ps, variables, memory);
        if(result != DEFAULT) {
            clean(ps,memory, oldSizes);
            return result;
        }
    }
    clean(ps,memory,oldSizes);
    return termination;
}

void Block::clean(ProgramState &ps, vector<vector<byte> *> &memory, const vector<size_t> &oldSizes) {
    for(size_t i=0;i<oldSizes.size();i++){
        ps.memoryCounter-=memory[i]->size()-oldSizes[i];
        memory[i]->resize(oldSizes[i]);
    }
    for(size_t i=oldSizes.size();i<memory.size();i++){
        ps.memoryCounter-=memory[i]->size();
        delete memory[i];
    }
    memory.resize(oldSizes.size());
}


IfElse::IfElse(const Boolean *condition, const Block* ifBlock, const Block* elseBlock) : condition(condition),
                                                                                   ifBlock(ifBlock),
                                                                                   elseBlock(elseBlock) {}

string IfElse::toString(size_t indentation) const {
    return offset(indentation)+
            "if "+condition->toString()+
            ":\n"+ifBlock->toString(indentation+1)+
            "\nelse:\n"+
            elseBlock->toString(indentation+1);
}

Termination IfElse::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    byte conditionResult = condition->calculate(ps,variables,memory);
    if(conditionResult == BOOL_ERROR)
        return EXIT;
    if(conditionResult)
        return ifBlock->run(ps,variables,memory);
    return elseBlock->run(ps,variables,memory);
}

While::While(const Boolean *condition, const Block *block) : condition(condition), block(block) {}

string While::toString(size_t indentation) const {
    return offset(indentation) + "while "+condition->toString()+":\n"+block->toString(indentation+1);
}

Termination While::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    while(true){
        byte conditionResult = condition->calculate(ps,variables,memory);
        if(conditionResult == BOOL_ERROR)
            return EXIT;
        if(!conditionResult)
            break;
        auto result = block->run(ps,variables,memory);
        if(result == DEFAULT || result == CONTINUE)
            continue;
        if(result == BREAK)
            break;
        return result;
    }
    return DEFAULT;
}

Function::Function(const string &name, size_t numOfParameters, size_t numOfArrays, const Block *block) : name(name),
                                                                                                         numOfParameters(numOfParameters),
                                                                                                         numOfArrays(numOfArrays),
                                                                                                         block(block) {}

string Function::toString() const {
    string toReturn = name;
    toReturn += "(";
    if(numOfParameters)
        toReturn += "x" + std::to_string(0);
    for(size_t i = 1;i<numOfParameters;i++)
        toReturn += ", x" + std::to_string(i);
    if(numOfArrays){
        string A0 = "A" + std::to_string(0);
        if(numOfParameters)
            toReturn += ", ";
        toReturn += A0;
    }
    for(size_t i = 1;i<numOfArrays;i++)
        toReturn += ", A" + std::to_string(i);
    toReturn += "):\n";
    toReturn += block->toString(1);
    return toReturn;
}

FunctionApplication::FunctionApplication(const Function *function, const vector<size_t> &parameters,
                                         const vector<size_t> &arrayReferences) : function(function),
                                                                                  parameters(parameters),
                                                                                  arrayReferences(arrayReferences) {}

string FunctionApplication::toString(size_t indentation) const {
    string toReturn = function->name;
    toReturn += "(";
    if(function->numOfParameters)
        toReturn += "x" + std::to_string(parameters[0]);
    for(size_t i = 1;i<function->numOfParameters;i++)
        toReturn += ", x" + std::to_string(parameters[i]);
    if(function->numOfArrays){
        string A0 = "A" + std::to_string(arrayReferences[0]);
        if(function->numOfParameters)
            toReturn += ", ";
        toReturn += A0;
    }
    for(size_t i = 1;i<function->numOfArrays;i++)
        toReturn += ", A" + std::to_string(arrayReferences[i]);
    toReturn += ");\n";
    return toReturn;
}

Termination FunctionApplication::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    ps.memoryCounter += 4*variables.size() + 8*memory.size();
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;

    vector<size_t> newVariables(parameters.size());
    for(size_t i = 0;i<parameters.size();i++)
        newVariables[i] = variables[parameters[i]];

    vector<vector<byte> *> newArrays(arrayReferences.size());
    for(size_t i = 0;i<arrayReferences.size();i++)
        newArrays[i] = memory[arrayReferences[i]];

    return function->block->run(ps,newVariables, newArrays);
}

WriteNumerical::WriteNumerical(size_t index) : index(index) {}

string WriteNumerical::toString(size_t indentation) const {
    return offset(indentation) + "write x" + std::to_string(index);
}

Termination WriteNumerical::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    ps.memoryCounter+=4;
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    ps.outNumerical.push_back(variables[index]);
    return DEFAULT;
}

Write::Write(size_t index) : index(index) {}

string Write::toString(size_t indentation) const {
    return offset(indentation) + "write A" + std::to_string(index);
}

Termination Write::run(ProgramState &ps, vector<size_t> &variables, vector<vector<byte> *> &memory) const {
    ps.memoryCounter+=memory[index]->size();
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    for (auto b : *memory[index])
        ps.outBoolean.push_back(b);
    return DEFAULT;
}

Program::Program(const Block *main, const vector<Function> &functions) : main(main), functions(functions) {}

ProgramState Program::run(size_t MAX_MEMORY, size_t MAX_TIME) const {
    ProgramState ps(MAX_MEMORY, MAX_TIME);
    vector<size_t> variables;
    vector<vector<byte> *> memory;
    main->run(ps, variables, memory);
    return ps;
}

string Program::toString() const {
    string toReturn;
    toReturn += "def main:\n";
    toReturn += main->toString(1) + '\n';
    for(const auto& f : functions)
        toReturn += f.toString() + '\n';
    return toReturn;
}
