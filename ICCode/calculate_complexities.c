/* A significant portion of this code is due to Fuller; see http://oeis.org/A005245/a005245.c.txt.  It was modified for the purposes of the paper "On Algorithms to Calculate Integer Complexity" by Cordwell, Epstein, Hemmady, Miller, Palsson, Sharma, Steinerberger, and Yen Nhi Truong Vu.

Our work closely follows "Algorithms for Determining Integer Complexity" by J. Arias de Reyna and J. van de Lune.  Prof. Arias de Reyna and Prof. van de Lune developed complexity analysis for their algorithm to calculate integer complexities in Python.  This code develops that complexity analysis in C, so that we are able to calculate values for higher bases and obtain stronger bounds.  We urge a reader to look at Prof. Arias de Reyna and Prof. van de Lune's paper before looking at our code.
*/

/*  Fuller's original code was, in part, designed to check for counterexamples to certain conjectures.  In Fuller's words: 

"Conjectures to check for counterexamples:
David Wilson [2]: MIN {A005245(x)+A005245(y) : x+y = n} = A005245(n-1)+1
(Counterexample n = 21080618 was suggested by CD_Eater at Russian forum lib.mexmat.ru and verified by Max Alekseyev.  This program confirms it is the smallest.)

David Wilson [3]: A005245(n) = MIN {A005245(n-1)+1} u {A005245(x)+A005245(y) : x*y = n}
(This program finds smallest counterexamples n = 353942783 and n = 516743639)"

We have commented the code that checks for these counterexamples.  It can be uncommented if it is of interest.
*/

/* To run this code, first run the command: "gcc -o calculate_complexities calculate_complexities.c" from terminal.  Then run the command: "./calculate_complexities". */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef unsigned char A005245_value_t;
typedef struct { unsigned size; A005245_value_t *array; } A005245_array_t;
A005245_array_t * A005245_arr;

// powarr stores the bases we want to check--i.e., the powers 2^m3^n where 2^m3^n < 500000000
int powarrlen = 286;
int powarr[286] = {2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 27, 32, 36, 48, 54, 64, 72, 81, 96, 108, 128, 144, 162, 192, 216, 243, 256, 288, 324, 384, 432, 486, 512, 576, 648, 729, 768, 864, 972, 1024, 1152, 1296, 1458, 1536, 1728, 1944, 2048, 2187, 2304, 2592, 2916, 3072, 3456, 3888, 4096, 4374, 4608, 5184, 5832, 6144, 6561, 6912, 7776, 8192, 8748, 9216, 10368, 11664, 12288, 13122, 13824, 15552, 16384, 17496, 18432, 19683, 20736, 23328, 24576, 26244, 27648, 31104, 32768, 34992, 36864, 39366, 41472, 46656, 49152, 52488, 55296, 59049, 62208, 65536, 69984, 73728, 78732, 82944, 93312, 98304, 104976, 110592, 118098, 124416, 131072, 139968, 147456, 157464, 165888, 177147, 186624, 196608, 209952, 221184, 236196, 248832, 262144, 279936, 294912, 314928, 331776, 354294, 373248, 393216, 419904, 442368, 472392, 497664, 524288, 531441, 559872, 589824, 629856, 663552, 708588, 746496, 786432, 839808, 884736, 944784, 995328, 1048576, 1062882, 1119744, 1179648, 1259712, 1327104, 1417176, 1492992, 1572864, 1594323, 1679616,
    1769472, 1889568, 1990656, 2097152, 2125764, 2239488, 2359296, 2519424, 2654208, 2834352, 2985984, 3145728, 3188646, 3359232, 3538944, 3779136, 3981312, 4194304, 4251528, 4478976, 4718592, 4782969, 5038848, 5308416, 5668704, 5971968, 6291456, 6377292, 6718464, 7077888, 7558272, 7962624, 8388608, 8503056, 8957952, 9437184, 9565938, 10077696, 10616832, 11337408, 11943936, 12582912, 12754584, 13436928, 14155776, 14348907, 15116544, 15925248, 16777216, 17006112, 17915904, 18874368, 19131876, 20155392, 21233664, 22674816, 23887872, 25165824, 25509168, 26873856, 28311552, 28697814, 30233088, 31850496, 33554432, 34012224, 35831808, 37748736, 38263752, 40310784, 42467328, 43046721, 45349632, 47775744, 50331648, 51018336, 53747712, 56623104, 57395628, 60466176, 63700992, 67108864, 68024448, 71663616, 75497472, 76527504, 80621568, 84934656, 86093442, 90699264, 95551488, 100663296, 102036672, 107495424, 113246208, 114791256, 120932352, 127401984, 129140163, 134217728, 136048896, 143327232, 150994944, 153055008, 161243136, 169869312, 172186884, 181398528, 191102976, 201326592, 204073344, 214990848, 226492416, 229582512, 241864704, 254803968, 258280326, 268435456, 272097792, 286654464, 301989888, 306110016, 322486272, 339738624, 344373768, 362797056, 382205952, 387420489, 402653184, 408146688, 429981696, 452984832, 459165024, 483729408};
