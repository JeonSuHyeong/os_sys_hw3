#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


long balance = 0;
long maxTry = 999;
long maxThreads = 10;
long t0cnt=0;
long t1cnt=0;

void *deposit(void *args);
void *withdraw(void *args);
long swap(long x);

pthread_mutex_t lck;  

int main(int argc, char *argv[])  {     
        pthread_t dep[100], wit[100];
        int retval;
        int l;
        if (argc <= 1) {
                printf("You should input 3 argument to implement\n");
                printf("Try again. ./a.out {balance} {MaxTry} {MaxThreads}");
                exit(1);
        }
        if (argc >= 2) {
                balance = atoi(argv[1]);
                maxTry = atoi(argv[2]);
        }

        if (argc >3){
                maxThreads = atoi(argv[3]);
                printf("Start Balance -> %ld, MaxTry -> %ld, MaxThreads -> %ld\n", balance, maxTry, maxThreads);
        }
        /**/
        retval = pthread_mutex_init(&lck, NULL);        //initialize the mutex
        /**/

        //create the pthread to access the global variable.
        for (l=0;l<maxThreads;l++){
                pthread_create( &dep[l], NULL, deposit, NULL);
                pthread_create( &wit[l], NULL, withdraw, NULL);
        }
        //waut the pthread to access the global variable.
        for (l=0;l<maxThreads;l++){
                pthread_join( dep[l], NULL);
                pthread_join( wit[l], NULL);
        }
        retval = pthread_mutex_destroy(&lck);   //Free the mutex 

        printf("Final Balance -> %ld\n", balance);

        return 0;
}

void *deposit(void *args)
{
        long i;
        short ran;
        int *dep = (int *)(args);

        for(i = 0; i < maxTry; i++)
        {
                pthread_mutex_lock(&lck);
                balance = balance + i;
                balance = swap(balance);
                pthread_mutex_unlock(&lck);

        }
        t0cnt++;
        pthread_exit(0);
}


void *withdraw(void *args)
{
        long i;
        int *wit = (int *)(args);

        for(i = 0; i < maxTry; i++)
        {
                pthread_mutex_lock(&lck);
                balance -= i;
                balance = swap(balance);
                pthread_mutex_unlock(&lck);

        }
        t1cnt++;
        pthread_exit(0);
}


long swap(long x)
{       
        long tmp;
        int l;
        for(l=0; l<100; l++){
                tmp = x;
                x = tmp;
        }
        return x;
}
