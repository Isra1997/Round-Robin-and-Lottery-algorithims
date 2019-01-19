#include<stdio.h>
#include <stdlib.h>
#include <time.h>

struct  process{
    int P_ID;
    int arrival_time;
    int CPU_burst;
    int  remaining_time;
    int start_time;
    int end_time;
    int number_of_tickets;
    int Wait;
};

int main(){
    FILE *fptr;//var for input filename
    FILE *out;//output file
    char filename[100], c;
    int  quantum , P_ID , arrival_time , CPU_burst , start_time  , number_of_tickets , total_number_of_tickets ,  num_procsses  , Turnaround ,WaitingTime;//global var's
    char buff [100];//var to read quantum time from file
    struct process Ready_Queue [50];//Ready_Queue of the process in the file

    clock_t start_t, end_t, total_t;//time variables



    printf("Please enter the file directory you want open \n");
    scanf("%s", filename);

    // Open  Input file
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    //open output file
    out=fopen("/Users/israragheb/Desktop/OS_MP2/OS_MP2/out.txt","w+");

    //intialize the quantum time
    fgets(buff, 255, (FILE*)fptr);
    sscanf(buff, "%d", &quantum);

    //intialize the total number of tickets
    fgets(buff, 255, (FILE*)fptr);
    sscanf(buff, "%d", &total_number_of_tickets);


    //add the processes to the Ready_Queue
    for (int i = 0; fscanf(fptr , "%d,%d,%d,%d" , &P_ID , & arrival_time , &CPU_burst ,  &number_of_tickets)>0; i++) {
       Ready_Queue[i].P_ID=P_ID;
       Ready_Queue[i].arrival_time=arrival_time;
      Ready_Queue[i].CPU_burst=CPU_burst;
       Ready_Queue[i].remaining_time=CPU_burst;
       Ready_Queue[i].number_of_tickets=number_of_tickets;
       num_procsses++;
    }




//rounding robin
int flag=0;
int ctime = 0;
int end_time =0;
int num_procsses2=num_procsses;
 for (int i = 0; i < num_procsses2; i++) {
      start_time=clock();
      int ms = 0;
if (Ready_Queue[i].remaining_time >= quantum){

  do {
  clock_t diff = clock() - start_time;
  ms = diff * 1000 / CLOCKS_PER_SEC;
  }
  while (ms < quantum);
  Ready_Queue[i].remaining_time -= quantum;
  ctime+=quantum;
  if (Ready_Queue[i].remaining_time==0) {
    flag = 1;
  }
} else{
  do {
  clock_t diff = clock() - start_time;
  ms = diff * 1000 / CLOCKS_PER_SEC;
  }
  while (ms < Ready_Queue[i].remaining_time);
  ctime+=Ready_Queue[i].remaining_time;
  Ready_Queue[i].remaining_time = 0;
  if (Ready_Queue[i].remaining_time==0) {
    flag = 1;
  }
}

      if (flag==1) {
        int Turnaround=0;
        end_time = ctime;
        Ready_Queue[i].end_time=end_time;
        // printf("process:%i\n", Ready_Queue[i].P_ID);
        //Time 699: P7 Turn around: 699 Waiting time: 600
        //fprintf(out, "process:%i\n",  Ready_Queue[i].P_ID);
        Turnaround=end_time;
        WaitingTime=Turnaround-Ready_Queue[i].CPU_burst;
        Ready_Queue[i].Wait=WaitingTime;
        fprintf(out, "%s", "Time ");
        fprintf(out, "%ld", end_time);
        fprintf(out, "%s", ": P");
        fprintf(out, "%i",Ready_Queue[i].P_ID );
        fprintf(out, "%s", " " );
        fprintf(out, "%s", "Turn around: ");
        fprintf(out, "%ld", end_time);
        fprintf(out, "%s", "Waiting time: ");
        fprintf(out, "%ld\n",WaitingTime );
        flag=0;
      } else{
      Ready_Queue[num_procsses2].P_ID=Ready_Queue[i].P_ID;
      Ready_Queue[num_procsses2].arrival_time=Ready_Queue[i].arrival_time;
      Ready_Queue[num_procsses2].CPU_burst=Ready_Queue[i].CPU_burst;
      Ready_Queue[num_procsses2].remaining_time=Ready_Queue[i].remaining_time;
      Ready_Queue[num_procsses2].number_of_tickets=Ready_Queue[i].number_of_tickets;
      num_procsses2++;
      end_time = ctime;
      fprintf(out, "%s", "Time ");
      fprintf(out, "%ld", end_time);
      fprintf(out, "%s", ": P");
      fprintf(out, "%i",Ready_Queue[i].P_ID );
      fprintf(out, "%s", " " );
      fprintf(out, "%s\n", "Entering quantum");
    }


    }
int sum_of_turnaround,sum_of_waitingtime,wait;

for (int i = num_procsses2 ; i >=0; i--) {
  sum_of_turnaround+=Ready_Queue[i].end_time;

  sum_of_waitingtime+=Ready_Queue[i].Wait;
}
double AW,AT;
AW=sum_of_waitingtime/num_procsses;
AT=sum_of_turnaround/num_procsses;
fprintf(out, "%s", "Average Waiting Time = ");
fprintf(out, "%f\n", AW);
fprintf(out, "%s", "Average Turnaround Time =");
fprintf(out, "%f\n",AT );

    fclose(fptr);
    return 0;

}
