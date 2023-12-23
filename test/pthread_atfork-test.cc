#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
 
#define ERROR(err, msg) do { errno = err; perror(msg); exit(-1); } while(0)
 
int count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 
void prepare() {
        int err;
        printf("prepare: pthread_mutex_lock ...\n");
        err = pthread_mutex_lock(&lock);
        if (err != 0) ERROR(err, "prepare: pthread_mutex_lock failed");
        printf("prepare: lock start...\n");
}
 
void parent() {
        int err;
        printf("parent: pthread_mutex_unlock ...\n");
        err = pthread_mutex_unlock(&lock);
        if (err != 0) ERROR(err, "parent: pthread_mutex_unlock");
}
 
void child() {
        int err;
        printf("child: pthread_mutex_unlock ...\n");
        err = pthread_mutex_unlock(&lock);
        if (err != 0) ERROR(err, "child: pthread_mutex_unlock");
}
 
void* thread_proc(void* arg) {
        while(1) {
                pthread_mutex_lock(&lock);
                count++;
                printf("parent thread:  count:%d\n",count);
                sleep(10);
                pthread_mutex_unlock(&lock);
                sleep(1);
        }
        return NULL;
}
 
int main(int argc,char * argv[])
{
        int err;
        pid_t pid;
        pthread_t tid;
        pthread_create(&tid, NULL, thread_proc, NULL);
        err = pthread_atfork(prepare, parent, child);
        if (err != 0) ERROR(err, "atfork");
 
        sleep(1);
        printf("parent is about to fork ...\n");
        pid = fork();
        if (pid < 0) ERROR(errno, "fork");
        else if (pid == 0) {
                // child process
 
                int status;
                printf("child running\n");
                while(1) {
                        pthread_mutex_lock(&lock);
                        count ++;
                        printf("child: count:%d\n",count);
                        sleep(2);
                        pthread_mutex_unlock(&lock);
                        sleep(1);
                }
                exit(0);
        }
 
        pthread_join(tid, NULL);
 
        return 0;
}