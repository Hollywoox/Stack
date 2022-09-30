#include "stack.h"

void StackCtor_(Stack* stk, size_t capacity, const char* name, const char* func, const char* file, int line)
{
    if(stk == NULL)
    {
        printf("Error: stack pointer is NULL[%p]\n", stk);
        abort();
    }

    stk->capacity = capacity;
    stk->size = 0;
    stk->error = 0;
    stk->data = (elem_t*)calloc(capacity, sizeof(elem_t));

    stk->st_info.name = name;
    stk->st_info.func = func;
    stk->st_info.file = file;
    stk->st_info.line = line;
    
    ASSERT_OK(stk);

    StackFill(stk);
}

void StackPush(Stack* stk, elem_t value)
{
    ASSERT_OK(stk);

    if(stk->size == stk->capacity)
    {
        StackResizeUp(stk);
    }

    stk->data[stk->size++] = value;

    ASSERT_OK(stk);
}

elem_t StackPop(Stack* stk)
{
    ASSERT_OK(stk);
    
    stk->size--;

    ASSERT_OK(stk);

    if((stk->size == (stk->capacity / 2)) && (stk->capacity % 2 == 0))
    {
        StackResizeDown(stk);
    }

    elem_t last = stk->data[stk->size];
    stk->data[stk->size] = poison;

    ASSERT_OK(stk);
    return last;
}

void StackResizeUp(Stack* stk)
{
    ASSERT_OK(stk);

    stk->data = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t) * 2);
    stk->capacity *= 2;

    ASSERT_OK(stk);
}

void StackFill(Stack* stk)
{
    ASSERT_OK(stk);

    for(int i = 0; i < stk->capacity; ++i)
    {
        stk->data[i] = poison;
    }
}

void StackResizeDown(Stack* stk)
{
    ASSERT_OK(stk);

    stk->data = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t) / 2);
    stk->capacity /= 2;

    ASSERT_OK(stk); 
}

int StackVerify(Stack* stk)
{
    if(stk == NULL)
    {
        stk->error = NULL_ST_POINTER;
        return stk->error;
    }

    if(stk->data == NULL)
    {
        stk->error += pow(2, (int)NULL_DATA_POINTER);
    }

    if(stk->capacity < 0)
    {
        stk->error += pow(2, (int)BAD_CAPACITY);
    }

    if(stk->size > stk->capacity)
    {
        stk->error += pow(2, (int)BIG_SIZE);
    }

    if(stk->size < 0)
    {
        stk->error += pow(2, (int)BAD_SIZE);
    }

    return stk->error;
}

void StackPrint(Stack* stk)
{
    for(int i = 0; i < stk->capacity; ++i)
    {
        if(i < stk->size)
        {
            printf("\t\t*[%d] = ", i);
            printf(elem_fmt, stk->data[i]);
            printf("\n");
        }
        else{
            printf("\t\t[%d] = NAN(POISON)\n", i); 
        }
    }
}

void StackDump(Stack* stk)
{
    if(stk == NULL)
    {
        printf("Error: stack pointer is NULL[%p]\n", stk);
        abort();
    }

    const char* status = (StackVerify(stk) != 0) ? "error" : "ok";

    printf("%s() at %s(%d):\n", __FUNCTION__, __FILE__, __LINE__);
    printf("Stack[%p](%s) \"%s\" at %s() at %s(%d)\n", stk, status, stk->st_info.name, stk->st_info.func, stk->st_info.file, stk->st_info.line);
    printf("{\n");
    printf("\tsize = %zu\n", stk->size);
    printf("\tcapacity = %zu\n", stk->capacity);
    printf("\tdata[%p]\n", stk->data);
    printf("\t{\n");
    StackPrint(stk);
    printf("\t}\n");
    printf("}\n\n");
}

void PrintError(Stack* stk)
{
    if((stk->error & 2) != 0)
    {
        printf("Error: data pointer is NULL[%p]\n", stk->data);
    }

    if((stk->error & 4) != 0)
    {
        printf("Error: capacity is negative\n");
    }

    if((stk->error & 8) != 0)
    {
        printf("Error: size is negative\n");
    }

    if((stk->error & 16) != 0)
    {
        printf("Error: size is bigger than capacity\n");
    }
}

void StackDtor(Stack* stk)
{
    if(stk == NULL)
    {
        printf("Error: stack pointer is NULL[%p]\n", stk);
        abort();
    }

    free(stk->data);
    stk->data = NULL;

    stk->capacity = 0;
    stk->size = 0;
    stk->error = 0;
}
