#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/transfer_options.h"
#include <math.h>


/*
  Compute binomial coefficient for SMALL numbers
*/

long int choose(int n, int k)
{
  long int result;

  result = factorial(n) / (factorial(n - k) * factorial(k));
  return result;
}

long int factorial(int n)
{
  long int result;

  result = n;

  while (n > 1)
    {
      result *= n-1;
      n--;
    }
  return result;
}
