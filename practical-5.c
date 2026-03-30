Byte Stuffing 

Below is an example program in C: 
#include <stdio.h> 
#include <string.h> 
#define SOF '@'   // Start-of-frame marker 
#define ESC '#'   // Escape character 
// Function to perform byte stuffing at the sender's side 
void sender(const char *message, char *stuffed_frame) { 
int j = 0; 
stuffed_frame[j++] = SOF; // Add SOF at the start of the frame 
for (int i = 0; message[i] != '\0'; i++) { 
if (message[i] == SOF || message[i] == ESC) { 
stuffed_frame[j++] = ESC; // Add escape character 
} 
stuffed_frame[j++] = message[i]; // Add the actual character 
} 
stuffed_frame[j++] = SOF; // Add SOF at the end of the frame 
stuffed_frame[j] = '\0';  // Null-terminate the stuffed frame 
} 
// Function to perform byte unstuffing at the receiver's side 
void receiver(const char *stuffed_frame, char *original_message) { 
int j = 0; 
for (int i = 1; stuffed_frame[i] != SOF; i++) { // Skip the initial SOF 
if (stuffed_frame[i] == ESC) { 
i++; // Skip the escape character 
} 
original_message[j++] = stuffed_frame[i]; 
} 
original_message[j] = '\0'; // Null-terminate the original message 
} 
int main() { 
const char *message = "Hello @World# Protocol"; 
char stuffed_frame[100], original_message[100]; 
// Step 1: Perform byte stuffing 
sender(message, stuffed_frame); 
printf("Stuffed Frame: %s\n", stuffed_frame); 
// Step 2: Perform byte unstuffing 
receiver(stuffed_frame, original_message); 
printf("Original Message: %s\n", original_message); 
return 0; 
} 