#define MAX_A005245_VALUE 127 /* Keep a safety factor of 2 to avoid overflow */

/* Helper method to find the index of a value in powarr */
int index_of(int val) {
  for (int i = 0; i < powarrlen; i++) {
    if (powarr[i] == val) {
      return i;
    }
  }
  /* if the element is not found */
  return -1;
}

/* Helper method for exponentiation (Thanks to http://stackoverflow.com/questions/213042/how-do-you-do-exponentiation-in-c) */
unsigned long long int power(int base, int exp) {
  int i;
  unsigned long long int result = 1;
  for (i = 0; i < exp; i++)
    result *= base;
  return result;
}

/* Helper method for primality testing (Thanks to http://stackoverflow.com/questions/5281779/c-how-to-test-easily-if-it-is-prime-number) */
int is_prime(int num)
{
  if (num <= 1) return 0;
  if (num % 2 == 0) return 0;
  for(int i = 3; i <= num/2; i+= 2)
    {
      if (num % i == 0)
        return 0;
    }
  return 1;
}

/* The next five methods are from Fuller to calculate the array A005245 of integer complexities */
unsigned A000792(A005245_value_t n)
{
  unsigned result = 1;
  while (n >= 5 || n == 3)
    {
      result *= 3;
      n -= 3;
    }
  return result << (n/2);
}

void A005245_free(A005245_array_t *a)
{
  a->size = 0;
  free(a->array);
  a->array = NULL;
}

int A005245_init(A005245_array_t *a, unsigned size)
{
  unsigned i;

  /* This line was in Fuller's original code but caused problems, so we have removed it */
  // A005245_free(a);  
                                                                          
  a->array = (A005245_value_t*)malloc(size * sizeof(A005245_value_t));
  if (a->array)
    {
      a->size = size;
      a->array[1] = 1;
      for (i = 2; i < size; i++)
        a->array[i] = MAX_A005245_VALUE;
    }
  return (a->array != 0);
}

void A005245_additions_to_n(A005245_array_t *a, unsigned n)
{
  unsigned limit_m, m;
  A005245_value_t target, k;

  if (a->array[n] > a->array[n-1] + 1)
    a->array[n] = a->array[n-1] + 1;

  target = a->array[n-1];
  k = target / 2;
  while (A000792(k) + A000792(target - k) < n)
    k--;
  limit_m = A000792(k);
  /* Already used m=1 earlier, and don't need m=2..5 as they cannot be better than m=1 */
  for (m = 6; m <= limit_m; m++)
    {
      if (a->array[n] > a->array[m] + a->array[n-m])
        {
          // Uncomment to see counterexample information
	  /* printf("Counterexample to [3]: A(%u) + A(%u) = %u < conjecture(%u) = %u\n", m, n-m, (unsigned)(a->array[m] + a->array[n-m]), n, 
	         (unsigned)a->array[n]); */
          a->array[n] = a->array[m] + a->array[n-m];
        }
      // Uncomment to see counterexample information
      /* else if (a->array[n-1]+1 > a->array[m] + a->array[n-m])
	 printf("Counterexample to [2]: A(%u) + A(%u) = %u < A(%u-1)+1 = %u\n", m, n-m, (unsigned)(a->array[m] + a->array[n-m]), n, 
	     (unsigned)a->array[n-1]+1); */
    }
}

