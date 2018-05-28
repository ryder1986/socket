#include <iostream>
#include <unistd.h>
#include <syslog.h>
using namespace std;

int main()
{
    //    if (daemon(1, 1) < 0) {
    //        syslog(LOG_FTP | LOG_ERR, "failed to become a daemon");
    //      //  exit(1);
    //    }
    //    while(1)
    //        ;

    int pid= fork();
    if(pid==0){
        printf("child");fflush(stdout);
    }
    else if(pid>0){
        printf("father");fflush(stdout);
        while(1)
            ;
    }
    return 0;
}

