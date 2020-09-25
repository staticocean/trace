
#define _CRT_SECURE_NO_WARNINGS

#include "picoc.h"
#include "picoc_interpreter.h"

void MsvcSetupFunc(Picoc *pc)
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
struct LibraryFunction MsvcFunctions[] =
{
    {CTest, "void Test(int);"},
    {CLineNo, "int LineNo();"},
    {NULL, NULL}
};

void PlatformLibraryInit(Picoc *pc)
{
    IncludeRegister(pc, "picoc_msvc.h", &MsvcSetupFunc, &MsvcFunctions[0], NULL);
}





















#ifdef DEBUGGER
static int gEnableDebugger = true;
#else
static int gEnableDebugger = false;
#endif

/* mark where to end the program for platforms which require this */
jmp_buf PicocExitBuf;

void PlatformInit(Picoc *pc)
{
}

void PlatformCleanup(Picoc *pc)
{
}

/* get a line of interactive input */
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{
	if (Prompt != NULL)
		printf("%s", Prompt);
	
	fflush(stdout);
	return fgets(Buf, MaxLen, stdin);
}

/* get a character of interactive input */
int PlatformGetCharacter()
{
	fflush(stdout);
	return getchar();
}

/* write a character to the console */
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
	putchar(OutCh);
}

/* read a file into memory */
char *PlatformReadFile(Picoc *pc, const char *FileName)
{
	struct stat FileInfo;
	char *ReadText;
	FILE *InFile;
	int BytesRead;
	char *p;
	
	if (stat(FileName, &FileInfo))
		ProgramFailNoParser(pc, "can't read file %s\n", FileName);
	
	ReadText = malloc(FileInfo.st_size + 1);
	if (ReadText == NULL)
		ProgramFailNoParser(pc, "out of memory\n");
	
	InFile = fopen(FileName, "r");
	if (InFile == NULL)
		ProgramFailNoParser(pc, "can't read file %s\n", FileName);
	
	BytesRead = fread(ReadText, 1, FileInfo.st_size, InFile);
	if (BytesRead == 0)
		ProgramFailNoParser(pc, "can't read file %s\n", FileName);
	
	ReadText[BytesRead] = '\0';
	fclose(InFile);
	
	if ((ReadText[0] == '#') && (ReadText[1] == '!')) {
		for (p = ReadText; (*p != '\r') && (*p != '\n'); ++p) {
			*p = ' ';
		}
	}
	
	return ReadText;
}

/* read and scan a file for definitions */
void PicocPlatformScanFile(Picoc *pc, const char *FileName)
{
	char *SourceStr = PlatformReadFile(pc, FileName);
	PicocParse(pc, FileName, SourceStr, strlen(SourceStr), true, false, true,
			   gEnableDebugger);
}

/* exit the program */
void PlatformExit(Picoc *pc, int RetVal)
{
	pc->PicocExitValue = RetVal;
	longjmp(pc->PicocExitBuf, 1);
}

#undef _CRT_SECURE_NO_WARNINGS