void A005245_multiplications_from_n(A005245_array_t *a, unsigned n)
{
  unsigned m, mn;
  for (m = 2, mn = 2*n; (m <= n) && (mn < a->size); m++, mn += n)
    if (a->array[mn] > a->array[m] + a->array[n])
      a->array[mn] = a->array[m] + a->array[n];
}


/* This method frees the array that we use to store the D(b, r) values (Thanks to http://stackoverflow.com/questions/5666214/how-to-free-c-2d-array) */
void COMPL_free(int **a)
{
  for (int i = 0; i < powarrlen; i++)
    {
      int* currRowPtr = a[i];
      free(currRowPtr);
    }
  free(a);
  a = NULL;
}

/* This method initializes the array that we use to store the D(b, r) values (Thanks to https://www.eskimo.com/~scs/cclass/int/sx9b.html) */
int ** COMPL_init()
{
  int ** arr;
  int i, val_at_index;

  /* There are powarrlen rows */
  arr = (int**) calloc(powarrlen, sizeof(int *));
  if (arr == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }

  for (i = 0; i < powarrlen; i++) {
    val_at_index = powarr[i];
    arr[i] = (int*) calloc(val_at_index, sizeof(int));
    if (arr[i] == NULL) {
      fprintf(stderr, "out of memory\n");
      exit(1);
    }
  }

  /* initialize D(2, 0) and D(2, 1) specially */
  arr[0][0] = 2;
  arr[0][1] = 3;

  return arr;

}

/* This function calculates the D(b, r) values, which we store in COMPL_arr */
int calc_count(int b, int r, int calc, A005245_array_t *a, int ** COMPL_arr) {
  int i, calc1, calc2, calc3, pow, base_index;

  base_index = index_of(b);
  /* This is from Equation 12 on page 16 of Prof. Arias de Reyna and Prof. van de Lune's paper */
  if (is_prime(b) == 1) {
    calc = a->array[b] + a->array[r];
  }
  else {
    /* D(b, 0) is the complexity of b */
    if (r == 0) {
      calc = a->array[b];
    }
    /* This optimization is justified by Theorem 2.1 in our paper, "On Algorithms to Calculate Integer Complexity" */
    else if (r == 1) {
      calc = a->array[b] + 1;
    }
    /* This optimization is also justified by Theorem 2.1 in our paper, "On Algorithms to Calculate Integer Complexity" */
    else if ((b % r) == 0) {
      calc = a->array[b] + 1;
    }
    /* Recursive step--access COMPL_arr */
    else if (COMPL_arr[base_index][r] != 0) {
      calc = COMPL_arr[base_index][r];
    }
    /* We follow the computation in Equation 13 on page 16 of Prof. Arias de Reyna and Prof. van de Lune's paper */
    else {
      if (a->array[b] + a->array[r] < calc) {
        calc = a->array[b] + a->array[r];
      }

      // The only divisors of our base are powers of the form 2^i*3^j
      for (i = 0; i < powarrlen; i++) {
        pow = powarr[i];
        if (pow < b) {
          if ((b % pow == 0) && (pow > 1)) {
            calc1 = calc_count(pow, r%pow, calc, a, COMPL_arr);
            calc2 = calc_count(b/pow, floor(r/pow), calc, a, COMPL_arr);
            /* Add the two together */
            calc3 = calc1 + calc2;
            /* Test this against the original calc */
            if (calc > calc3) {
              calc = calc3;
            }
          }
        }
      }
    }
  }

  return calc;
}

