
#include "picoc_api.h"

void MsvcSetupFunc(Picoc *pc)
{
}

void picoc_api_speedtest  (struct ParseState *Parser, struct Value *ReturnValue,
						   struct Value **Param, int NumArgs)
{
	for (volatile int i = 0; i < 10000000; ++i) { volatile int a; a = i * i; }
}

/* list of all library functions and their prototypes */
struct LibraryFunction MsvcFunctions[] =
{
		{picoc_api_speedtest, "void api_speedtest(void);"},
		{NULL, NULL}
};

void PlatformLibraryInit(Picoc *pc)
{
	IncludeRegister(pc, "api.h", &MsvcSetupFunc, &MsvcFunctions[0], NULL);
}


