#include <assert.h>
#include <string.h>

#include "c4/c4.h"

int main(void)
{
    assert(strcmp(c4_hello(), "Hello, World!") == 0);
    return 0;
}
