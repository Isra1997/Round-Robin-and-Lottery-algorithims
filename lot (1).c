// gcc -o hello hello.c
// ./hello

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct  process{
    int P_ID;
    int arrival_time;
    int CPU_burst;
    int remaining_time;
    int start_time;
    int end_time;
    int test_time;
    int number_of_tickets;
};

int main(){
    FILE *fptr; // var for filename
    char filename[100], c;
    int  quantum, P_ID, arrival_time, CPU_burst, start_time, end_time, number_of_tickets, total_number_of_tickets; // global vars
    int num_processes;
    char buff [100]; // var to read quantum time from file
    struct process Ready_Queue [50]; // Ready_Queue of the processes in the file
    clock_t start_t, end_t, total_t; // time variables

    printf("Please enter the file directory you want open \n");
    scanf("%s", filename);

    // open input file
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    // set output file
    FILE *out = fopen("/Users/israragheb/Desktop/OS_MP2/OS_MP2/out.txt", "w+");

    // intialize the quantum time
    fgets(buff, 255, (FILE*)fptr);
    sscanf(buff, "%d", &quantum);
    printf("quantum:\n");
    printf("%d\n",quantum );

    // intialize the total number of tickets
    fgets(buff, 255, (FILE*)fptr);
    sscanf(buff, "%d", &total_number_of_tickets);
    printf("total_number_of_tickets:\n");
    printf("%d\n",total_number_of_tickets);

    // add the processes to the Ready_Queue
    for (int i = 0; fscanf(fptr , "%d,%d,%d,%d" , &P_ID , &arrival_time , &CPU_burst ,  &number_of_tickets)>0; i++) {
        Ready_Queue[i].P_ID=P_ID;
        Ready_Queue[i].arrival_time=arrival_time;
        Ready_Queue[i].CPU_burst=CPU_burst;
        Ready_Queue[i].remaining_time=CPU_burst;
        Ready_Queue[i].number_of_tickets=number_of_tickets;
        Ready_Queue[i].start_time = -1; // IMPORTANT
        num_processes++;
    }

    printf("Please wait, this may take up to 10 seconds...\n");

    // lottery
    // assigning numbers to each process
    int tickets[num_processes];
    int total = 0; // cumulative ticket counter, for process selection
	for(int c1 = 0; c1 < num_processes; c1++) {
	    total += Ready_Queue[c1].number_of_tickets;
	    tickets[c1] = total;
	}

	// lottery vars
	time_t t; // used in random number generation
	srand((unsigned) time(&t)); // used in random number generation
	int c3 = 0; // counter
	long ck = (clock() / CLOCKS_PER_SEC)*quantum; // initial clock value
	// pick a random process X times
	// where X is the total number of tickets
	for(int c2 = 0; c2 < total_number_of_tickets; c2++) {
	    // initialize random number
	    // between 0 and num of tickets
	    int random = rand() % total_number_of_tickets;
	    for(c3 = 0; c3 < num_processes; c3++) {
	        // random number is in which range?
	        if(tickets[c3] > random) {
	            // if a finished process is picked, disregard
	            if(Ready_Queue[c3].remaining_time <= 0) {c2++; break;}
	            fprintf(out,"Time : %ld P%i Entering quantum\n", ck, Ready_Queue[c3].P_ID);
              Ready_Queue[c3].test_time=ck;
	            ///printf("Time : %ld P%i Entering quantum\n", ck, Ready_Queue[c3].P_ID);
	            if(Ready_Queue[c3].start_time == -1) Ready_Queue[c3].start_time = ck;
	            // if remaining time more than quantum
	            if(Ready_Queue[c3].remaining_time > quantum) {
	                Ready_Queue[c3].remaining_time -= quantum;
	                while((clock() / CLOCKS_PER_SEC)*quantum < ck+quantum) {
                        // do nothing until the quantum passes in real time
	                    ck = ck;
	                }
	                ck += quantum;
	                if(Ready_Queue[c3].remaining_time == 0) Ready_Queue[c3].end_time = ck;
	                break;
	            }
	            // if remaining time less than quantum
	            else {
	                int rem1 = Ready_Queue[c3].remaining_time;
	                Ready_Queue[c3].remaining_time = 0;
	                while((clock() / CLOCKS_PER_SEC)*quantum < ck+rem1) {
	                    // do nothing until the quantum passes in real time
	                    ck = ck;
	                }
	                ck += rem1;
	                Ready_Queue[c3].end_time = ck;
	                break;
	            }
	        }
	    }
	}

	// average times
	float turn_avg = 0;
	float wait_avg = 0;
	for(int i=0; i<num_processes; i++) {
	    float t = Ready_Queue[i].test_time;
	    float w = t - Ready_Queue[i].CPU_burst;
	    turn_avg += t;
	    wait_avg += w;
	}
	turn_avg = turn_avg/num_processes;
	wait_avg = wait_avg/num_processes;
	fprintf(out,"%s","\n");
	fprintf(out,"Average Turnarround Time = %f\n",turn_avg);
	fprintf(out,"Average Waiting Time = %f\n",wait_avg);
	printf("Average Turnarround Time = %f\n",turn_avg);
	printf("Average Waiting Time = %f\n",wait_avg);
}
