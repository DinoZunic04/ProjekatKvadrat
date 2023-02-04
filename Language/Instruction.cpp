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

Result::Result(ResultType type, size_t value) : type(type), value(value) {}
Result::Result(size_t value) : type(RETURN), value(value) {}
Result::operator ResultType() const {
    return type;
}

Result Instruction::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(ps.timeCounter > ps.MAX_TIME || ps.memoryCounter > ps.MAX_MEMORY)
        return EXIT;
    ps.MAX_TIME++;
    return DEFAULT;
}

DeclareVariable::DeclareVariable(const Numerical *value) : value(value) {}

string DeclareVariable::toString(size_t indentation) const {
    return offset(indentation) + "decl " + value->toString();
}

Result DeclareVariable::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    ps.memoryCounter++;
    size_t valueResult = value->calculate(ps, variables, memory);
    if(valueResult == NUM_ERROR)
        return EXIT;

    variables.push_back(valueResult);
    return DEFAULT;
}

NumericAssignment::NumericAssignment(size_t index, const Numerical *value) : index(index), value(value) {}

string NumericAssignment::toString(size_t indentation) const {
    return offset(indentation) + "x_" + std::to_string(index) + " = " + value->toString();
}

Result NumericAssignment::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
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
    return offset(indentation) + "A_" + std::to_string(metaIndex) + "[" + index->toString() + "] = " + value->toString();
}

Result BooleanAssignment::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    if(metaIndex >= memory.size())
        return EXIT;
    size_t indexResult = index->calculate(ps, variables, memory);
    if(indexResult>=memory[metaIndex].size())
        return EXIT;
    byte valueResult = value->calculate(ps, variables, memory);
    if(valueResult == BOOL_ERROR)
        return EXIT;

    memory[metaIndex][indexResult] = valueResult;
    return DEFAULT;
}

DeclareArray::DeclareArray(const Numerical *size) : size(size) {}

string DeclareArray::toString(size_t indentation) const {
    return offset(indentation) + "declare["+size->toString()+"]";
}

Result DeclareArray::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    size_t sizeResult = size->calculate(ps, variables, memory);
    if(sizeResult == NUM_ERROR)
        return EXIT;
    if(ps.memoryCounter + sizeResult > ps.MAX_MEMORY)
        return EXIT;

    memory.emplace_back(sizeResult);
    ps.memoryCounter+=sizeResult;
    return DEFAULT;
}

string ContinueCommand::toString(size_t indentation) const {
    return offset(indentation) + "continue";
}

Result ContinueCommand::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    return CONTINUE;
}

string BreakCommand::toString(size_t indentation) const {
    return offset(indentation) + "break";
}

Result BreakCommand::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    return BREAK;
}

ReturnCommand::ReturnCommand(const Numerical *value) : value(value) {}

string ReturnCommand::toString(size_t indentation) const {
    return offset(indentation) + "return " + value->toString();
}

Result ReturnCommand::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;
    size_t valueResult = value->calculate(ps, variables, memory);
    if(valueResult == NUM_ERROR)
        return EXIT;
    return {RETURN, valueResult};
}

Block::Block(const vector<const Instruction*> &instructions) : instructions(instructions) {}

string Block::toString(size_t indentation) const {
    string toReturn;
    for(auto instruction:instructions)
        toReturn += instruction->toString(indentation) + '\n';
    return toReturn;
}

Result Block::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
    if(Instruction::run(ps, variables, memory) == EXIT)
        return EXIT;

    //garbage collection
    size_t oldVariablesSize = variables.size();
    vector<size_t> oldMemorySizes(variables.size());
    for(int i=0;i<variables.size();i++)
        oldMemorySizes[i] = memory.size();

    for(auto instruction : instructions) {
        auto result = instruction->run(ps, variables, memory);
        if(result != DEFAULT) {
            clean(ps,variables,memory,oldVariablesSize,oldMemorySizes);
            return result;
        }
    }
    clean(ps,variables,memory,oldVariablesSize,oldMemorySizes);
    return DEFAULT;
}

void Block::clean(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory, size_t oldVariablesSize,
                  const vector<size_t> &oldMemorySizes) {
    ps.memoryCounter -= variables.size() - oldVariablesSize;
    variables.resize(oldVariablesSize);

    for(int i=0;i<oldMemorySizes.size();i++){
        ps.memoryCounter-=memory[i].size()-oldMemorySizes[i];
        memory[i].resize(oldMemorySizes[i]);
    }
    for(int i=oldMemorySizes.size();i<memory.size();i++)
        ps.memoryCounter-=memory[i].size();
    memory.resize(oldMemorySizes.size());
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

Result IfElse::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
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

Result While::run(ProgramState &ps, vector<size_t> &variables, vector<vector<bool>> &memory) const {
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
