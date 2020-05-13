#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>

int result = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

struct MultiArgs {
  int begin;
  int end;
};

void multiply(const struct MultiArgs *args)
{
  int i;
  int prod = 1;
  for(i = (*args).begin; i < (*args).end; i++)
	prod *= i;
  pthread_mutex_lock(&mut);
  result *= prod ;
  pthread_mutex_unlock(&mut);

}


int main(int argc, char **argv) {


  int threads_num = 0; 
  int k = 0; // factoral argument
  int mod = 0; // mod base
  int j;   

  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"threads_num", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
             k = atoi(optarg);
             if(k < 1)
             {
                printf("k is a positive number\n");
                return 1;
             }
            break;
          case 1:
            threads_num = atoi(optarg);
            if (threads_num < 1)
            {
                printf("threads_num is a positive number\n");
                return 1;
            }
            break;
	  case 2:
            mod = atoi(optarg);
            if (mod < 1)
            {
                printf("mod is a positive number\n");
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

  if (threads_num == 0 || k == 0 || mod == 0) 
  {
    printf("Usage: %s --k \"num\" --threads_num \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }
  pthread_t threads[threads_num];
 
  int array_step = k / threads_num; 
  int last_step = k % threads_num;
  if(last_step == 0)
  { 
     last_step = array_step;
  }
  else 
  {
     array_step ++;
     last_step = k - array_step * (threads_num - 1);
  }
  int local_step[threads_num];
  struct MultiArgs args[threads_num];
  
  for (j = 0; j < threads_num; j++) 
  {
    local_step[j]  = j < threads_num - 1 ? array_step : last_step;
    args[j].begin = j * array_step + 1;
    args[j].end =  j * array_step + local_step[j] + 1;
    if (pthread_create(&threads[j], NULL, (void *)multiply, (void *)&args[j]))
    {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  for (j = 0; j < threads_num; j++) {
    pthread_join(threads[j], NULL);
  }
 
  printf("result: %d\n", result % mod);
  
  return -1;
}


