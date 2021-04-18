#include "Number.h"

number new_number(int block, int number)
{
    struct number_type n = {.number = number, .block = block};
    return n;
}
