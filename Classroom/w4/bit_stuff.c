#include <limits.h>
#include "bit_stuff.h"

#define BITS_PER_BYTE 8
#define NR_BITS_IN_TYPE(type) (BITS_PER_BYTE * sizeof(type))

unsigned int MakeBitmask(unsigned int width, unsigned int shift)
/* pre : -
 * post: a bitmask with a given width and a given shift is generated (so w=5 and s=1 gives 00111110)
 */
{
    // TODO: implement
    return 0x0;
}

unsigned int CountOnes(unsigned int value)
/* pre : -
 * post: the number of bits with value 1 is counted and returned
 */
{
    // TODO: implement
    return 0;
}
