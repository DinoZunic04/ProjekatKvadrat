//
// Created by Computer on 2/6/2023.
//

#ifndef PROJEKATKVADRAT_UNIVERSE_H
#define PROJEKATKVADRAT_UNIVERSE_H

#include "Language/Instruction.h"

class Universe {
public:
    size_t maxCost;

    explicit Universe(size_t maxCost);

    vector<vector<const Numerical*>> numericals;
    vector<vector<const Boolean*>> booleans;
    vector<vector<const Instruction*>> instructions;

    void insertNumetical(const Numerical *numerical);
    void insertBoolean(const Boolean *boolean);
    void insertInstruction(const Instruction *instruction);

    void constructNumericals();
    void constructInstructions();

    virtual ~Universe();
};


#endif //PROJEKATKVADRAT_UNIVERSE_H
