#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
int products = 0;
int pro = 0;
/*信号量*/
sem_t empty_sem, full_sem;   
pthread_cond_t full = PTHREAD_COND_INITIALIZER;        
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;       
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;      

#define BUFFERNUM 10    

struct Buffer_Queue {
	char production[BUFFERNUM];  //产品
	int front, rear;             //指针
	int num;                     //缓冲区数量        
};

//生产者1
void *producer1(void *arg)
{
	struct Buffer_Queue *q;
	q = (struct Buffer_Queue *) arg;
	while(pro<50)
	{
		pthread_mutex_lock(&lock);       
		while (q->num == BUFFERNUM)    //等待
		{            
			pthread_cond_wait(&full, &lock);
		}
		sem_wait(&empty_sem);

		/*生产产品*/
		char c = 'd';  
		q->rear = (q->rear + 1) % BUFFERNUM;                            
		q->production[q->rear] = c;                                  
		q->num++;
		pro++;
		/*打印输出结果*/
		printf("-------------------------------------------------------------\n");
		
		int i;
		printf("缓冲区数据%d个\n",q->num); 
		printf("当前进程：生产者1\n"); 

		sem_post(&full_sem);

		if (q->num == 1) {
			pthread_cond_signal(&empty);    
		}
		pthread_mutex_unlock(&lock); 

		sleep(rand() % 2);            
	}
}

//生产者2
void *producer2(void *arg)
{
	struct Buffer_Queue *q;
	q = (struct Buffer_Queue *) arg;
	while(pro<50)
	{
		pthread_mutex_lock(&lock);       
		while (q->num == BUFFERNUM)    //等待
		{            
			pthread_cond_wait(&full, &lock);
		}
		sem_wait(&empty_sem);

		/*生产产品*/
		char c = 'd'; 
		q->rear = (q->rear + 1) % BUFFERNUM;                            
		q->production[q->rear] = c;                                   
		q->num++;
		pro++;
		/*打印输出结果*/
		printf("-------------------------------------------------------------\n");
		int i;
		printf("缓冲区数据%d个\n",q->num);  
		printf("当前进程：生产者2\n"); 

		sem_post(&full_sem);

		if (q->num == 1) {
			pthread_cond_signal(&empty);    
		}
		pthread_mutex_unlock(&lock); 

		sleep(rand() % 2);            
	}
}

//消费者1
void *consumer1(void *arg)
{
	struct Buffer_Queue *q;
	q = (struct Buffer_Queue *) arg;
	while(products<50)
	{
		pthread_mutex_lock(&lock);        
		while (q->num == 0)           //等待
		{        
			pthread_cond_wait(&empty, &lock);
		}
		sem_wait(&full_sem);

		/*消费产品*/
		q->front = (q->front + 1) % BUFFERNUM;    
		char c = q->production[q->front];         
		q->production[q->front] = ' ';           
		q->num--;
		products++;
		/*打印输出结果*/
		printf("-------------------------------------------------------------\n");
		int i;
		printf("缓冲区数据%d个\n",q->num);  
		printf("当前进程：消费者1\n");  
		printf("消费产品%d个\n",products); 
		
		sem_post(&empty_sem);
		if (q->num == BUFFERNUM - 1) {
			pthread_cond_signal(&full);
		}
		pthread_mutex_unlock(&lock);
		sleep(rand() % 2);
	}
}

//消费者2
void *consumer2(void *arg)
{
	struct Buffer_Queue *q;
	q = (struct Buffer_Queue *) arg;
	while(products<50)
	{
		pthread_mutex_lock(&lock);        
		while (q->num == 0)           //等待
		{        
			pthread_cond_wait(&empty, &lock);
		}
		sem_wait(&full_sem);

		/*消费产品*/
		q->front = (q->front + 1) % BUFFERNUM;    
		char c = q->production[q->front];        
		q->production[q->front] = ' ';          
		q->num--;
		products++;
		/*打印输出结果*/
		printf("-------------------------------------------------------------\n");
		int i;
		printf("缓冲区数据%d个\n",q->num);     
		printf("当前进程：消费者2\n");  
		printf("消费产品%d个\n",products); 
		
		sem_post(&empty_sem);
		if (q->num == BUFFERNUM - 1) {
			pthread_cond_signal(&full);
		}
		pthread_mutex_unlock(&lock);
		sleep(rand() % 2);
	}
}
//消费者3
void *consumer3(void *arg)
{
	struct Buffer_Queue *q;
	q = (struct Buffer_Queue *) arg;
	while(products<50)
	{
		pthread_mutex_lock(&lock);        
		while (q->num == 0)           //等待
		{        
			pthread_cond_wait(&empty, &lock);
		}
		sem_wait(&full_sem);

		q->front = (q->front + 1) % BUFFERNUM;    
		char c = q->production[q->front];       
		q->production[q->front] = ' ';           
		q->num--;
		products++;
		/*打印输出结果*/
		printf("-------------------------------------------------------------\n");
		int i;
		printf("缓冲区数据%d个\n",q->num);    
		printf("当前进程：消费者3\n");  
		printf("消费产品%d个\n",products); 

		sem_post(&empty_sem);
		if (q->num == BUFFERNUM - 1) {
			pthread_cond_signal(&full);
		}
		pthread_mutex_unlock(&lock);
		sleep(rand() % 2);
	}
}
int main(int argc, char *argv[])
{
	/*缓冲区*/
	struct Buffer_Queue *q;
	q = (struct Buffer_Queue *) malloc(sizeof(struct Buffer_Queue));
	//初始化
	q->front = q->rear = BUFFERNUM - 1;
	q->num = 0;

	/*执行进程*/
	pthread_t pid1, cid1, pid2, cid2, cid3;
	//初始化
	sem_init(&empty_sem, 0, BUFFERNUM);   
	sem_init(&full_sem, 0, 0);   

	pthread_create(&pid1, NULL, producer1, (void *) q);
	pthread_create(&cid1, NULL, consumer1, (void *) q);
	pthread_create(&pid2, NULL, producer2, (void *) q);
	pthread_create(&cid2, NULL, consumer2, (void *) q);
	pthread_create(&cid3, NULL, consumer3, (void *) q);
	
	pthread_join(pid1, NULL);
	pthread_join(cid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(cid2, NULL);
	pthread_join(cid3, NULL);
	
	sem_destroy(&empty_sem);
	sem_destroy(&full_sem);

	printf("\n终止生产和消费\n"); 

	return 0;
}
