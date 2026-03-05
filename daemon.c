#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>
#include <time.h>

void daemonize();


int main(){
    // This suppose to ignore the SIGHUP
    signal(SIGHUP, SIG_IGN);
    daemonize();
    //here we are deamon.
    while(true){
        int time_fd = open("/home/user/Documents/time.txt",O_CREAT | O_WRONLY ,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        char time_string[100] = {0};
        time_t now;
        time(&now);
        struct tm *mytime = localtime(&now);
        strftime(time_string, sizeof(time_string), "%X", mytime);
        write(time_fd,time_string,sizeof(time_string));
        close(time_fd);
    }
    return 0;

}

void daemonize(){

    // STAGE 1 - split from parent
    int pid = fork(); 

    if (pid == 0){ // its the child
    // STAGE 2 - detouch from termional - 2
        pid = setsid();
        if(pid == -1){
            printf("error accured");
            return;
        }
    // STAGE 3 - the second split 
        pid = fork();
        if (pid == 0){ // its the child
           
    // STAGE 4 - files permissions      
            umask(0);
    // STAGE 5 - move the secure path
            
            chdir("/");
            
    // STAGE 6 AND LAST - Complete sealing
            int max_fds = sysconf(_SC_OPEN_MAX);
            //skip 0-2.
            for(int i = 3; i <= max_fds; i++){
                close(i);
            }
            int null_location = open("/dev/null", O_RDWR); // empty location
            // now take care of the printing.
            for(int i = 0; i <= 2; i++){
                close(i);
                dup2(null_location,i);
            }
            close(null_location);
            return;
        }
        if(pid == -1){
            printf("error accured");
            return;
        }else{    
            exit(0);
        }
    }else if(pid == -1){
        printf("error accured2");
        return;
    }else{
        sleep(6);
        exit(0);
    }
}
