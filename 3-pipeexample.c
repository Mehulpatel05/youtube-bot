pipes and related system calls for pipe management


6. Code Implementation 
Below is an example in C: 
Copy code 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
int main() { 
int fd[2]; // File descriptors for the pipe 
pid_t pid; 
char write_msg[] = "Hello from parent!"; 
char read_msg[100]; 
// Step 2: Create the pipe 
if (pipe(fd) == -1) { 
perror("Pipe failed"); 
return 1; 
} 
// Step 3: Fork a child process 
pid = fork(); 
if (pid < 0) { 
perror("Fork failed"); 
return 1; 
} 
if (pid > 0) { // Parent process 
// Step 4: Close unused read end 
close(fd[0]); 
// Step 5: Write to the pipe 
write(fd[1], write_msg, strlen(write_msg) + 1); 
close(fd[1]); // Close write end after writing 
} else { // Child process 
// Step 4: Close unused write end 
close(fd[1]); 
// Step 5: Read from the pipe 
read(fd[0], read_msg, sizeof(read_msg)); 
printf("Child received: %s\n", read_msg); 
close(fd[0]); // Close read end after reading 
} 
return 0; 
}    










8. Compile and Run 
bash 
Copy code 
gcc -o pipe_example pipe_example.c 
./pipe_example