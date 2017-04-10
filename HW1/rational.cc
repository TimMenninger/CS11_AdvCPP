#include "rational.hh"
#include "common.hh"

using namespace std;

/*
 simplify

 Simplifies the Rational values so that m_num/m_denom is in simplest form.

 Arguments:     None.

 Returns:       None.

 Global Impact: Updates m_num and m_denom
*/
void Rational::simplify() {
    // Get GCD of numerator and denominator to divide through
    int gcd = GCD(m_num, m_denom);

    // New values are in simplest form after dividing out GCD
    m_num   /= gcd;
    m_denom /= gcd;

    // Ensure that the denominator is positive
    m_num *= sign(m_denom);
    m_denom *= sign(m_denom);

    return;
}

/******************************
 CONSTRUCTORS
 ******************************/
 Rational::Rational(int n, int d) : m_num(n), m_denom(d) {
     // Can't have a zero in the denominator of a rational number
     if (d == 0)
     throw invalid_argument("Denominator must be nonzero.");

     // Simplify values
     simplify();

     return;
 }
Rational::Rational(int n) : m_num(n), m_denom(1) { }
Rational::Rational() : m_num(0), m_denom(1) { }

/******************************
 ARITHMETIC
 ******************************/
/*
 reciprocal

 Returns the reciprocal of the Rational values

 Arguments:     None.

 Returns:       None.

 Global Impact: None.
*/
Rational Rational::reciprocal() {
    return Rational(m_denom, m_num);
}

/******************************
 ACCESSORS
 ******************************/
/*
 denom

 Returns the denominator of this Rational instance.

 Arguments:     None.

 Returns:       m_denom (int) - The denominator.

 Global Impact: None.
*/
int Rational::denom() {
    return m_denom;
}

/*
 num

 Returns the numerator of this Rational instance.

 Arguments:     None.

 Returns:       m_num (int) - The numerator.

 Global Impact: None.
*/
int Rational::num() {
    return m_num;
}

/******************************
 OPERATORS
 ******************************/
/*
 unary not

 Returns the unary not of this Rational instance by returning ~numerator /
 ~denominator

 Arguments:     None.

 Returns:       (Rational) - Unary not of this Rational instance.

 Global Impact: None.
*/
const Rational Rational::operator~() {
    // Do unary not for numerator and denominator
    int n = ~m_num;
    int d = ~m_denom;
    return Rational(n, d);
}

/*
 Add

 Returns the sum of this Rational instance and the argued one.

 Arguments:     r (Rational&) - Added to this Rational instance.

 Returns:       (Rational) - Sum of this Rational and argued one.

 Global Impact: None.
*/
const Rational Rational::operator+(const Rational& r) {
    // Put fractions in common denominator, which we can do by multiplying
    // each by denominator/denominator of the other
    int d = this->m_denom * ((Rational) r).denom();
    int n = this->m_num * ((Rational) r).denom() +
               ((Rational) r).num() * this->m_denom;

    return Rational(n, d);
}

/*
 Subtract

 Returns the difference of this Rational instance and the argued one.  This
 works by adding the negative of the argument.

 Arguments:     r (Rational&) - Subtracted from this Rational instance.

 Returns:       (Rational) - Difference of this Rational and argued one.

 Global Impact: None.
*/
const Rational Rational::operator-(const Rational& r) {
    // Add the negative
    Rational out = *this + ((Rational) r * Rational(-1));
    out.simplify();

    return out;
}

/*
 Divide

 Returns the quotient of this Rational instance and the argued one.  This works
 by multiplying by the reciprocal of the argument.

 Arguments:     r (Rational&) - This Rational instance divided by r

 Returns:       (Rational) - Quotient of this Rational and argued one.

 Global Impact: None.
*/
const Rational Rational::operator/(const Rational& r) {
    // Multiply by reciprocal
    Rational out = *this * ((Rational) r).reciprocal();
    out.simplify();

    return out;
}

/*
 Multiply

 Returns the product of this Rational instance and the argued one.

 Arguments:     r (Rational&) - Multiplied by this Rational instance.

 Returns:       (Rational) - Product of this Rational and argued one.

 Global Impact: This instance overwritten with result.
*/
const Rational Rational::operator*(const Rational& r) {
    // Multiply numerators with each other and then denominators
    int n = this->m_num * ((Rational) r).num();
    int d = this->m_denom * ((Rational) r).denom();

    return Rational(n, d);
}

/*
 Add

 Returns the sum of this Rational instance and the argued one and saves the
 value in the instance.

 Arguments:     r (Rational&) - Added to this Rational instance.

 Returns:       (Rational) - Sum of this Rational and argued one.

 Global Impact: This instance overwritten with result.
*/
const Rational& Rational::operator+=(const Rational& r) {
    // Add and then store in this instance
    *this = *this + r;
    return *this;
}

/*
 Subtract

 Returns the difference of this Rational instance and the argued one.  This
 works by adding the negative of the argument.  It saves the result in this
 instance.

 Arguments:     r (Rational&) - Subtracted from this Rational instance.

 Returns:       (Rational) - Difference of this Rational and argued one.

 Global Impact: This instance overwritten with result.
*/
const Rational& Rational::operator-=(const Rational& r) {
    // Subtract and store result in this instance
    *this = *this - r;
    return *this;
}

