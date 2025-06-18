(01)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSGSIZE 16

char* msg1 = "hello, world #1";
char* msg2 = "hello, world #2";
char* msg3 = "hello, world #3";


int main(){
	char inbuf[MSGSIZE];
	int p[2],i;
	
	if(pipe(p)<0)
		exit(1);
	
	/*continued
	write pipe*/
	
	write(p[1], msg1,MSGSIZE); //write(file descriptor ,message,message size)
	write(p[1], msg2,MSGSIZE);
	write(p[1], msg3,MSGSIZE);
	
	for(i=0;i<3;i++){
		//read pipe
		read(p[0],inbuf,MSGSIZE); //read(fildes,msg,msgsize)
		printf("%s\n",inbuf);
	}
	return 0;
}


===================================================================================================================================================
 exercise (01)
1. get 2 inputs from users(//parent process)
name:
regno:
age:
2. print the output from the child(//child process)

name:tikku
regno:2021ict89
age:23

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char name[50];
    char regno[20];
    int age;

    // Taking input in the parent process
    printf("Enter Name: ");
    scanf("%s", name);
    printf("Enter Reg No: ");
    scanf("%s", regno);
    printf("Enter Age: ");
    scanf("%d", &age);

    pid_t pid = fork();  // Creating the child process

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if (pid == 0) { // Child process
        printf("\nChild Process Output:\n");
        printf("Name: %s\n", name);
        printf("Reg No: %s\n", regno);
        printf("Age: %d\n", age);
    }
    else { // Parent process
        wait(NULL); // Wait for child to finish
        printf("\nParent Process Completed!\n");
    }

    return 0;
}



----------------------------------------------------------------------------------------------
(pipe method)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MSGSIZE 100  // Define message size

int main() {
    int p[2];  // Pipe file descriptors
    char buffer[MSGSIZE];  // Buffer to store received message
    pid_t pid;

    // Creating a pipe
    if (pipe(p) < 0) {
        perror("Pipe creation failed");
        exit(1);
    }

    pid = fork();  // Creating child process

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid > 0) { // Parent Process
        close(p[0]); // Close reading end of the pipe

        // Taking user input
        char name[50], regno[20], age[10];
        printf("Enter Name: ");
        scanf("%s", name);
        printf("Enter Reg No: ");
        scanf("%s", regno);
        printf("Enter Age: ");
        scanf("%s", age);

        // Formatting message and writing to pipe
        snprintf(buffer, MSGSIZE, "Name: %s\nReg No: %s\nAge: %s", name, regno, age);
        write(p[1], buffer, strlen(buffer) + 1);  // Sending formatted message to child
        
        close(p[1]); // Close writing end of the pipe
    }
    else { // Child Process
        close(p[1]); // Close writing end of the pipe

        // Reading data from parent
        read(p[0], buffer, MSGSIZE);
        printf("\nChild Process Output:\n%s\n", buffer);

        close(p[0]); // Close reading end of the pipe
    }

    return 0;
}

===================================================================================================================================================
exercise (02)
 step01: parent process get input from user
 step02: sending these inputs to child process
 step03: child process getting these inputs from parent and doing calculation
 step04: step the calculated values to parent
 step05: parent print the outputs
 
 [circle,rectangle,triangle,square]
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void calculate_area(int shape_code, float dimensions[], float *result) {
    switch (shape_code) {
        case 1: // Circle
            *result = 3.1416 * dimensions[0] * dimensions[0];
            break;
        case 2: // Rectangle
            *result = dimensions[0] * dimensions[1];
            break;
        case 3: // Triangle
            *result = 0.5 * dimensions[0] * dimensions[1];
            break;
        case 4: // Square
            *result = dimensions[0] * dimensions[0];
            break;
        default:
            *result = -1;  // Invalid shape
    }
}

int main() {
    int fd1[2], fd2[2];  // Pipes: fd1 for sending input, fd2 for receiving output
    pid_t pid;

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) {  // Parent Process
        close(fd1[0]);  // Close read end of fd1
        close(fd2[1]);  // Close write end of fd2

        int shape_code;
        float dimensions[2];

        printf("Select shape:\n");
        printf("1. Circle\n2. Rectangle\n3. Triangle\n4. Square\n");
        printf("Enter choice (1-4): ");
        scanf("%d", &shape_code);

        if (shape_code == 1 || shape_code == 4) {
            printf("Enter dimension (radius/side): ");
            scanf("%f", &dimensions[0]);
        } else if (shape_code == 2 || shape_code == 3) {
            printf("Enter first dimension: ");
            scanf("%f", &dimensions[0]);
            printf("Enter second dimension: ");
            scanf("%f", &dimensions[1]);
        } else {
            printf("Invalid choice!\n");
            return 1;
        }

        // Send data to child process
        write(fd1[1], &shape_code, sizeof(shape_code));
        write(fd1[1], dimensions, sizeof(dimensions));
        close(fd1[1]);  // Close write end of fd1

        float result;
        read(fd2[0], &result, sizeof(result));  // Read result from child
        close(fd2[0]);  // Close read end of fd2

        if (result != -1) {
            printf("The calculated area is: %.2f\n", result);
        } else {
            printf("Error in calculation.\n");
        }
    } else {  // Child Process
        close(fd1[1]);  // Close write end of fd1
        close(fd2[0]);  // Close read end of fd2

        int shape_code;
        float dimensions[2], result;

        read(fd1[0], &shape_code, sizeof(shape_code));
        read(fd1[0], dimensions, sizeof(dimensions));
        close(fd1[0]);  // Close read end of fd1

        calculate_area(shape_code, dimensions, &result);

        write(fd2[1], &result, sizeof(result));  // Send result to parent
        close(fd2[1]);  // Close write end of fd2

        exit(0);
    }

    return 0;
}
