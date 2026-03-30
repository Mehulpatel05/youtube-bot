// LRC and Checksum 


//lrc
#include <stdio.h> 
#include <string.h> 
void calculateLRC(char data[][9], int rows, char *lrc) { 
int colSum[8] = {0}; 
// Calculate LRC 
for (int col = 0; col < 8; col++) { 
for (int row = 0; row < rows; row++) { 
colSum[col] ^= (data[row][col] - '0'); // XOR each bit column-wise 
} 
lrc[col] = colSum[col] + '0'; // Convert back to character 
} 
lrc[8] = '\0'; // Null-terminate the LRC 
} 
int main() { 
char data[4][9] = { // Example binary data (8 bits per block) 
"11001101", 
"10101010", 
"11110000", 
"00001111" 
}; 
char lrc[9]; 
calculateLRC(data, 4, lrc); 
printf("Input Data Blocks:\n"); 
for (int i = 0; i < 4; i++) { 
printf("%s\n", data[i]); 
} 
printf("LRC: %s\n", lrc); 
return 0; 
}   

















//chceksum 
#include <string.h> 
// Function to calculate checksum 
unsigned int calculateChecksum(int data[], int n) { 
unsigned int sum = 0; 
// Add all data blocks 
for (int i = 0; i < n; i++) { 
sum += data[i]; 
} 
// Calculate the 1's complement of the sum 
unsigned int checksum = ~sum; 
return checksum; 
} 
int main() { 
int data[] = {0x1234, 0x5678, 0x9ABC, 0xDEF0}; // Example data (16-bit blocks) 
int n = sizeof(data) / sizeof(data[0]); 
// Calculate checksum 
unsigned int checksum = calculateChecksum(data, n); 
printf("Input Data Blocks:\n"); 
for (int i = 0; i < n; i++) { 
printf("0x%X\n", data[i]); 
} 
printf("Checksum: 0x%X\n", checksum); 
return 0; 
}  