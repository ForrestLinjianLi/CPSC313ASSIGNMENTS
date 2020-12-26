#include <limits.h>

long maxsa(long *array, long size, long **sa_start, long **sa_end) {

  if (size == 1) {
    *sa_start = *sa_end = array;
    return *array;
  }

  long half = size >> 1;
  long *mid = array + half, *end = array + size - 1;

  long *ptr;
  long sum, leftsum = LONG_MIN, rightsum = LONG_MIN;

  for (ptr = mid - 1, sum = 0; ptr >= array; ptr--) {
    sum += *ptr;
    if (sum > leftsum) {
      leftsum = sum;
      *sa_start = ptr;
    }
  }
  
  for (ptr = mid, sum = 0; ptr <= end; ptr++) {
    sum += *ptr;
    if (sum > rightsum) {
      rightsum = sum;
      *sa_end = ptr;
    }
  }

  long bestsum = leftsum + rightsum;
  long *sub_start, *sub_end;
  
  sum = maxsa(array, half, &sub_start, &sub_end);
  if (sum > bestsum) {
    bestsum = sum;
    *sa_start = sub_start;
    *sa_end = sub_end;
  }

  sum = maxsa(mid, size - half, &sub_start, &sub_end);
  if (sum > bestsum) {
    bestsum = sum;
    *sa_start = sub_start;
    *sa_end = sub_end;
  }

  return bestsum;
}
