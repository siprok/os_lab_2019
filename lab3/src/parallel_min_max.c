#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
             if(seed < 1)
	     {
                printf("seed is a positive number\n");
                return 1;
             }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0)
            {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0)
            {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  
  int array_step = array_size / pnum;
  int last_step = array_size % pnum;
  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        // parallel somehow
	int local_step = i < pnum - 1 ? array_step : last_step;
        struct MinMax *min_max = malloc(sizeof(struct MinMax)); 
	min_max[0] = GetMinMax(array, i * array_step, i * array_size + local_step);
	printf("Find %d and %d\n", min_max[0].min, min_max[0].max);
        if (with_files) 
 	{
	  char path[10];
	  sprintf(path, "%d.bin",i);
	  FILE *minmax_file;
	  if((minmax_file = fopen(path, "wb")) == NULL)
 	  {
   	    printf("ERROR openning for adding to  minmax file");
 	  }
	  
	  if(fwrite(min_max,sizeof(struct MinMax),1, minmax_file) != 1)
	  {
	    printf("ERROR write minmax struct to file");
	    return 1;
	  }
	  fclose(minmax_file);
 	}
	 else
	 {
          // use pipe here
        }
	free(min_max);
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  int status;
  while (active_child_processes > 0) {
    // your code here
    wait(&status);
    active_child_processes -= 1;
  }

  struct MinMax *min_max = malloc(sizeof(struct MinMax));
  int min = INT_MAX;
  int max = INT_MIN;
  char filepath[10];
  FILE* minmax_file;
  size_t get_el;
  for (int i = 0; i < pnum; i++)
 {
    if (with_files)
    {
      // read from files
      sprintf(filepath, "%d.bin",i);
      if((minmax_file = fopen(filepath, "rb")) == NULL)
          {
            printf("ERROR openning for reading to  minmax file");
          }
      
      if(fread(min_max, sizeof(struct MinMax), 1, minmax_file) != 1)
     {
	 printf("ERROR reading from file");
	 return 1;
     }	
      fclose(minmax_file);
      remove(filepath);
    }
    else
    {
      // read from pipes
    }
    if (min > min_max[0].min) min = min_max[0].min;
    if (max < min_max[0].max) max = min_max[0].max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  free(min_max);
  printf("Min: %d\n", min);
  printf("Max: %d\n", max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
