#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<ctype.h>
#define CHECK if (chr == EOF) return;
//structs
struct TAG_S_DEFINE_VARIABLE {
	char* key;
	char* value;
}typedef S_DEFINE_VARIABLE;

struct TAG_S_ALL_DEFINE_VARIABLES {
	S_DEFINE_VARIABLE* array;
	int length;
}typedef S_ALL_DEFINE_VARIABLES;
//global variables
S_ALL_DEFINE_VARIABLES Defines;
int isCommentInDefine = 0;
int isCommentInDefineValue = 0;
char* partOfValue = NULL;
int isDefineStartLine = 0;
int isDefineString = 0;
//prototypes
char* f_getNewFileName(char* name);
void f_secondSleshMode(FILE* srcFile, FILE* desFile);
void f_sleshStarhMode(FILE* srcFile, FILE* desFile);
void f_firstSleshMode(FILE* srcFile, FILE* desFile);
void f_startNewLine(FILE* srcFile, FILE* desFile);
void f_charsMode(FILE* srcFile, FILE* desFile);
void f_sleshStarStarhMode(FILE* srcFile, FILE* desFile);
void f_hashMode(FILE* srcFile, FILE* desFile);
void f_hashIMode(FILE* srcFile, FILE* desFile);
void f_hashDMode(FILE* srcFile, FILE* desFile);
void f_defineMode(FILE* srcFile, FILE* desFile);
void f_readDefineName(FILE* srcFile, FILE* desFile, char* name);
void f_defineMode(FILE* srcFile, FILE* desFile);
void f_readDefineValue(FILE* srcFile, FILE* desFile, char* value);
void f_addDefineValue(char* defineValue);
void f_defineNewLine(FILE* srcFile, FILE* desFile, char* value);
void f_defineValueMode(FILE* srcFile, FILE* desFile);
void f_compareWithDefine(char* word, FILE* desFile);
void f_writeDefineContent(FILE* desFile, int index);
void f_readIncludeFileName(FILE* srcFile, FILE* desFile, char* fileName);
void f_readIncludeFileContent(FILE* desFile, char* fileName);
void f_stringMode(FILE* srcFile, FILE* desFile);
char f_getAllWord(FILE* srcFile, char firstChar, char** word);
//implementations
void f_secondSleshMode(FILE* srcFile, FILE* desFile) {
	isCommentInDefine = 0;
	char chr = fgetc(srcFile);
	CHECK
		switch (chr)
		{
		case '\n':
			fputc(chr, desFile);
			f_startNewLine(srcFile, desFile);
			break;
		default:
			f_secondSleshMode(srcFile, desFile);
			break;
		}
}
void f_sleshStarStarhMode(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	CHECK
		switch (chr) {
		case '/': {
			if (!isCommentInDefine && !isCommentInDefineValue)
				f_charsMode(srcFile, desFile);
			break;
		}
		case '\n':
			fputc(chr, desFile);
		default:
			f_sleshStarhMode(srcFile, desFile);
			break;
		}
	return;
}
char* f_getNewFileName(char* name)
{
	char* newName = _strdup(name);
	char* dotPointer = strrchr(newName, '.');
	*dotPointer = '\0';
	newName = (char*)realloc(newName, sizeof(char) * (strlen(newName) + 4));
	if (newName != NULL)
	{
		strcat_s(newName, (strlen(name) + 2) * sizeof(char), "o.c");
	}
	return newName;
}
void f_sleshStarhMode(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	CHECK
		switch (chr)
		{
		case '*':
			f_sleshStarStarhMode(srcFile, desFile);
			break;
		case '\n':
			fputc(chr, desFile);
		default:
			f_sleshStarhMode(srcFile, desFile);
			break;
		}
}
void f_charsMode(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	char* word = NULL;
	CHECK
		if (isalpha(chr) || chr == '_') {
			word = (char*)realloc(word, sizeof(char) * 2);
			word[0] = chr;
			word[1] = 0;
			chr = f_getAllWord(srcFile, chr, &word);
			f_compareWithDefine(word, desFile);
			fputc(chr, desFile);
			return f_charsMode(srcFile, desFile);
		}
	switch (chr)
	{
	case '/':
	{
		f_firstSleshMode(srcFile, desFile);
		break;
	}
	case '"':
	case 39:
		fputc(chr, desFile);
		f_stringMode(srcFile, desFile);
		f_charsMode(srcFile, desFile);
		break;
	case '\n':
		fputc(chr, desFile);
		f_startNewLine(srcFile, desFile);
		break;
	default:
		fputc(chr, desFile);
		f_charsMode(srcFile, desFile);
		break;
	}
}
void f_firstSleshMode(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	CHECK
		switch (chr)
		{
		case '/':
		{isCommentInDefine = 0;
		if (isCommentInDefineValue) {
			f_addDefineValue(partOfValue);
			isCommentInDefineValue = 0;
		}
		f_secondSleshMode(srcFile, desFile);
		break;
		}
		case '*':
		{
			f_sleshStarhMode(srcFile, desFile);
			break;
		}
		case '\n':
			isCommentInDefine = 0;
			isCommentInDefineValue = 0;
			fputc(chr, desFile);
			f_startNewLine(srcFile, desFile);
			break;
		default:
			isCommentInDefine = 0;
			if (!isCommentInDefineValue)
			{
				fputc('/', desFile);
				fputc(chr, desFile);
				f_charsMode(srcFile, desFile);
			}
			else
			{
				isCommentInDefineValue = 0;
				int length = strlen(partOfValue);
				partOfValue = (char*)realloc(partOfValue, sizeof(char) * length + 2);
				partOfValue[length] = '/';
				partOfValue[length + 1] = chr;
				partOfValue[length + 2] = 0;
				f_readDefineValue(srcFile, desFile, partOfValue);
			}
			break;
		}
}
void f_addDefineName(char* defineName) {
	int defineNum = Defines.length;
	Defines.length++;
	Defines.array = (S_DEFINE_VARIABLE*)realloc(Defines.array, sizeof(S_DEFINE_VARIABLE) * Defines.length);
	Defines.array[defineNum].key = _strdup(defineName);
	Defines.array[defineNum].value = _strdup("");
	return;
}
void f_addDefineValue(char* defineValue) {
	int defineNum = Defines.length - 1;
	Defines.array[defineNum].value = _strdup(defineValue);
	return;
}
void f_defineNewLine(FILE* srcFile, FILE* desFile, char* value) {
	char chr = fgetc(srcFile);
	CHECK
		char* defineValue = _strdup(value);
	int length = 0;
	if (defineValue)
		length = strlen(defineValue);
	switch (chr)
	{
	case '\n':
		if (!isDefineString)
		{
			f_readDefineValue(srcFile, desFile, defineValue);
			break;
		}

	default:
		defineValue = (char*)realloc(defineValue, sizeof(char) * (length + 2));
		defineValue[length] = '\\';
		defineValue[length + 1] = chr;
		defineValue[length + 2] = 0;
		f_readDefineValue(srcFile, desFile, defineValue);
		break;
	}
}
void f_readDefineValue(FILE* srcFile, FILE* desFile, char* value) {
	char chr = fgetc(srcFile);
	char* defineValue = { 0 };
	defineValue = _strdup(value);
	int length = 0;
	if (defineValue)
		length = strlen(defineValue);
	CHECK
		switch (chr)
		{

		case'\n':
			f_addDefineValue(value);
			isDefineString = 0;
			f_startNewLine(srcFile, desFile);
			break;
		case '/':
			isCommentInDefineValue = 1;
			partOfValue = _strdup(value);
			f_firstSleshMode(srcFile, desFile);
			isCommentInDefineValue = 0;
			f_readDefineValue(srcFile, desFile, partOfValue);
			break;
		case '\\':
			f_defineNewLine(srcFile, desFile, value);
			break;
		case '"':
		case 39:
			isDefineString = 1;
		default:
			defineValue = (char*)realloc(defineValue, sizeof(char) * (length + 1));
			defineValue[length] = chr;
			defineValue[length + 1] = 0;
			f_readDefineValue(srcFile, desFile, defineValue);
		}
}
void f_readDefineName(FILE* srcFile, FILE* desFile, char* name) {
	char chr = fgetc(srcFile);
	char* defineName = { 0 };
	defineName = _strdup(name);
	int length = 0;
	if (defineName)
		length = strlen(defineName);
	CHECK
		switch (chr)
		{
		case ' ':
			f_addDefineName(name);
			f_readDefineValue(srcFile, desFile, "");
			break;
		case'\n':
			f_addDefineName(name);
			f_startNewLine(srcFile, desFile);
			break;
		case '/':
			isCommentInDefine = 1;
			f_addDefineName(name);
			f_firstSleshMode(srcFile, desFile);
			// if it happens
			isCommentInDefine = 0;
			f_readDefineValue(srcFile, desFile, "");
			break;
		default:
			defineName = (char*)realloc(defineName, sizeof(char) * (length + 1));
			defineName[length] = chr;
			defineName[length + 1] = 0;
			f_readDefineName(srcFile, desFile, defineName);
		}
}
void f_defineMode(FILE* srcFile, FILE* desFile) {
	char chr[2] = { 0 };
	chr[0] = fgetc(srcFile);
	if (chr[0] == EOF)
		return;
	switch (chr[0])
	{
	case ' ':
		f_defineMode(srcFile, desFile);
		break;
	case '\\':
		if ((chr[0] = fgetc(srcFile)) == '\n' && chr[0] != EOF)
			f_defineMode(srcFile, desFile);
		else
			f_charsMode(srcFile, desFile);
	case '/':
		isCommentInDefine = 1;
		f_firstSleshMode(srcFile, desFile);
		isCommentInDefine = 0;
		f_defineMode(srcFile, desFile);
		break;
	default:
		f_readDefineName(srcFile, desFile, chr);
	}
}
void f_defineValueMode(FILE* srcFile, FILE* desFile) {
	char chr[2] = { 0 };
	chr[0] = fgetc(srcFile);
	CHECK
		switch (chr[0])
		{
		case ' ':
			f_defineValueMode(srcFile, desFile);
			break;
		case '\n':
			if (isDefineStartLine)
			{
				isDefineStartLine = 0;
				f_defineValueMode(srcFile, desFile);
			}
			else
				f_startNewLine(srcFile, desFile);
			break;
		case '\\':
			isDefineStartLine = 1;
			f_defineValueMode(srcFile, desFile);
			break;
		case '/':
			isCommentInDefineValue = 1;
			f_firstSleshMode(srcFile, desFile);
			isCommentInDefineValue = 0;
			f_defineValueMode(srcFile, desFile);
			break;
		default:
			f_readDefineName(srcFile, desFile, chr);
		}
}
void f_readIncludeFileContent(FILE* desFile, char* fileName) {
	errno_t err;
	FILE* includeFile;
	if ((err = fopen_s(&includeFile, fileName, "r")) != 0) {
		perror("Could not open Include file");
		return exit(1);
	};
	f_startNewLine(includeFile, desFile);
}
void f_readIncludeFileName(FILE* srcFile, FILE* desFile, char* fileName) {
	char chr = fgetc(srcFile);
	char* includeName = { 0 };
	includeName = _strdup(fileName);
	int length = 0;
	if (includeName)
		length = strlen(includeName);
	CHECK
		switch (chr)
		{
		case ' ':
		case '"':
			if (*fileName)
				f_readIncludeFileContent(desFile, fileName);
			//f_charsMode(srcFile, desFile);
			else
				f_readIncludeFileName(srcFile, desFile, "");
			break;
		case '<':
			f_readIncludeFileName(srcFile, desFile, "");
			break;
		case '>':
			f_readIncludeFileContent(desFile, fileName);
			//f_charsMode(srcFile, desFile);
			break;
		default:
			includeName = (char*)realloc(includeName, sizeof(char) * (length + 1));
			includeName[length] = chr;
			includeName[length + 1] = 0;
			f_readIncludeFileName(srcFile, desFile, includeName);
			break;
		}
}
void f_hashIMode(FILE* srcFile, FILE* desFile) {
	char word[7] = { 0 };
	fgets(word, 7, srcFile);
	if (!strcmp(word, "nclude"))
		f_readIncludeFileName(srcFile, desFile, "");
	f_charsMode(srcFile, desFile);
}
void f_hashDMode(FILE* srcFile, FILE* desFile) {
	char word[6] = { 0 };
	fgets(word, 6, srcFile);
	if (strcmp(word, "efine"))
		f_charsMode(srcFile, desFile);
	else
		f_defineMode(srcFile, desFile);
}
void f_hashMode(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	CHECK
		switch (chr)
		{
		case ' ':
			f_hashMode(srcFile, desFile);
			break;
		case 'i':
			f_hashIMode(srcFile, desFile);
			break;
		case 'd':
			f_hashDMode(srcFile, desFile);
			break;
		default:
			f_charsMode(srcFile, desFile);
		}
}
void f_compareWithDefine(char* word, FILE* desFile) {
	for (int i = 0; i < Defines.length; i += 1)
		if (!strcmp(word, Defines.array[i].key))
			return	f_writeDefineContent(desFile, i);
	return fputs(word, desFile);
}
void f_stringMode(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	CHECK
		fputc(chr, desFile);
	switch (chr)
	{
	case '"':
	case 39:
		return;
	default:
		f_stringMode(srcFile, desFile);
		break;
	}
}
void f_writeDefineContent(FILE* desFile, int index)
{
	char* defineValue = Defines.array[index].value;
	while (*defineValue) {
		fputc(*defineValue, desFile);
		defineValue++;
	}
}
char f_getAllWord(FILE* srcFile, char firstChar, char** word) {
	int length = 2;
	char chr = fgetc(srcFile);
	CHECK
		while (isalpha(chr) || isdigit(chr) || chr == '_')
		{
			(*word) = (char*)realloc((*word), ++length * sizeof(char));
			(*word)[length - 2] = chr;
			(*word)[length - 1] = 0;
			chr = fgetc(srcFile);
			CHECK
		}
	return chr;
}
void f_startNewLine(FILE* srcFile, FILE* desFile) {
	char chr = fgetc(srcFile);
	char* word = NULL;
	CHECK
		if (isalpha(chr) || chr == '_') {
			word = (char*)realloc(word, sizeof(char) * 2);
			word[0] = chr;
			word[1] = 0;
			chr = f_getAllWord(srcFile, chr, &word);
			f_compareWithDefine(word, desFile);
			fputc(chr, desFile);
			return f_charsMode(srcFile, desFile);
		}
	switch (chr)
	{
	case '/':
	{
		f_firstSleshMode(srcFile, desFile);
		break;
	}
	case '"':
	case 39:
		fputc(chr, desFile);
		f_stringMode(srcFile, desFile);
		f_charsMode(srcFile, desFile);
		break;
	case '\n':
		fputc(chr, desFile);
		f_startNewLine(srcFile, desFile);
		break;
	case '#':
		f_hashMode(srcFile, desFile);
		break;
	default:
		fputc(chr, desFile);
		f_charsMode(srcFile, desFile);
		break;

	}
}
int main(int argc, char** argv) {
	if (argc != 2) {
		printf("no arggument");
		return EXIT_FAILURE;
	}
	Defines.length = 0;
	errno_t err;
	FILE* sourceFile, * outputFile;
	char* newFileName;
	newFileName = f_getNewFileName(argv[1]);
	if ((err = fopen_s(&sourceFile, argv[1], "r")) != 0) {
		perror("Could not open file");
		return EXIT_FAILURE;
	};
	if ((err = fopen_s(&outputFile, newFileName, "w")) != 0) {
		perror("could not open file");
		return EXIT_FAILURE;
	};
	f_startNewLine(sourceFile, outputFile);
	fclose(outputFile);
	return EXIT_SUCCESS;
}
