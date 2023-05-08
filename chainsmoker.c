#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <string.h>
#include <semaphore.h> 
#include <sys/syscall.h>
#include <linux/kernel.h>

sem_t more_needed;
sem_t match;
sem_t paper;
sem_t tobacco;


void *agent (){
	int i=0;
	int sm=1;
	int s=0,p=0,m=0;
	while (1){
      	int number = rand() % 3;
	if(i==10){
		printf("\n\nTotal number of time smoker with ciggerete smoked: %d\n",s);
		printf("\nTotal number of time smoker with paper smoked: %d\n",p);
		printf("\nTotal number of time smoker with match smoked: %d\n",m);
		exit(0);
	}
	sleep(1);
	if (number == 0){
		sem_post (&match); 
		sem_post (&paper);
		syscall(333,"Agent has put match and paper on the table\n");
		printf("Smoking %d time\n", sm++);
		s++;
	}
	else if (number == 1){
		sem_post (&match);
		sem_post (&tobacco);
		syscall(333,"Agent has put match and tobacco on the table\n");
		printf("Smoking %d time\n", sm++);
		p++;
	}
	else if (number == 2){
		sem_post (&paper);
		sem_post (&tobacco);
		syscall(333,"Agent has put paper and tobacco on the table\n");
		printf("Smoking %d time\n", sm++);
		m++;
	}
	sem_wait (&more_needed); 
	i++;
    }
}

void *smoker_with_tobacco (){
	while (1){
      		sem_wait (&match);
      		if (sem_trywait (&paper) == 0){
			syscall(333,"match and paper feched");
			syscall(333,"smoker with tobacco is smoking\n");
			sleep(0.5);
          		sem_post (&more_needed);
      		}
      		else 
			sem_post (&match);
    	}
}

void *smoker_with_match (){
	while (1){
      		sem_wait (&paper);
      		if (sem_trywait (&tobacco) == 0){
			syscall(333,"tobacco and paper feched");
			syscall(333,"smoker with match is smoking\n");
			sleep(0.5);
          		sem_post (&more_needed); 
      		}
      		else 
			sem_post (&paper);
    	}
}

void *smoker_with_paper (){
	while (1){
      		sem_wait (&tobacco);
      		if (sem_trywait (&match) == 0) {
			syscall(333,"match and tobacco feched");
			syscall(333,"smoker with paper is smoking\n");
			sleep(0.5);
         		sem_post (&more_needed); 
      		}
      		else 
			sem_post (&tobacco); 
    	}
}

int main(){
	pthread_t th_1, th_2, th_3, th_4;
	sem_init(&more_needed,0,1);
	sem_init(&match,0,0);
	sem_init(&paper,0,0);
	sem_init(&tobacco,0,0);
	pthread_create(&th_1,NULL,agent,NULL); 
	pthread_create(&th_2,NULL,smoker_with_tobacco,NULL);
	pthread_create(&th_3,NULL,smoker_with_paper,NULL); 
        pthread_create(&th_4,NULL,smoker_with_match,NULL);

        pthread_join(th_1,NULL);
        pthread_join(th_2,NULL);
	pthread_join(th_3,NULL);
	pthread_join(th_4,NULL);	
	return 0;
}
