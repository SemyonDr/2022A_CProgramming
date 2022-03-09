#include <stdio.h>
#include <math.h>
#include "complex.h"

/* Prints complex number.
   Arguments:
    c   - Number to print.  */
void PrintComplex(complex c) {
    printf("%.2f+(%.2f)i\n", c.Re, c.Im);
}

/* Adds two complex numbers and returns the result.
   Arguments:
    c_a - First comlex number.
    c_b - Second complex number.
   Returns:
    Sum value.*/
complex AddComplex(complex a, complex b) {
    complex sum;
    sum.Re = a.Re+b.Re;
    sum.Im = a.Im+b.Im;

    return sum;
}

/* Multiplies complex number a by complex number b
   and returns the result.
   Arguments:
    a   - First complex number.
    b   - Second complex number.
   Returns:
    Multiplication result. */
complex MultiplyComplex(complex a, complex b) {
    complex mul;
    mul.Re = a.Re*b.Re - a.Im*b.Im;
    mul.Im = a.Re*b.Im+a.Im*b.Re;

    return mul;
}

/* Calculates absolute value of complex number c
   and returns complex number that equals to it. 
   Arguments:
    c   - Complex number.
   Returns:
    Complex number equal to absolute value of c. */
complex AbsoluteComplex(complex c) {
    complex abs;
    abs.Re = sqrt(c.Re*c.Re+c.Im*c.Im);
    abs.Im = 0.0;

    return abs;
}