/* In this method, we calculate the values of D(b, r) for each base b in powarr and each 0 <= r < b.  These values are stored in COMPL_arr.  For each b in powarr, we calculate four values: dbr_sum, m0, m1, m2.  In dbr_sum, we store the sum from r = 0 to r = b-1 of D(b, r).  This sum is used to bound C_avg (see Proposition 12 in Arias de Reyna and van de Lune's paper.)  In m0, we store the sum of all values of D(b, r) from r = 0 to r = b-1 that are 0 mod 3.  In m1, we store the sum of all values of D(b, r) from r=0 to r = b-1 that are 1 mod 3.  In m2, we store the sum of all values of D(b, r) from r=0 to r = b-1 that are 2 mod 3.  These are used to bound the overall complexity of the algorithm (see Proposition 13 in Arias de Reyna and van de Lune's paper, and pay particular attention to the computation of alpha on page 19.)*/
int main(int argc, char *argv[]) {
  int dbr, i, r, b, n = 483729409;
  unsigned long long int dbr_sum = 0, m0 = 0, m1 = 0, m2 = 0, maxPrecision = power(2, sizeof(unsigned long long int)*8) - 1;
  A005245_array_t A005245;
  int ** COMPL_arr = COMPL_init();
  FILE *f = fopen("calculate_complexities.txt", "w");
 
  // Watch out for overflow
  if (sizeof(int) != 4) {
    printf("Overflows will occur!  Switch to a system where sizeof(int) is 4 bytes\n");
    exit(2);
  }

  // Initialize the array of integer complexities
  if (!A005245_init(&A005245, n)) {
    A005245_free(&A005245);
    printf("Not enough memory\n");
    return 2;
  }
  for (n = 2; n < A005245.size; n++)
    {
      A005245_additions_to_n(&A005245, n);
      A005245_multiplications_from_n(&A005245, n);
    }


  /* Now we'll calculate the D(b, r) values.  We have already initialized D(2, 0) and D(2, 1), so we can start at i = 1 instead of i = 0 */
  for(i = 1; i < powarrlen; i++) {
    b = powarr[i];
    for (r = 0; r < b; r++) {
      // We call calc_count with calc initially equal to the largest possible integer (this is sort of like calling with calc = infty)
      dbr = calc_count(b, r, 2147483647, &A005245, COMPL_arr);

      // We store the D(b, r) values in COMPL_arr
      COMPL_arr[i][r] = dbr;

      // Watch out for overflow
      if (maxPrecision - dbr_sum < dbr) {
        printf("Unsigned int limit is about to overflow; output past this  point is unreliable");
	exit(2);
      }
      // Update the sum of the D(b, r) from r = 0 to b-1.  This is used to bound C_avg.
      dbr_sum += dbr;
      
      // Calculate the values m0, m1, m2 which are used to calculate alpha and bound the overall complexity of the algorithm
      if (dbr%3 == 0) {
	// Watch out for overflow
	if (maxPrecision - m0 < power(3, (dbr/3))) {
	  printf("Unsigned int limit is about to overflow; output past this  point is unreliable");
	  exit(2);
	}
        m0 += power(3, (dbr/3));
      }
      if (dbr%3 == 1) {
	// Watch out for overflow
	if (maxPrecision - m1 < power(3, ((dbr-1)/3))) {
          printf("Unsigned int limit is about to overflow; output past this  point is unreliable");
          exit(2);
	}
        m1 += power(3, ((dbr-1)/3));
      }
      if (dbr%3 == 2) {
	// Watch out for overflow
        if (maxPrecision - m2 < power(3, ((dbr-2)/3))) {
          printf("Unsigned int limit is about to overflow; output past this  point is unreliable");
          exit(2);
        }
        m2 += power(3, ((dbr-2)/3));
      }
    }

    /* Write output to the terminal */
    printf("dbr_sum is: %llu, base is: %d\n", dbr_sum, b);
    printf("Base is: %d, %llu, %llu, %llu\n", b, m0, m1, m2);
    /* Write output to "calculate_complexities.txt" */
    fprintf(f, "dbr_sum is: %llu, base is: %d\n", dbr_sum, b);
    fprintf(f, "Base is: %d, %llu, %llu, %llu\n", b, m0, m1, m2);
    fflush(f);

    // Reset the values for the next base
    dbr_sum = 0;
    m0 = 0;
    m1 = 0;
    m2 = 0;
    fflush(stdout);
  }

  // When all bases are done, close the file and free memory
  fclose(f);
  A005245_free(&A005245);
  COMPL_free(COMPL_arr);

  return 0;
}
