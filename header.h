#ifndef HEADER_H
#define HEADER_H
#define DEBUG 1

#ifdef DEBUG
#define DBG(format, ...) do {\
        printf("DBG: \"");\
        printf(format, ##__VA_ARGS__);\
        printf("\" function: %s file: %s line: %d\n",(char*) __func__, (char*)__FILE__, __LINE__);} while(0);
#define DBG_EXIT(format, ...) do {\
        DBG(format, ##__VA_ARGS__)\
        exit(EXIT_FAILURE);} while(0);
#else
#define DBG(format, ...) do {} while(0);
#define DBG_EXIT(format, ...) do {} while(0);
#endif

#include "FatalException.h"
#endif