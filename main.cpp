#include <iostream>
#include "Language/Instruction.h"

using std::cout;
using std::endl;

int main() {
    ProgramState ps(100,100);
    vector<size_t> variables = {0,1,2,3};
    vector<vector<bool>> memory = {{false,false},{false,true},{true,false},{true,true}};
    Constant zer(0), one(1);
    Variable x(0), y(1), z(2), t(3);
    Operation op1('+', &zer, &one);
    Operation op2('+', &t, &t);
    Operation op3('-', &x, &y);
    Operation op4('*', &x, &y);
    Operation op5('/', &zer, &one);
    cout << op1.toString() << endl;
    cout << op2.toString() << endl;
    cout << op3.toString() << endl;
    cout << op4.toString() << endl;
    cout << op5.toString() << endl;
    cout<<endl;
    cout << op1.calculate(ps, variables, memory) << endl;
    cout << op2.calculate(ps, variables, memory) << endl;
    cout << op3.calculate(ps, variables, memory) << endl;
    cout << op4.calculate(ps, variables, memory) << endl;
    cout << op5.calculate(ps, variables, memory) << endl;
    cout<<endl;
    return 0;
}
