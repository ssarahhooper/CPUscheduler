
/* FCFS SCHEDULING */

// /*ku_push ku_pop */
// pid_t ku_pop(void)
// {
// 	pid_t pid;
// 	int i;

// 	if(ku_is_empty()) return IDLE;

// 	pid = job_queue[0].pid;
// 	for(i = 1; i < job_count; i++){
// 	job_queue[i-1] = job_queue[i];
// 	}
// 	job_count--;

// 	return pid;

// }
// void ku_push(pid_t pid, int jt)
// {
// 	if(job_count >= MAX_JOBS)
// 	{
// 		printk(KERN_ERR "Waiting Queue is full\n");
// 		return;
// 	}

// 	job_queue[job_count].pid = pid;
// 	job_queue[job_count].jobTime = jt;
// 	job_count++;
// }


// SYSCALL_DEFINE2(os2024_ku_cpu, char*, name, int, jobTime){

// 	job_t newJob;
// 	newJob.pid = current->pid;
// 	newJob.jobTime = jobTime;

// 	if(now == IDLE) now = newJob.pid;

// 	if (now == newJob.pid){
// 		if(jobTime == 0){
// 			printk("Process Finished: %s\n", name);

// 			if(ku_is_empty()) now = IDLE;
// 			else now = ku_pop();

// 		}
// 		else printk("Working: %s\n", name);
// 	return 0;
// 	}
// 	else{
// 		if (ku_is_new_id(newJob.pid)) ku_push(newJob.pid, newJob.jobTime);
// 		printk("Working Denied: %s \n", name );
// 	}
// 	return 1;
// }


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/* SJF SCHEDULING */
////////////////////
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/slab.h>

/*what i need to implement*/
/*global variables*/
#define IDLE -1
#define MAX_JOBS 10


/*job_t*/
typedef struct job {
	pid_t pid;
	int jobTime;
	struct list_head list; // For linked list management
} job_t;

static pid_t now = IDLE;
static job_t job_queue[MAX_JOBS];
static int job_count = 0;

/*ku_is_empty()*/
bool ku_is_empty(void)
{
	return job_count == 0;
}
/*ku_is_new_id*/
bool ku_is_new_id(pid_t pid)
{
	int i;
	for(i = 0; i < job_count; i++)
	{
	if(job_queue[i].pid == pid) return false;
	}
	return true;
}

/*ku_push ku_pop */
pid_t ku_pop(void)
{
	pid_t pid;
        int i;
        int j;
        int index = 0;

	if(ku_is_empty()) return IDLE;

	for (i = 1; i < job_count; i++) {
	if (job_queue[i].jobTime < job_queue[index].jobTime) {
	index = i;
		}
	}

	pid = job_queue[index].pid;

	for(j = index + 1; j < job_count; j++){
		job_queue[j-1] = job_queue[j];
	}
	job_count--;

	return pid;

}
void ku_push(pid_t pid, int jt)
{
	if(job_count >= MAX_JOBS)
	{
		printk(KERN_ERR "Waiting Queue is full\n");
		return;
	}

	job_queue[job_count].pid = pid;
	job_queue[job_count].jobTime = jt;
	job_count++;
}




SYSCALL_DEFINE1(os2024_pid_print, char*, name)
{
	pid_t pid;
	pid = current->pid;
	printk("Process name: %s pid: %d\n", name, pid);

	return 0;
}

SYSCALL_DEFINE2(os2024_ku_cpu, char*, name, int, jobTime){

	job_t newJob;
	newJob.pid = current->pid;
	newJob.jobTime = jobTime;

	if(now == IDLE) now = newJob.pid;

		if (now == newJob.pid){
			if(jobTime == 0){
			printk("Process Finished: %s\n" ,name);

				if(ku_is_empty()) now = IDLE;
				else now = ku_pop();

			}
			else printk("Working: %s\n", name);
			return 0;
			}
		else{
		if (ku_is_new_id(newJob.pid)) ku_push(newJob.pid, newJob.jobTime);
		printk("Working Denied: %s \n", name );
	}
	return 1;
}


