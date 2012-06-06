#ifndef INPUT_H
#define INPUT_H
#include "globals.h"

void inputInit();
void inputHandle();

typedef void (*inputcall_t)(void);

dir_t inputGetDirection();
#endif
