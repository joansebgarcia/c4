#include <assert.h>
#include <string.h>

#include "c4/c4.h"

int main(void)
{
    const char *result = c4_hello();
    assert(strcmp(result, "Hello, World!") == 0);
    if (strcmp(result, "Hello, World!") != 0) {
        return 1;
    }
    return 0;
}
