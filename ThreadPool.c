

//c中没有模板，使用宏代替
#define LL_ADD(item, list) do{ \
    item->prev = NULL; \
    item->next = list; \
    if(list != NULL) list->prev = item; \
    list = item; \
}while(0)

#define LL_REMOVE(item, list) do{ \
    if(item->prev !=NULL) item->prev->next = item->next; \
    if(item->next !=NULL) item->next->prev = item->prev; \
    if(list == item) list = item->next; \
    item->prev = item->next = NULL; \
}while(0)

struct NWORKER{
    pthread_t thread;
    struct NMANAGER *pool;
    int terminate; //判断是否停止工作
    struct NWORKER *prev;
    struct NWORKER *next;
};

struct NJOB{
    void (*func)(struct NJOB *job);
    void *user_data;
    struct NJOB *prev;
    struct NJOB *next;
};

struct NMANAGER{
    struct NWORKER *workers;
    struct NJOB *jobs;
    struct NMANAGER *pool;
    pthread_cond_t jobs_cond;//条件等待 队列为空的状态
    pthread_mutex_t jobs_mutex;//队列加锁
};
typedef struct NMANAGER nThreadPool;

static void *nThreadCallback(void *arg){ //线程具体的工作
    struct NWORKER *worker = (struct NWORKER*)arg;
    while(1){
        pthread_mutex_lock(&worker->pool->jobs_mutex);
        while(work->pool->jobs == NULL){
            if(worker->terminate) break;
            pthread_cond_wait(worker->pool->jobs_cond,worker->pool->jobs_mutex);
        }
        if(worker->terminate){
            pthread_mutex_unlock(&worker->pool->jobs_mutex);
            break;
        }
        struct NJOB *job = worker->pool->jobs;
        LL_REMOVE(job,worker->pool->jobs);
        pthread_mutex_unlock(&worker->pool->jobs_mutex);
        job->func(job->user_data);
    }
    free(worker):
    pthread_exit(NULL);

}

//Thread pool Create
int nThreadPoolCreate(nThreadPool *pool,int numWorkers){
    if(numWorkers <1) numWorkers = 1;
    if(pool = NULL) return -1;
    memset(pool,0,sizeof(nThreadPool));
    pthread_cond_t blank_cond = PTHERAD_COND_INITIALIZER;
    memcpy(&pool->jobs_cond, &blank_cond, sizeof(pthread_cond_t));
    pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->jobs_mutex,&blank_mutex,sizeof(pthread_mutex_t));

    int i = 0;
    for(i=0;i<numWorkers;i++){
        struct NWORKER *worker = (struct NWORKER*)malloc(sizeof(struct NWORKER));
        if(worker == NULL){
            perror("malloc");
            return -2;
        }
        memset(worker,0,sizeof(struct NWORKER));
        worker->pool = pool;
        int ret = pthread_create(worker->thread,NULL,nThreadCallback,worker);
        if(ret){
            perror("pthread_create");
            free(worker);
            return -3;
        }
        LL_ADD(worker, pool->workers);
    }

}

void nThreadPoolPush(nThreadPool *pool,struct NJOB *job){
    pthread_mutex_lock(&pool->jobs_mutex);
    LL_ADD(job,pool->jobs);
    pthread_cond_signal(&pool->jobs_cond);
    pthread_mutex_unlock(&pool->jobs_mutex);
}

void nThreadPoolDestory(nThreadPool *pool){
    struct NWORKER *worker = NULL;
    for(worker = pool->workers; worker != NULL; worker = worker->next){
        worker->terminate = 1;
    }
    pthread_mutex_lock(&pool->jobs_mutex);
    pthread_cond_broadcast(&pool->jobs_cond);
    pthread_mutex_unlock(&pool->jobs_mutex);
}
