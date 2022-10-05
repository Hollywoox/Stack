#include "stack.h"

void StackCtor_(Stack* stk, size_t capacity, const char* name, const char* func, const char* file, int line)
{
    if(stk == NULL)
    {
        ASSERT_OK(stk);
    }

    stk->capacity = capacity;
    stk->size = 0;
    stk->error = 0;
    stk->left_protector = CANARY;
    stk->right_protector = CANARY;
    stk->data = (elem_t*)calloc(1, sizeof(elem_t) * capacity + 2 * sizeof(canary));
    if(stk->data == NULL)
    {
        ASSERT_OK(stk);
    }

    *((canary*)stk->data) = CANARY;
    *((canary*)(stk->data + SHIFT + capacity)) = CANARY;

    stk->data = stk->data + SHIFT;

    stk->st_info.name = name;
    stk->st_info.func = func;
    stk->st_info.file = file;
    stk->st_info.line = line;

    for(int i = 0; i < stk->capacity; ++i)
    {
        stk->data[i] = POISON;
    }

    ASSERT_OK(stk);
    //count_hash
}

void StackPush(Stack* stk, elem_t value)
{
    ASSERT_OK(stk);

    if(stk->size == stk->capacity)
    {
        StackResizeUp(stk);
    }

    stk->data[stk->size++] = value;
    //count_hash

    ASSERT_OK(stk);
}

elem_t StackPop(Stack* stk)
{
    ASSERT_OK(stk);
    
    if(stk->size == 0)
    {
        stk->size--;
        ASSERT_OK(stk);
    }

    if((stk->size - 1 == (stk->capacity / 2)) && (stk->capacity % 2 == 0))
    {
        StackResizeDown(stk);
    }

    stk->size--;
    elem_t last = stk->data[stk->size];
    stk->data[stk->size] = POISON;
    //count_hash

    return last;
}

void StackResizeUp(Stack* stk)
{
    ASSERT_OK(stk);

    stk->data = stk->data - SHIFT;
    stk->data = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t) * 2 + 2 * sizeof(canary));
    if(stk->data == NULL)
    {
        ASSERT_OK(stk);
    }

    *((canary*)stk->data) = CANARY;
    *((canary*)(stk->data + SHIFT + stk->capacity * 2)) = CANARY;

    stk->data = stk->data + SHIFT;

    for(int i = stk->capacity; i < 2 * stk->capacity; ++i)
    {
        stk->data[i] = POISON;
    }    

    stk->capacity *= 2;
}

void StackResizeDown(Stack* stk)
{
    ASSERT_OK(stk);

    stk->data = stk->data - SHIFT;
    stk->data = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t) / 2 + 2 * sizeof(canary));
    if(stk->data == NULL)
    {
        ASSERT_OK(stk);
    }

    *((canary*)stk->data) = CANARY;
    *((canary*)(stk->data + SHIFT + stk->capacity / 2)) = CANARY;

    stk->data = stk->data + SHIFT;
    stk->capacity /= 2;
}

int StackVerify(Stack* stk)
{
    if(stk == NULL)
    {
        FILE* log = fopen("log.txt", "a");
        fprintf(log, "Error: stack pointer is NULL[%p]\n", stk);
        fclose(log);
        abort();
    }

    if((stk->data == NULL) && ((stk->error & NULL_DATA_POINTER) == 0)) 
    {
        stk->error += (int)NULL_DATA_POINTER;
        return stk->error;
    }

    if(((long long)stk->capacity < 0) && ((stk->error & BAD_CAPACITY) == 0))
    {
        stk->error += (int)BAD_CAPACITY;
    }

    if((stk->size > stk->capacity) && ((stk->error & BIG_SIZE) == 0))
    {
        stk->error += (int)BIG_SIZE;
    }

    if(((long long)stk->size < 0) && ((stk->error & BAD_SIZE) == 0))
    {
        stk->error += (int)BAD_SIZE;
    }

    if((stk->left_protector != CANARY) && ((stk->error & LEFT_ST_CANARY_DEAD) == 0))
    {
        stk->error += (int)LEFT_ST_CANARY_DEAD;
    }

    if((stk->right_protector != CANARY) && ((stk->error & RIGHT_ST_CANARY_DEAD) == 0))
    {
        stk->error += (int)RIGHT_ST_CANARY_DEAD;
    } 

    if((*((canary*)(stk->data - SHIFT)) != CANARY) && ((stk->error & LEFT_DATA_CANARY_DEAD) == 0))
    {
        stk->error += (int)LEFT_DATA_CANARY_DEAD;
    }

    if((*((canary*)(stk->data + stk->capacity)) != CANARY) && ((stk->error & RIGHT_DATA_CANARY_DEAD) == 0))
    {
        stk->error += (int)RIGHT_DATA_CANARY_DEAD;
    }

    if((stk->data != NULL) && ((stk->error & ELEM_POISONED) == 0))
    {
        for(int i = 0; i < stk->size; ++i)
        {
            if(stk->data[i] == POISON)
            {
                stk->error += (int)ELEM_POISONED;
                break;
            }
        }
    }

    return stk->error;
}

