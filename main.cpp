#include "stack.h"


int main()
{
    Stack stk1 = {};
    StackCtor(&stk1, 1);
    StackPush(&stk1, 3);
    StackPush(&stk1, 10);
    StackPush(&stk1, 15); 

    StackDump(&stk1);

    StackPop(&stk1);

    StackDump(&stk1);

    StackPop(&stk1);
    StackPop(&stk1);
    //StackPop(&stk1); 
    StackDump(&stk1);

    StackDtor(&stk1);

    return 0;
}