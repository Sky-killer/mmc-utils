#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h> 
void help()
{
  printf("Command Form:./dd_test <Order 1: Sequential 2:Random> <verify_file> <mmc_device> <size(512 bytes)> <off_set(sequential)> <test_times(random)>.\n");
  printf("Example:./dd_tset test.log /dev/mmcblk0 5 10\n");
  return ;
}

int random_t(int argc,char *argv[]){ //<verify_file> <mmc_device> <size(MB)> <test_times>
    long size=0;
    long times=0;
    long off_set=0;
    long i=0;
    char dd_in[200];
    char dd_out[200];
    char diff_cmd[200];
    size =atol(argv[2]);
    times =atol(argv[3]);
    char *device=argv[1];
    char *file=argv[0];
    _Bool good=true;
    struct timeval tv;

    if(!(size&&times)){
        printf("random error!\n");
        return 0;
    }
    printf("================random start===============\n");
    for(;i<times;i++){
        gettimeofday(&tv,NULL);
        srand(tv.tv_usec);
        off_set=rand()%1000+1;
        sprintf(dd_in,"dd if=%s of=%s seek=%ld bs=1M count=%ld",file,device,off_set,size);
        sprintf(dd_out,"dd if=%s of=dd_out skip=%ld bs=1M count=%ld",device,off_set,size);
        sprintf(diff_cmd,"diff %s dd_out",file);
        printf("--times:%ld test size=%ldM,address start at %ld M--\n",i+1,size,off_set);
         system(dd_in);
         system(dd_out);
            //   sleep(30);
         if(system(diff_cmd))
          {
            good=false;
           printf("data error!\n");
          }
         else
          printf("good data!\n");

    }
    printf("================random end===============\n");
    if(good) printf("All good data!\n");
    else printf("Have error data!\n");
}

int sequential(int argc,char *argv[]){ //<verify_file> <mmc_device> <size(MB)> <off_set>
    long size =0;
    long off_set=0;
    char dd_in[200];
    char dd_out[200];
    char diff_cmd[200];
    size =atol(argv[2]);
    off_set =atol(argv[3]);
    char *device=argv[1];
    char *file=argv[0];
    if(!(size&&off_set)){
        printf("sequential error!\n");
        return 0;
    }
    printf("================sequential start===============\n");
    printf("test size=%ldM,address start at%ldM\n",size,off_set);
    sprintf(dd_in,"dd if=%s of=%s seek=%ld bs=1M count=%ld",file,device,off_set,size);
    sprintf(dd_out,"dd if=%s of=dd_out skip=%ld bs=1M count=%ld",device,off_set,size);
    sprintf(diff_cmd,"diff %s dd_out",file);
    
     system(dd_in);
     system(dd_out);
     printf("================sequential end===============\n");
     if(system(diff_cmd))
      {
       printf("data error!");
      }
     else
      printf("good data!\n");
}
int main(int argc,char *argv[])
{
    long times=0;
    int  order=0; 
    long size; //MB
    long off_set=1; //MB
    order=atoi(argv[1]);
    if(argc!=6&&!order)
    {
        printf("command_nums form error!\n");
        help();
        return 0;
    }

    switch(order){
        case 1:sequential(argc-1,&argv[2]);break;
        case 2:random_t(argc-1,&argv[2]);break;
        default:
            printf("command error! %d\n",order);
            help();
            return 0;
    }
}