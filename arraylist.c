#include <stdio.h>;
#include <stdlib.h>;
#include <unistd.h>;
#include "arraylist.h";

void al_init(array_t *, unsigned);
void al_destroy(array_t *);

void al_append(array_t, int);