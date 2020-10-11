
#include "picoc_api.h"

void trj_std_setup(Picoc *pc)
{
}

void CTest (struct ParseState *Parser, struct Value *ReturnValue,
	struct Value **Param, int NumArgs)
{
    printf("test(%d)\n", Param[0]->Val->Integer);
    Param[0]->Val->Integer = 1234;
}

void CLineNo (struct ParseState *Parser, struct Value *ReturnValue,
	struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = Parser->Line;
}

/* list of all library functions and their prototypes */
struct LibraryFunction trj_std[] =
{
    {CTest, "void Test(int);"},
    {CLineNo, "int LineNo();"},
    {NULL, NULL}
};

void PlatformLibraryInit(Picoc *pc)
{
    IncludeRegister(pc, "trj_std.h", &trj_std_setup, &trj_std[0], NULL);
}

