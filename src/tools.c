#include "../include/tools.h"
#include <stdlib.h>

inline int rand_int(int low, int high) { return (rand() % (high - low)) + low; }