/*
 Divide

 Returns the quotient of this Rational instance and the argued one.  This works
 by multiplying by the reciprocal of the argument.  The result is stored in
 this instance.

 Arguments:     r (Rational&) - This Rational instance divided by r

 Returns:       (Rational) - Quotient of this Rational and argued one.

 Global Impact: None.
*/
const Rational& Rational::operator/=(const Rational& r) {
    // Divide then return
    *this = *this / r;
    return *this;
}

/*
 Multiply

 Returns the product of this Rational instance and the argued one.  Then the
 result is stored in this instance.

 Arguments:     r (Rational&) - Multiplied by this Rational instance.

 Returns:       (Rational) - Product of this Rational and argued one.

 Global Impact: This instance overwritten with result.
*/
const Rational& Rational::operator*=(const Rational& r) {
    // Multiply then return
    *this = *this * r;
    return *this;
}

/*
 Pre Increment

 Increments the Rational instance, overwriting it, then returns the incremented
 Rational.

 Arguments:     None.

 Returns:       (Rational) - Incremented version of rational

 Global Impact: This instance overwritten with result.
*/
const Rational& Rational::operator++() {
    // Add one then return
    *this = *this + Rational(1);
    return *this;
}

/*
 Pre Decrement

 Increments the Rational instance, overwriting it, then returns the decremented
 Rational.

 Arguments:     None.

 Returns:       (Rational) - Decremented version of rational

 Global Impact: This instance overwritten with result.
*/
const Rational& Rational::operator--() {
    // Subtract one then return
    *this = *this + Rational(-1);
    return *this;
}

/*
 Post Increment

 Increments the Rational instance, overwriting it, then returns the original
 Rational.

 Arguments:     None.

 Returns:       (Rational) - Original version of rational

 Global Impact: This instance overwritten with result.
*/
const Rational Rational::operator++(int) {
    // Store the original version that will be returned
    Rational out = *this;

    // Increment the instance
    ++*this;

    return out;
}

/*
 Post Decrement

 Decrements the Rational instance, overwriting it, then returns the original
 Rational.

 Arguments:     None.

 Returns:       (Rational) - Original version of rational

 Global Impact: This instance overwritten with result.
*/
const Rational Rational::operator--(int) {
    // Store the original version that will be returned
    Rational out = *this;

    // Decrement the instance
    --*this;

    return out;
}

/******************************
 COMPARATORS
 ******************************/
/*
 Less than operator

 Returns true if this Rational instance is less than the argued one.

 Arguments:     r (Rational) - Instance this is compared to

 Returns:       (bool) - True if this is less than r
*/
const bool Rational::operator<(Rational& r) {
    return (double) *this < (double) r;
}

/*
 Greater than operator

 Returns true if this Rational instance is greater than the argued one.

 Arguments:     r (Rational) - Instance this is compared to

 Returns:       (bool) - True if this is greater than r
*/
const bool Rational::operator>(Rational& r) {
    return (double) *this > (double) r;
}

/*
 Less than or equal to operator

 Returns true if this Rational instance is less than or equal to the argued
 one.

 Arguments:     r (Rational) - Instance this is compared to

 Returns:       (bool) - True if this is less than or equal to r
*/
const bool Rational::operator<=(Rational& r) {
    return (double) *this <= (double) r;
}

/*
 Greater than or equal to operator

 Returns true if this Rational instance is greater than or equal to the argued
 one.

 Arguments:     r (Rational) - Instance this is compared to

 Returns:       (bool) - True if this is greater than or equal to r
*/
const bool Rational::operator>=(Rational& r) {
    return (double) *this >= (double) r;
}

/*
 Equal to operator

 Returns true if this Rational instance is equal to the argued one.

 Arguments:     r (Rational) - Instance this is compared to

 Returns:       (bool) - True if this is equal to r
*/
const bool Rational::operator==(Rational& r) {
    return (double) *this == (double) r;
}

/*
 Not equal to operator

 Returns true if this Rational instance is not equal to the argued one.

 Arguments:     r (Rational) - Instance this is compared to

 Returns:       (bool) - True if this is not equal to r
*/
const bool Rational::operator!=(Rational& r) {
    return (double) *this != (double) r;
}

/******************************
 IMPLICIT CASTING
 ******************************/
/*
 float implicit converter

 Converts the Rational into a float.

 Arguments:     None.

 Returns:       (float) - The Rational as a float.
*/
const Rational::operator float() {
    return (float) m_num / m_denom;
}

/*
 double implicit converter

 Converts the Rational into a double.

 Arguments:     None.

 Returns:       (double) - The Rational as a double.
*/
const Rational::operator double() {
    return (double) m_num / m_denom;
}

/******************************
 STREAM OUTPUT
 ******************************/

/*
 ostream operator

 Outputs the Rational onto ostream.  If the denominator is 1, then only the
 numerator is output.

 Arguments:     None.

 Returns:       (ostream) - The ostream that the Rational is output onto
*/
ostream& operator<<(ostream& out, const Rational& r) {
    // Get the numerator and denominator for easier accessing
    int n = ((Rational) r).num();
    int d = ((Rational) r).denom();
    // Put the numerator on the ostream. Only put the denominator if it != 1
    out << n;
    if (d != 1)
        out << "/" << d;

    return out;
}
