#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
//#include <openssl/rand.h>
//#include <time.h>

#define N 710
//#define NUM_THREADS 16

unsigned long input_matrix[N][N];
	unsigned long long threadless[N][N], threaded[N][N], squared_threads[N][N];

struct index {
   int i; /* row */
   int j; /* column */
};

//The function used by threaded[N][N]
static inline void *thread_method(void *param) {
   struct index *data = param; // the structure that holds our data
   int sum = 0; //the counter and sum

   //Row multiplied by column
   for(int n = 0; n< N; n++){
      sum += input_matrix[data->i][n] * input_matrix[n][data->j];
   }
   //assign the sum to its coordinate
   squared_threads[data->i][data->j] = sum;

   //Exit the thread
   pthread_exit(0);
}

//The function used by squared_threads[N][N]
static inline void *squared_thread_method(void *param) {
   struct index *data = param; // the structure that holds our data
   int sum = 0; //the counter and sum

   //Row multiplied by column
   for(int n = 0; n< N; n++){
      sum += input_matrix[data->i][n] * input_matrix[n][data->j];
   }
   //assign the sum to its coordinate
   squared_threads[data->i][data->j] = sum;

   //Exit the thread
   pthread_exit(0);
}

unsigned long get_random()
{
	/*
	unsigned char *buf;
	int num;
	 int RAND_bytes(buf, num);
	return &buf;
	*/
	return rand() % 100;
}
int main()
{
	//fill both matrices
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			input_matrix[i][j] = get_random();
			threadless[i][j] = 0;
			//printf("%lu ", a[i][j]);
		}
		//printf("\n");
	}

	//mulitply matrices w/o threads
	printf("%s\n","threadless starting");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				threadless[i][j] += input_matrix[i][k] * input_matrix[k][j];
			}
		}
	}
	printf("%s\n","threadless completed");

	//multiply matrices using N^2 threads
	int count = 0;
	printf("%s\n","threaded starting");
	for(int i = 0; i < N; i++) {
   		for(int j = 0; j < N; j++) {
   		//Assign a row and column for each thread
		struct index *data = (struct index *) malloc(sizeof(struct index));
		data->i = i;
		data->j = j;
		/* Now create the thread passing it data as a parameter */
		pthread_t tid;       //Thread ID
		pthread_attr_t attr; //Set of thread attributes
		//Get the default attributes
		pthread_attr_init(&attr);
		//Create the thread
		pthread_create(&tid, &attr, squared_thread_method, data);
		//Make sure the parent waits for all thread to complete
		pthread_join(tid, NULL);
		count++;
		}
	}
	printf("%s\n","threaded completed");




/*
	//print result
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%llu ", c[i][j]);
		}
		printf("\n");
	}
*/

	return 0;
}