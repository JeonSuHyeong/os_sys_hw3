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
                printf("Usage:");
                exit(1);
        }
        if (argc >= 2) {
                balance = atoi(argv[1]);
                maxTry = atoi(argv[2]);
        }
        if (argc >3){
                maxThreads = atoi(argv[3]);
                printf("Start Balance -> %ld, MaxTry -> %ld, MaxThreads -> %d\n", balance, maxTry, maxThreads);
        }
        /**/
        retval = pthread_mutex_init(&lck, NULL);
        /**/
        for (l=0;l<maxThreads;l++){
                pthread_create( &dep[l], NULL, deposit, NULL);
                pthread_create( &wit[l], NULL, withdraw, NULL);
        }
        
        for (l=0;l<maxThreads;l++){
                pthread_join( &dep[l], NULL);
                pthread_join( &wit[l], NULL);
        }
        
        retval = pthread_mutxex_destroy(&lck);

        printf("Final Balance -> d%ld\n", balance);

        return 0;
}

void *deposit(void *args)
{
        int i;
        int *dep = (int *)(args);
        balance += dep[i];
        for(i = 0; i < maxTry; ++i)
        {
                pthread_mutex_lock(&lck);
                balance =balance + i;
                balance = swap(balance);
                pthread_mutex_unlock(&lck);

        }
        pthread_exit(0);
}


void *withdraw(void *args)
{
        int i;
        int *wit = (int *)(args);
        balance -= wit[i];
        for(i = 0; i < maxTry; i++)
        {
                pthread_mutex_lock(&lck);
                balance = swap(balance);
                pthread_mutex_unlock(&lck);

        }
        pthread_exit(0);
}


long swap(long x)
{       
        int temp_t;
        temp_t = temp;
        temp = balance;
        balance = temp;
}

void initialize()
{
        
}
