#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* maxmin(void* param); //Function definition for threads

int main(){
    pid_t child1;
    int fd1[2];
    int n1 = 0;
    pipe(fd1);
    printf("Enter to parent the no. of terms of Fibonacci series: ");
    scanf("%d", &n1);
    child1 = fork();
    if(child1 == 0)  //Child 1
    {
        close(fd1[1]);
        read(fd1[0], &n1, sizeof(n1));
        printf("Child 1 received %d\n", n1);
        printf("The Fibonacci Series printed by Child 1 is: ");
        int num1 = 1, num2 = 1;
        if(n1 <= 1){
        	printf("%d", num1);
        }else if(n1 <= 2){
        	printf("%d %d ", num1, num2);
        }else{
        	printf("%d %d ", num1, num2);
        	int num3 = 0;
        	int count = 2;
        	while(count < n1){
        		num3 = num1 + num2;
        		printf("%d ", num3);
        		num1 = num2;
        		num2 = num3;
        		count++;
        	}
        }
        printf("\n");
        close(fd1[0]);
        exit(0);
    }
    else if (child1 > 0) //Parent
    {
    	close(fd1[0]);
        write(fd1[1], &n1, sizeof(n1));
        close(fd1[1]);
        wait(NULL);
    
    	pid_t child2;
    	int pipe2[2], pipe3[2], pipe4[2];
    	pipe(pipe2);
    	pipe(pipe3);
    	pipe(pipe4);
    	child2 = fork();
    	if(child2 == 0) //Child 2
    	{
    		int nc2, nc3;
        	close(pipe2[1]);
        	read(pipe2[0], &nc2, sizeof(nc2));
        	close(pipe2[0]);
        	printf("\nChild 2 received %d as no. of threads\n", nc2);
        
        	close(pipe3[1]);
        	read(pipe3[0], &nc3, sizeof(nc3));
        	close(pipe3[0]);
        	printf("\nChild 2 received %d as number of array elements\n", nc3);
        
        	int arrc[nc3];
        	close(pipe4[1]);
        	read(pipe4[0], arrc, sizeof(arrc));
        	close(pipe4[0]);

        	int size = nc3/nc2, count = 0;
        	int gmax = -1, gmin = 1001;
        	pthread_t thread[nc2];
        	for(int i = 0; i < nc2; i++){
        		int val[size+4];
        		val[0] = size;
       	 	val[1] = i*(size); //Lower Bound Value
        		val[2] = ((i+1)*size) - 1; //Upper Bound Value
        		val[3] = i+1; //Thread Number
        		for(int j = 4; j < size+4; j++) val[j] = arrc[count++]; 
        		pthread_create(&thread[i], NULL, maxmin, (void*)val);
        		int* retarr;
        		pthread_join(thread[i], (void*)&retarr);
        		if(((int*)retarr)[1] > gmax) gmax = ((int*)retarr)[1];
       	 	if(((int*)retarr)[0] < gmin) gmin = ((int*)retarr)[0];
        	}
        	printf("\nCalculated by Child 2: max = %d, min = %d\n", gmax, gmin);
        	exit(0);
    	}
    	else if (child2 > 0) //Parent Continues Here
    	{
    		int n2, n3;
    		printf("\nEnter to parent the no. of child threads to be created: ");
    		scanf("%d", &n2);
        	close(pipe2[0]);
        	write(pipe2[1], &n2, sizeof(n2));
        	close(pipe2[1]);
        	//sleep(0.2); //This statement will help align the output on screen

	        printf("\nEnter to parent number of array elements for Child 2: ");
	    	scanf("%d", &n3);
	    	close(pipe3[0]);
	        write(pipe3[1], &n3, sizeof(n3));
	        close(pipe3[1]);
	        //sleep(0.1); //This statement will help align the output on screen
        
        	int arr[n3];
        	printf("\nEnter to parent the array elements for Child 2: ");
        	for(int i = 0; i < n3; i++){
        		scanf("%d", &arr[i]);
        	}
        	close(pipe4[0]);
        	write(pipe4[1], arr, sizeof(arr));
        	close(pipe4[1]);
        	wait(NULL);
        
        	pid_t child3;
    		int fd3[2];
    		int n4 = 0;
    		pipe(fd3);
    		printf("\nEnter the no. for factorial computation in parent: ");
    		scanf("%d", &n4);
    		child3 = fork();
    		if (child3 > 0) //Parent continues here
    		{
    			close(fd3[0]);
        		write(fd3[1], &n4, sizeof(n4));
        		close(fd3[1]);
    		}
    		else if(child3 == 0) //Child 3
    		{
        		close(fd3[1]);
        		read(fd3[0], &n4, sizeof(n4));
        		close(fd3[0]);
       	 	printf("\nChild 3 received %d for factorial computation\n", n4);
       	 	int fact = 1;
       	 	for(int i = 1; i <= n4; i++){
       	 	    fact *= i;
       	 	}
       	 	printf("\nFactorial computed by Child 3 is %d\n", fact);
       	 	close(fd3[0]);
       	 	exit(0);
    		}
	}
    }
    return 0;
}

void* maxmin(void* param){
	int* subarr = (int *) param;
	int lmin = 1001, lmax = 0;
	for(int i = 4; i < subarr[0]+4; i++){
		if(subarr[i] > lmax) lmax = subarr[i];
		if(subarr[i] < lmin) lmin = subarr[i];
	}
	printf("\nLower bound for Thread %d is %d", subarr[3],subarr[1]+1);
	printf("\nUpper bound for Thread %d is %d", subarr[3],subarr[2]+1);
	printf("\nComputed by Thread %d: max = %d, min = %d\n", subarr[3], lmax, lmin);
	
	int *ret = (int*)malloc(2*sizeof(int));
	ret[0] = lmin;
	ret[1] = lmax;
	pthread_exit((void*)ret);
}



