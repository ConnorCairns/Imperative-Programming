#include <stdio.h>
#include <assert.h>

int mysquare ( int n) {
int s = 0;
int i = 0;
int k = 0; // assertion variable counts number of times in the loop
assert (n >= 0); // Pre - condition to catch invalid input
assert (s == k*n && i == k ); // Invariant to catch mistaken variable initialisation
// e. g. i != 0 or s != 0
while (i < n) {
s = s + n ;
i = i + 1;
k = k + 1;
assert (( s == k*n ) && (i == k )); // Invariant to catch errors in the loop computation
}
assert (k == n ); // Post - condition to catch a mistaken loop exit
assert (s == k*n && i == k ); // Invariant to catch errors in the loop computation
assert (s == n * n ); // Check desired overall post - condition
return s;
}
int main () {
int n = 4;
int square = 0;
printf (" n = % d\n" , n );
square = mysquare (n );
printf (" n ^2 = %d\n", square );
return 0;
}