void StackPrint(Stack* stk, FILE* log)
{
    for(int i = 0; i < stk->capacity; ++i)
    {
        if(stk->data[i] != POISON)
        {
            fprintf(log, "\t\t*[%d] = ", i);
            fprintf(log, elem_fmt, stk->data[i]);
            fprintf(log, "\n");
        }
        else{
            fprintf(log, "\t\t[%d] = NAN(POISON)\n", i); 
        }
    }
}

void StackDump_(Stack* stk, const char* function, const char* file, int line)
{
    FILE* log = fopen("log.txt", "a");

    if(stk == NULL)
    {
        ASSERT_OK(stk);
    }

    const char* status = (StackVerify(stk) != 0) ? "error" : "ok";

    fprintf(log, "%s() at %s(%d):\n", function, file, line);
    fprintf(log, "Stack[%p](%s) \"%s\" at %s() at %s(%d)\n", stk, status, stk->st_info.name, stk->st_info.func, stk->st_info.file, stk->st_info.line);
    fprintf(log, "{\n");
    fprintf(log, "\tsize = %zu\n", stk->size);
    fprintf(log, "\tcapacity = %zu\n", stk->capacity);
    fprintf(log, "\terror code = %d\n", stk->error);
    fprintf(log, "\tleft stack canary = %llu\n", stk->left_protector);
    fprintf(log, "\tright stack canary = %llu\n", stk->right_protector);
    fprintf(log, "\tdata[%p]\n", stk->data);
    fprintf(log, "\t{\n");
    StackPrint(stk, log);
    fprintf(log, "\t}\n");
    fprintf(log, "}\n\n");

    fclose(log);
}

void PrintError(Stack* stk)
{
    FILE* log = fopen("log.txt", "a");

    if((stk->error & NULL_DATA_POINTER) != 0)
    {
        fprintf(log, "Error: data pointer is NULL[%p]\n", stk->data);
    }

    if((stk->error & BAD_CAPACITY) != 0)
    {
        fprintf(log, "Error: capacity is negative\n");
    }

    if((stk->error & BAD_SIZE) != 0)
    {
        fprintf(log, "Error: size is negative\n");
    }

    if((stk->error & BIG_SIZE) != 0)
    {
        fprintf(log, "Error: size is bigger than capacity\n");
    }

    if((stk->error & LEFT_ST_CANARY_DEAD) != 0)
    {
        fprintf(log, "Error: oh no! Left canary died!\n");
    }     

    if((stk->error & RIGHT_ST_CANARY_DEAD) != 0)
    {
        fprintf(log, "Error: oh no! Right canary died!\n");
    } 

    if((stk->error & LEFT_DATA_CANARY_DEAD) != 0)
    {
        fprintf(log, "Error: Shit! Left canary in data died!\n");
    }

    if((stk->error & RIGHT_DATA_CANARY_DEAD) != 0)
    {
        fprintf(log, "Error: Shit! Right canary in data died!\n");
    }

    if((stk->error & ELEM_POISONED) != 0)
    {
        fprintf(log, "Error: There are some innocent poisoned elements in the stack!\n");
    }

    fclose(log);
}

void StackDtor(Stack* stk)
{
    if(stk == NULL)
    {
        ASSERT_OK(stk);
    }

    stk->data = stk->data - SHIFT;
    free(stk->data);
    stk->data = NULL;

    stk->capacity = 0;
    stk->size = 0;
    stk->error = 0;
}
