#include "../include/tools.h"
#include <stdlib.h>

int rand_int(int low, int high) { return (rand() % (high - low)) + low; }
