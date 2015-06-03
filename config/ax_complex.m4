AU_ALIAS([AC_COMPLEX_NUMBERS], [AX_COMPLEX])


AC_DEFUN([AX_COMPLEX],[
     AC_CHECK_HEADER([complex.h])
     AC_CACHE_CHECK([for complex number support], [ax_cv_complex],
  [ ax_cv_complex=no
    AC_COMPILE_IFELSE(
        [AC_LANG_SOURCE([
/*
http://stackoverflow.com/questions/6418807/how-to-work-with-complex-numbers-in-c
https://xkcd.com/1185/
*/
#include <stdio.h>      /* Standard Library of Input and Output */
#include <complex.h>    /* Standard Library of Complex Numbers */
int main() {

    double complex z1 = 1.0 + 3.0 * I;
    double complex z2 = 1.0 - 4.0 * I;

    printf("Working with complex numbers:\n\v");

    printf("Starting values: Z1 = %.2f + %.2fi\tZ2 = %.2f %+.2fi\n", creal(z1), cimag(z1), creal(z2), cimag(z2));

    double complex sum = z1 + z2;
    printf("The sum: Z1 + Z2 = %.2f %+.2fi\n", creal(sum), cimag(sum));

    double complex difference = z1 - z2;
    printf("The difference: Z1 - Z2 = %.2f %+.2fi\n", creal(difference), cimag(difference));

    double complex product = z1 * z2;
    printf("The product: Z1 x Z2 = %.2f %+.2fi\n", creal(product), cimag(product));

    double complex quotient = z1 / z2;
    printf("The quotient: Z1 / Z2 = %.2f %+.2fi\n", creal(quotient), cimag(quotient));

    double complex conjugate = conj(z1);
    printf("The conjugate of Z1 = %.2f %+.2fi\n", creal(conjugate), cimag(conjugate));

    return 0;
}
])],
[ax_cv_complex=yes],
[AC_MSG_FAILURE([compiling program with C99 complex number feature])]
)])])
