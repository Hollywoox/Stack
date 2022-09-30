#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define DOUBLE

#ifdef INT
    typedef int elem_t;
    static const char* elem_fmt = "%d";
    const int poison = 0xDEADF00D;
#endif

#ifdef DOUBLE
    typedef double elem_t;
    static const char* elem_fmt = "%f";
    const double poison = -6789456123.435612;
#endif

#ifdef CHAR
    typedef char elem_t;
    static const char* elem_fmt = "%c";
    const char poison = '\0';
#endif


#define StackCtor(stk, size) StackCtor_((stk), (size), #stk, __FUNCTION__, __FILE__, __LINE__)

#define ASSERT_OK(stk) if(StackVerify(stk) != 0)  \
                       {                          \
                           StackDump(stk);        \
                           PrintError(stk);       \
                           abort();               \
                       }


typedef struct 
{
    const char* name;
    const char* func;
    const char* file;
    int line;
} varinfo;

typedef struct
{
    elem_t* data;
    size_t size;
    size_t capacity;
    int error;

    varinfo st_info;
} Stack;


enum errors
{
    NULL_ST_POINTER,
    NULL_DATA_POINTER,
    BAD_CAPACITY,
    BAD_SIZE,
    BIG_SIZE
};


/* creates a stack with the necessary capacity */
void StackCtor_(Stack* stk, size_t capacity, const char* name, const char* func, const char* file, int line);

/* pushs a new element in the end of the stack */
void StackPush(Stack* stk, elem_t value);

/* deletes and returns the last element of the stack */
elem_t StackPop(Stack* stk);

/* increases the capacity of the stack */
void StackResizeUp(Stack* stk);

/* reduces the capacity of the stack */
void StackResizeDown(Stack* stk);

/* fill stack with poison */
void StackFill(Stack* stk);

/* looks for errors in the stack and returns the sum */
int StackVerify(Stack* stk);

/* prints the elements of the stack */
void StackPrint(Stack* stk);

/* prints the information about the stack */
void StackDump(Stack* stk);

/* prints the information about errors */
void PrintError(Stack* stk);

/* deletes the whole stack and clears the memory */
void StackDtor(Stack* stk);