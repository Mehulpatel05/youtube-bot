//Bit Stuffing 

#include <stdio.h> 
#include <string.h> 
#define FLAG "01111110" 
// Function to perform bit stuffing at the sender's side 
void sender(const char *data, char *stuffed_data) { 
int count = 0, j = 0; 
// Add the flag sequence at the start of the frame 
strcpy(stuffed_data, FLAG); 
j += strlen(FLAG); 
for (int i = 0; data[i] != '\0'; i++) { 
if (data[i] == '1') { 
count++; 
} else { 
count = 0; 
} 
// Add the current bit to the stuffed data 
stuffed_data[j++] = data[i]; 
// Stuff a '0' after five consecutive '1's 
if (count == 5) { 
stuffed_data[j++] = '0'; 
count = 0; 
} 
} 
// Add the flag sequence at the end of the frame 
strcpy(stuffed_data + j, FLAG); 
j += strlen(FLAG); 
stuffed_data[j] = '\0'; // Null-terminate the stuffed data 
} 
// Function to perform bit unstuffing at the receiver's side 
void receiver(const char *stuffed_data, char *original_data) { 
int count = 0, j = 0; 
// Skip the initial flag sequence 
int start = strlen(FLAG); 
for (int i = start; stuffed_data[i] != '\0'; i++) { 
// Stop at the final flag sequence 
if (strncmp(stuffed_data + i, FLAG, strlen(FLAG)) == 0) { 
break; 
} 
if (stuffed_data[i] == '1') { 
count++; 
} else { 
count = 0; 
} 
// Add the current bit to the original data 
original_data[j++] = stuffed_data[i]; 
// Skip the stuffed '0' after five consecutive '1's 
if (count == 5 && stuffed_data[i + 1] == '0') { 
i++; 
count = 0; 
} 
} 
original_data[j] = '\0'; // Null-terminate the original data 
} 
int main() { 
const char *data = "0111111011111100001111111"; // Input binary string 
char stuffed_data[100], original_data[100]; 
// Step 1: Perform bit stuffing 
sender(data, stuffed_data); 
printf("Stuffed Data: %s\n", stuffed_data); 
// Step 2: Perform bit unstuffing 
receiver(stuffed_data, original_data); 
printf("Original Data: %s\n", original_data); 
return 0; 
}