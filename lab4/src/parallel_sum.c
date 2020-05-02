#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "utils.h"
#include <getopt.h>
#include <pthread.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args)
{
  int sum = 0;
  // TODO: your code here 
  for(int i = (*args).begin; i < (*args).end; i++)
	sum += (*args).array[i];
  return sum;
}

void *ThreadSum(void *args) 
{
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0; 
  
  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"threads_num", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"seed", required_argument, 0, 0},
                                      {0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            threads_num = atoi(optarg);
             if(threads_num < 1)
             {
                printf("threads_num is a positive number\n");
                return 1;
             }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size < 1)
            {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
	  case 2:
            seed = atoi(optarg);
            if (seed < 1)
            {
                printf("seed is a positive number\n");
                return 1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc)
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (threads_num == 0 || array_size == 0 || seed  == 0) 
  {
    printf("Usage: %s --threads_num\"num\" --array_size \"num\" --seed\"num\" \n",
           argv[0]);
    return 1;
  }

  pthread_t threads[threads_num];
  
  /*
   * TODO:
   * your code here
   * Generate array here
   */
  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  uint32_t array_step = array_size / threads_num;
  uint32_t last_step = array_size % threads_num;
  if(last_step == 0) last_step = array_step;
  else array_step++;
  int local_step[threads_num];
  struct SumArgs args[threads_num];

  struct timeval time_start, time_end;
  gettimeofday(&time_start, NULL);
  for (uint32_t i = 0; i < threads_num; i++) 
  {
    local_step[i]  = i < threads_num - 1 ? array_step : last_step;
    args[i].array = array;
    args[i].begin = i * array_step;
    args[i].end =  i * array_step + local_step;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i]))
    {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }
  gettimeofday(&time_end, NULL);
  free(array);
  printf("Total: %d\n", total_sum);
  printf("Time : %lf sec\n", time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0);
  return 0;
}
