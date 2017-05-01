/*
 common.hh

 Contains functions that are common among vector files in HW2

 Revisions:
    09 Apr 2017 - Tim Menninger: Created
*/

#ifndef COMMON
#define COMMON

/*
 smallestPow2

 Returns the smallest power of two greater than or equal to the argument.  If
 the argument is 0 or negative, it returns 0.

 Arguments:     n (int) - Value to find smallest power of 2 greater than.

 Returns:       (int) - Smallest power of 2 greater than argument.
*/
static inline int smallestPow2(int n) {
    // Return 0 if n is less than or equal to 0
    if (n <= 0)
        return 0;

    // Otherwise find smallest power of 2 greater than argument
    int out = 1;
    // Double until greater than or equal to argument
    while (out < n)
        out <<= 1;
    return out;
}

/*
 ceil

 Returns the ceiling of a double.  If the argument is already an even integer,
 itself is returned.

 Arguments:     n (double) - Value to find ceiling of.

 Returns:       (int) - Ceiling of the argument.
*/
// static inline int ceil(double n) {
//     return (int) n == n ? n : (int) n + 1;
// }

#endif // ifndef COMMON
