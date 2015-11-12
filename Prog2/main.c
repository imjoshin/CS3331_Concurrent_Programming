// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 02/27/15                                      
// PROGRAM ASSIGNMENT #1                                     
// FILE NAME : main.c           
// PROGRAM PURPOSE :                                           
//          This is the main program that will sort arrays
//	concurrently by quick sort and merge sort. Data is read
//	from an input file and stored in shared memory.
// ----------------------------------------------------------- 

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>

// ----------------------------------------------------------- 
// FUNCTION  main :                          
//     This function is called when the process is started.
//	It takes an input file and sorts an array by qsort and
//	two arrays by merge sort.                           
// PARAMETER USAGE :                                           
//                  
// FUNCTION CALLED :                                           
//    printf, scanf, perror, shmget, ftok, shmat, shmdt,
//	shmctl, exit, execvp, wait         
// ----------------------------------------------------------- 

int main(int argc, char* argv[]){
	int i, k, m, n, forkID, mshmID, qshmID;
	key_t mkey, qkey;
	char pre[] = "*** MAIN: ";

	printf("Quicksort and Binary Merge with Multiple Processes:\n\n");

	//read k
	if(scanf("%d", &k) <= 0) perror("Error reading input");
	
	//create struct
	struct qstruct {
		int n;
		int array[k];
	};

	struct qstruct qsort;
	qsort.n = k;

	int a[k];
	//read a
	for(i = 0; i < k; i++){
		if(scanf("%d", &qsort.array[i]) <= 0) perror("Error reading input");
	}
	//read m
	if(scanf("%d", &m) <= 0) perror("Error reading input");
	
	int x[m];
	//read x
	for(i = 0; i < m; i++){
		if(scanf("%d", &x[i]) <= 0) perror("Error reading input");
	}

	//read n
	if(scanf("%d", &n) <= 0) perror("Error reading input");
	
	
	int y[n];
	//read y
	for(i = 0; i < n; i++){
		if(scanf("%d", &y[i]) <= 0) perror("Error reading input");
	}

	//create struct
	struct mstruct {
		int xsize;
		int ysize;
		int x[m];
		int y[n];
		int ret[m + n];
	};



	//create key
	qkey = ftok("./", 'q');

	//create shared memory for qsort
	printf("%sshared memory key = %d\n", pre, qkey);

	if((qshmID = shmget(qkey, sizeof(struct qstruct), IPC_CREAT | 0666)) < 0){
		printf("Failed to create shared memory.\n");
		exit(1);
	}	
	printf("%sshared memory created\n", pre);

	//set shared memory values
	struct qstruct* qdata;
	qdata = (struct qstruct*) shmat(qshmID, NULL, 0);
	qdata->n = qsort.n;
	for(i = 0; i < qsort.n; i++){
		qdata->array[i] = qsort.array[i];
	}
	
	printf("%sshared memeory attached and is ready to use\n\n", pre);



	//create key
	mkey = ftok("./", 'm');

	//create shared memory for merge
	printf("%sshared memory key = %d\n", pre, mkey);

	if((mshmID = shmget(mkey, sizeof(struct mstruct), IPC_CREAT | 0666)) < 0){
		printf("Failed to create shared memory.\n");
		exit(1);
	}	
	printf("%sshared memory created\n", pre);

	//set shared memory values
	struct mstruct* mdata;
	mdata = (struct mstruct*) shmat(mshmID, NULL, 0);
	/*if((int) qdata < 0){
		printf("Failed to attached shared memory.");
		exit(1);
	}*/
	
	printf("%sshared memeory attached and is ready to use\n\n", pre);

	//set shared memory to stored values
	mdata->xsize = m;
	mdata->ysize = n;

	for(i = 0; i < m || i < n; i++){
		if(i < m) mdata->x[i] = x[i];
		if(i < n) mdata->y[i] = y[i];
	}
	

	//print out input
	printf("Input array for qsort has %d elements:\n", k);
	printf("  ");
	for(i = 0; i < k; i++) printf("  %d",qsort.array[i]);
	printf("\n");
	
	printf("Input array x[] for merge has %d elements:\n", m);
	printf("  ");
	for(i = 0; i < m; i++) printf("  %d", mdata->x[i]);
	printf("\n");
	
	printf("Input array y[] for merge has %d elements:\n", n);
	printf("  ");
	for(i = 0; i < n; i++) printf("  %d", mdata->y[i]);
	printf("\n");



	//generate argv for child
	char* args[5];
	char right[16]; sprintf(right, "%d", k - 1);
	char asize[16]; sprintf(asize, "%d", k);
	char qprog[] = {"qsort"}; char left[] = {"0"};
	args[0] = qprog; args[1] = left; args[2] = right; args[3] = asize; args[4] = NULL;
	
	printf("%sabout to spawn the process for qsort\n", pre);

	//fork and execute
	forkID = fork();
	if(forkID == 0){
		if(execvp("./qsort", args) < 0){
			printf("execvp() failed\n");
			exit(1);
		}
		exit(0);
	}


	//generate argv for child
	char mprog[] = {"merge"}; 
	char xsize[16]; sprintf(xsize, "%d", m);
	char ysize[16]; sprintf(ysize, "%d", n);
	args[0] = mprog; args[1] = xsize; args[2] = ysize; args[3] = NULL;
 
	printf("%sabout to spawn the process for merge\n", pre);
	
	//fork and execute
	forkID = fork();
	if(forkID == 0){
		if(execvp("./merge", args) < 0){
			printf("execvp() failed\n");
			exit(1);
		}
		exit(0);
	}

	//wait for both children to complete
	wait();
	wait();

	//print out sorted arrays
	printf("\n%ssorted array by qsort:\n  ", pre);
	for(i = 0; i < k; i++) printf("  %d", qdata->array[i]);
	printf("\n");

	printf("%smerged array:\n  ", pre);
	for(i = 0; i < m + n; i++) printf("  %d", mdata->ret[i]);
	printf("\n");


	//detach and remove shared memory
	shmdt((void *) qdata);
	printf("\n%sshared memory successfully detached\n", pre);
	shmctl(qshmID, IPC_RMID, NULL);
	printf("%sshared memory successfully removed\n\n", pre);

	shmdt((void *) mdata);
	printf("%sshared memory successfully detached\n", pre);
	shmctl(mshmID, IPC_RMID, NULL);
	printf("%sshared memory successfully removed\n\n", pre);



}