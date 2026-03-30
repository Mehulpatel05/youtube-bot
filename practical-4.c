//character Count Protocol 


#include <stdio.h> 
#include <string.h> 
// Function to simulate the sender 
void sender(const char *messages[], int num_messages, char frames[][100]) { 
for (int i = 0; i < num_messages; i++) { 
int length = strlen(messages[i]);    // Calculate the message length 
frames[i][0] = length;               
// First byte is the length 
strcpy(frames[i] + 1, messages[i]); // Copy the message after the length 
} 
} 
// Function to simulate the receiver 
void receiver(char frames[][100], int num_frames) { 
for (int i = 0; i < num_frames; i++) { 
int length = frames[i][0]; // Read the first byte as length 
printf("Frame %d (Length: %d): %.*s\n", i + 1, length, length, frames[i] + 1); 
} 
} 
int main() { 
const char *messages[] = {"Hello", "World", "Character Count Protocol"}; 
int num_messages = sizeof(messages) / sizeof(messages[0]); 
char frames[10][100]; // Array to store frames 
// Step 1: Sender creates frames 
sender(messages, num_messages, frames); 
// Step 2: Receiver processes frames 
printf("Receiver Output:\n"); 
receiver(frames, num_messages); 
return 0; 
} 












● Compile the code: 
bash 
Copy code 
gcc -o char_count_protocol char_count_protocol.c 
● Run the program: 
bash 
Copy code 
./char_count_protocol 