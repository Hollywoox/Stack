#include "stack.h"


int main()
{
    Stack stk1 = {};
    StackCtor(&stk1, 1);
    StackPush(&stk1, 3);

    StackDump(&stk1);

    StackPush(&stk1, 10);
    StackPush(&stk1, 15); 

    StackDump(&stk1);

    StackPop(&stk1);

    StackDump(&stk1);

    StackPop(&stk1);
    StackPop(&stk1);
    //StackPop(&stk1); 
    StackDump(&stk1);

    StackPush(&stk1, 36);
    StackDump(&stk1);
    StackPush(&stk1, 49);
    StackPush(&stk1, 50);
    StackPush(&stk1, 51);
    StackPush(&stk1, 52);
    //StackPush(&stk1, 53);
    //StackPush(&stk1, 54);
    StackDump(&stk1);    

    StackDtor(&stk1);

    return 0;
}