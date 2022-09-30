#include "stack.h"


int main()
{
    Stack stk1 = {};
    StackCtor(&stk1, 1.0);
    StackPush(&stk1, 3.0);
    StackPush(&stk1, 10.0);
    StackPush(&stk1, 15.0); 

    StackDump(&stk1);

    StackPop(&stk1);

    StackDump(&stk1);

    StackDtor(&stk1);

    return 0;
}