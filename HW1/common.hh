/*
 common.hh

 Contains common functions for the Rational class

 Revisions:
    09 Apr 2017 - Tim Menninger: Created with gcd and sign
*/

/*
 GCD

 Computes the greatest common divisor of the two inputs.

 Arguments:     a (int) - First value to find GCD of
                b (int) - Second value to find GCD of

 Returns:       GCD (int) - Greatest common divisor of a and b
*/
static int GCD(int a, int b) {
    return b == 0 ? a : GCD(b, a%b);
}

/*
 sign

 Computes the sign of the argued integer.  It returns 1 if positive, 0 if 0
 and -1 otherwise.

 Arguments:     n (int) - The value to return the sign of.

 Returns:       (int) - The sign of the argument.
*/
static int sign(int n) {
    return (n > 0 ? 1 : (n < 0 ? -1 : 0));
}
