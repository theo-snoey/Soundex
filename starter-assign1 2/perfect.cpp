/*
 * OVERVIEW HEADER COMMENT:
 * Code Author: Theo Snoey
 * Hey section leader,
 * What's up!
 * I am from Berkeley, CA.
 * Something I learned doing this warmup was how mathematical isights into
 * perceptively simple computations can make huge differences in terms of efficiency of programs.
 * I thought that the Marsenne primes method was particularly cool!
 */
#include "console.h"
#include <iostream>
#include "SimpleTest.h"
#include "perfect.h"
using namespace std;


/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}



/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* The smarterSum function takes one argument n and returns a sum of the
 *  divisors of that numbers using a square root finding method that improves
 *  computational efficiency
 */
long smarterSum(long n) {
    long total = 0;
    if (n != sqrt(n)){
        for (long divisor = 1; divisor <= sqrt(n); divisor++) {
            if (n % divisor == 0) {
                total += divisor;

            }
            if (n % divisor == 0 && divisor != 1 && divisor != sqrt(n)){
                total += n/divisor;
            }
        }
    }
    return total;
}


/* The is perfect smarter function takes an argument n and returns a boolean
 * true of false indicating whether or not n is perfect by calling the smarterSum
 * function.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* The findPerfectsSmarter function takes an argument stop and performs a search
 * of all the perfect numbers from the range 1 to stop using the smarter square root
 * searching algorithm from SmarterSum
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}


/* The findNthPerfectEuclid function takes an argument n and calculates the nth perfect number using
 * Marsenne primes.
 */
long findNthPerfectEuclid(long n) {
    long k = 1;
    long count = 0;
    long m = 0;

    while (true){
        m = pow(2,k) - 1;
        if (smarterSum(m) == 1){
            count ++;
        }
        if (count == n){
            return (m * (pow(2, (k-1))));
            break;
        }
        k++;
    }
    return 0;
}


/* * * * * * Test Cases * * * * * */

// Student Tests

STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {
    int smallest = 38750, largest = 310000;
    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}


STUDENT_TEST("Calls isPerfect with varying negative numbers to examine what boolean it returns"){
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-23));
    EXPECT(!isPerfect(-500));
}


STUDENT_TEST("Check if output of smarterSum matches expected values"){
    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
}

STUDENT_TEST("Check if output of smarterSum matches expected values"){
    EXPECT_EQUAL(divisorSum(1), smarterSum(1));
}

STUDENT_TEST("Check if output of smarterSum matches expected values"){
    EXPECT_EQUAL(divisorSum(-36.8), smarterSum(-36.8));
}


STUDENT_TEST("run time test on findPerfectSmarter"){
    TIME_OPERATION(1500000,findPerfectsSmarter(1500000));
    TIME_OPERATION(3000000,findPerfectsSmarter(3000000));
    TIME_OPERATION(6000000,findPerfectsSmarter(6000000));
    TIME_OPERATION(12000000,findPerfectsSmarter(12000000));
}

STUDENT_TEST("Testing my perfectEuclid function"){
    EXPECT(isPerfect(findNthPerfectEuclid(1)));
    EXPECT(isPerfect(findNthPerfectEuclid(3)));
    EXPECT(isPerfect(findNthPerfectEuclid(5)));
}

STUDENT_TEST("Testing if perfectEuclid function out puts the correct perfect number"){
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);

}

// Provided Tests

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}
