#ifndef AFD_H
#define AFD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AFN.h"

void test_mot(struct Automate A1, char* mot);
struct Automate determiniser(struct Automate A1);
struct Automate minimiser(struct Automate A1);


#endif
