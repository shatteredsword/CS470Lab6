#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
//#include <openssl/rand.h>
//#include <time.h>

#define N 710
//#define NUM_THREADS 16

unsigned long input_matrix[N][N];
	unsigned long long threadless[N][N], threaded[N][N], squared_threads[N][N];

struct index
{
	int i;
	int j;
};

//The function used by threaded[N][N]
static inline void *threaded_method(void *param)
{
	struct index *entry = param; // the structure that holds our data
	int sum = 0; //the counter and sum

	//Row multiplied by column
	for(int n = 0; n < N; n++)
	{
		sum += input_matrix[entry -> i][n] * input_matrix[n][entry -> j];
	}
	//assign the sum to its coordinate
	threaded[entry -> i][entry -> j] = sum;

	//Exit the thread
	pthread_exit(0);
}

//The function used by squared_threads[N][N]
static inline void *squared_thread_method(void *param)
{
	struct index *entry = param; // the structure that holds our data
	int sum = 0; //the counter and sum

	//Row multiplied by column
	for(int n = 0; n < N; n++)
	{
		sum += input_matrix[entry -> i][n] * input_matrix[n][entry -> j];
	}
	//assign the sum to its coordinate
	squared_threads[entry -> i][entry -> j] = sum;

	//Exit the thread
	pthread_exit(0);
}

unsigned long get_random()
{
	return rand() % 100;
}

int main()
{
	struct timespec begin, end;
	double elapsed;

	int i, j, k; //incrementers declared here to reduce time of each inner for loop
	//fill both matrices
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			input_matrix[i][j] = get_random();
			threadless[i][j] = 0;
			//printf("%lu ", a[i][j]);
		}
		//printf("\n");
	}

	//mulitply matrices w/o threads
	printf("%s\n", "threadless starting");
	clock_gettime(CLOCK_MONOTONIC, &begin);
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				threadless[i][j] += input_matrix[i][k] * input_matrix[k][j];
			}
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	elapsed = end.tv_sec - begin.tv_sec;
	elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;

	printf("%s%f%s\n", "threadless completed in ", elapsed, " seconds");

	//multiply matrices using N threads
	printf("%s\n", "threaded starting");
	clock_gettime(CLOCK_MONOTONIC, &begin);
	for(i = 0; i < N; i++)
	{
   		for(j = 0; j < N; j++)
   		{
	   		//Assign a row and column for each thread
			struct index *entry = (struct index*) malloc(sizeof(struct index));
			entry -> i = i;
			entry -> j = j;
			/* Now create the thread passing it data as a parameter */
			pthread_t thread_id;       //Thread ID
			//Create the thread
			pthread_create(&thread_id, NULL, threaded_method, entry);
			//Make sure the parent waits for all thread to complete
			pthread_join(thread_id, NULL);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	elapsed = end.tv_sec - begin.tv_sec;
	elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
	printf("%s%f%s\n", "threaded completed in ", elapsed, " seconds");

	//multiply matrices using N^2 threads
	printf("%s\n", "squared_threaded starting");
	clock_gettime(CLOCK_MONOTONIC, &begin);
	for(i = 0; i < N; i++)
	{
   		for(j = 0; j < N; j++)
   		{
	   		//Assign a row and column for each thread
			struct index *entry = (struct index*) malloc(sizeof(struct index));
			entry -> i = i;
			entry -> j = j;
			/* Now create the thread passing it data as a parameter */
			pthread_t thread_id;       //Thread ID
			//Create the thread
			pthread_create(&thread_id, NULL, squared_thread_method, entry);
			//Make sure the parent waits for all thread to complete
			pthread_join(thread_id, NULL);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	elapsed = end.tv_sec - begin.tv_sec;
	elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
	printf("%s%f%s\n", "squared_threaded completed in ", elapsed, " seconds");

/*
	//print result
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%llu ", c[i][j]);
		}
		printf("\n");
	}
*/
	return 0;
}