#ifndef UT_LIB_H
#define UT_LIB_H

int UT_create(int argc, char* argv[]);
int UT_buildindex(int argc, char* argv[]);
int UT_destroy(int argc, char* argv[]);
int UT_quit(void);
int UT_print(char* relName);
void UT_PrintError(char *errString);

#endif /* UT_LIB_H */
