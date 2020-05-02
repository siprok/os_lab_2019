#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) 
{
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  printf("\nSearch min and max in array:\n");
  for(int i = begin; i < end; i++)
  {
    printf("%d ", array[i]);
    if(array[i] < min_max.min)
    {
      min_max.min = array[i];
      continue;
    }
    if(array[i] > min_max.max)
    {
      min_max.max = array[i];
    }
  }
  printf("\n");
  return min_max;
}
