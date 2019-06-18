
#include "status.h"

static char* unknownError = "Unknown Error";

static char* msg[] = {
    "ok",
    "File open failed",
    "Access refused"
    "File close failed"
    "Memory Allocation failed"
    "Wrong Date"
    "Full structure"
    "Empty structure"
    "Value not found"
    "value already exists"
    "Index out of bounds"
    "Unable to perform operation"

    "Unknown error"
};

char* message(status s) {
    return (s<0 || s>= ERRUNKNOWN) ? msg[ERRUNKNOWN] : msg[s];

}
