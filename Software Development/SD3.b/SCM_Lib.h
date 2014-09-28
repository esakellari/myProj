#ifndef SCM_LIB_H
#define SCM_LIB_H

int SCM_create(int argc, char* argv[]);
int SCM_buildIndex(int argc, char* argv[]);
int SCM_destroy(int argc, char* argv[]);
int SCM_print(char* relName);
int SCM_insert(int argc, char* argv[]);
int SCM_delete(int argc, char* argv[]);
int SCM_select(int argc, char* argv[]);
int SCM_join(int argc, char* argv[]);

#endif /* SCM_LIB_H */
