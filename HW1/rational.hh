/*
 rational.hh

 Contains the Rational class definition.

 Revisions:
    09 Apr 2017 - Tim Menninger: Created
*/

#ifndef RATIONAL
#define RATIONAL

#include <iostream>

/*
 Rational Class

 Emulates rational numbers, their representation and common arithmetic that
 can be done to them.
*/
class Rational {
private:
    /******************************
     MEMBERS
     ******************************/
    int m_num;                              // Numerator
    int m_denom;                            // Denominator

    /******************************
     PRIVATE METHODS
     ******************************/
    void simplify();                        // Simplifies the fraction

public:
    /******************************
     CONSTRUCTORS
     ******************************/
    Rational();                             // Initializes to 0/1
    Rational(int n);                        // Initializes to n/1
    Rational(int n, int d);                 // Initializes to n/d

    /******************************
     DESTRUCTOR
     ******************************/
    ~Rational();                            // Does nothing

    /******************************
     ACCESSORS
     ******************************/
    int denom();                            // Returns denominator
    int num();                              // Returns numerator

    /******************************
     ARITHMETIC
     ******************************/
    Rational reciprocal();                  // Returns the reciprocal

    /******************************
     OPERATORS
     ******************************/
    const Rational operator~();                   // Unary not
    const Rational operator+(const Rational& r);  // Add
    const Rational operator-(const Rational& r);  // Subtract
    const Rational operator/(const Rational& r);  // Divide
    const Rational operator*(const Rational& r);  // Multiply
    const Rational& operator+=(const Rational& r);// Add
    const Rational& operator-=(const Rational& r);// Subtract
    const Rational& operator/=(const Rational& r);// Divide
    const Rational& operator*=(const Rational& r);// Multiply

    const Rational& operator++();                 // Pre increment
    const Rational& operator--();                 // Pre decrement
    const Rational operator++(int);               // Post increment
    const Rational operator--(int);               // Post decrement

    /******************************
     COMPARATORS
     ******************************/
    const bool operator<(Rational& r);            // Less than
    const bool operator>(Rational& r);            // Greater than
    const bool operator<=(Rational& r);           // Less than or equal to
    const bool operator>=(Rational& r);           // Greater than or equal to
    const bool operator==(Rational& r);           // Equal to
    const bool operator!=(Rational& r);           // Not equal to

    /******************************
     IMPLICIT CASTING
     ******************************/
    const operator float();                       // Convert to float
    const operator double();                      // Convert to double

    /******************************
     STREAM OUTPUT
     ******************************/
    friend std::ostream& operator<<(std::ostream& out, const Rational& val);
};

#endif // ifndef RATIONAL
