#ifndef COMPLEX_H
    #define COMPLEX_H

/* Datatype for complex number variable
   Stores separately real and imaginary parts. */
typedef struct complex {
    double Re;
    double Im;
} complex;

/* Prints complex number.
   Arguments:
    c   - Number to print.  */
void PrintComplex(complex c);

/* Adds two complex numbers and returns the result.
   Arguments:
    a - First comlex number.
    b - Second complex number
   Returns:
    Sum value.*/
complex AddComplex(complex a, complex b);

/* Multiplies complex number a by complex number b
   and returns the result.
   Arguments:
    a   - First complex number.
    b   - Second complex number.
   Returns:
    Multiplication result. */
complex MultiplyComplex(complex a, complex b);

/* Calculates absolute value of complex number c
   and returns complex number that equals to it. 
   Arguments:
    c   - Complex number.
   Returns:
    Complex number equal to absolute value of c. */
complex AbsoluteComplex(complex c);

#endif /* COMPLEX_H */