#include "lib.h"

#include "version.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}

size_t fact(size_t i) {
    if (i == 0) return 1;
    return (i*fact(i-1));
}