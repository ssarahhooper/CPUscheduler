#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KU_CPU 339

int main(int argc, char **argv) {
    int jobTime;
    int delayTime;
    char name[4];
    int wait = 0;  // Initialize wait time to 0

    if (argc < 4) {
        printf("\nInsufficient Arguments..\n");
        return 1;
    }

    jobTime = atoi(argv[1]);
    delayTime = atoi(argv[2]);
    strcpy(name, argv[3]);

    sleep(delayTime);
    printf("\nProcess %s: I will use CPU for %ds.\n", name, jobTime);
    jobTime *= 10;


    int first_cpu_use = 0;
    int response = 0;
    // Start processing the job
    while (jobTime) {
        if (!syscall(KU_CPU, name, jobTime)) {
		if(first_cpu_use == 0) first_cpu_use = 1;
            jobTime--;
        } else {
		if(first_cpu_use == 0) ++response;
            wait++;
        }
        usleep(100000); // Sleep for 100ms to simulate processing delay
    }

    // Calculate response time and wait time
    int response_time = (response + 5) / 10; // Adjust based on
//how much time has passed
    int total_wait_time = (wait + 5) / 10; // Adjust wait time to seconds

    syscall(KU_CPU, name, 0);
    printf("\nProcess %s: Finish! My response time is %ds and my total wait time is %ds.\n", name, response_time, total_wait_time);

    return 0;
}
