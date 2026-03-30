// Hamming Code

c 
Copy code 
#include <stdio.h> 
#include <math.h> 
#include <string.h> 
#define MAX_BITS 32 
// Function to calculate parity bit for a given position (using XOR) 
int calculateParityBit(int data[], int n, int pos) { 
    int parity = 0; 
    for (int i = 0; i < n; i++) { 
        if ((i + 1) & (1 << (pos - 1))) { // Check if bit i is in the parity group 
            parity ^= data[i];  // XOR operation for parity calculation 
        } 
    } 
    return parity; 
} 
 
// Function to encode the data with Hamming code (generate parity bits) 
void encodeHammingCode(char *data, int n, int *encoded) { 
    int i, j, k = 0; 
    int m = n + ceil(log2(n)) + 1;  // Total number of bits (data + parity) 
    int parityPos = 1; 
     
    // Initialize encoded array with data and placeholders for parity bits 
    for (i = 0; i < m; i++) { 
        if (i == parityPos - 1) { 
            encoded[i] = -1;  // Placeholder for parity bit 
            parityPos *= 2; 
        } else { 
            encoded[i] = data[k++] - '0'; // Copy data bits 
        } 
    } 
     
    // Calculate parity bits 
    for (i = 0; i < log2(m); i++) { 
        encoded[(1 << i) - 1] = calculateParityBit(encoded, m, i + 1); 
    } 
} 
 
// Function to detect and correct errors in received data using Hamming Code 
int decodeHammingCode(int *encoded, int n) { 
    int i, j, errorPos = 0; 
    int m = n + ceil(log2(n)) + 1; 
     
    // Check each parity bit 
    for (i = 0; i < log2(m); i++) { 
        int parity = calculateParityBit(encoded, m, i + 1); 
        if (parity != encoded[(1 << i) - 1]) { 
            errorPos += (1 << i); // Find the position of the error 
        } 
    } 
     
    // Correct the error if detected 
    if (errorPos) { 
        printf("Error detected at position: %d\n", errorPos); 
        encoded[errorPos - 1] = (encoded[errorPos - 1] == 1) ? 0 : 1; 
        printf("Data after correction: "); 
        for (i = 0; i < m; i++) { 
            printf("%d", encoded[i]); 
        } 
        printf("\n"); 
    } else { 
        printf("No errors detected.\n"); 
    } 
     
    // Return the decoded data (after removing parity bits) 
    printf("Decoded data: "); 
    for (i = 0; i < m; i++) { 
        if ((i + 1) != (1 << i)) { // Skip parity bits 
            printf("%d", encoded[i]); 
        } 
    } 
    printf("\n"); 
} 
 
int main() { 
    char data[MAX_BITS]; 
    int n; 
     
    // Input data to be transmitted (binary string) 
    printf("Enter the binary data to be encoded: "); 
    scanf("%s", data); 
     
    // Length of data 
    n = strlen(data); 
     
    // Encoded data (with parity bits) 
    int encoded[MAX_BITS] = {0}; 
     
    // Encode the data using Hamming Code 
    encodeHammingCode(data, n, encoded); 
     
    // Print encoded data with parity bits 
    printf("Encoded data with parity bits: "); 
    for (int i = 0; i < n + ceil(log2(n)) + 1; i++) { 
        printf("%d", encoded[i]); 
    } 
    printf("\n"); 
     
    // Simulate a bit error (optional for testing) 
// Example: Change one of the data bits to simulate an error 
// encoded[4] = (encoded[4] == 0) ? 1 : 0; 
// Decode and check for errors 
decodeHammingCode(encoded, n); 
return 0; 
} 