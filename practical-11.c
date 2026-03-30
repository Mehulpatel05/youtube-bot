//control protocols: Leaky Bucket  

c 
Copy code 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#define MAX_BUCKET_SIZE 10 // Maximum bucket capacity (in packets) 
#define LEAK_RATE 1 // Leak rate (in packets per second) 
int bucket_size = 0; // Current size of the bucket 
// Function to simulate data arriving at random intervals 
void simulateIncomingData() { 
// Simulate the random arrival of data packets 
int data_arrival = rand() % 3 + 1; // Random data arrival between 1 and 3 packets 
printf("Incoming data: %d packets\n", data_arrival); 
if (bucket_size + data_arrival <= MAX_BUCKET_SIZE) { 
bucket_size += data_arrival; // Add data to the bucket 
printf("Data added to the bucket. Current bucket size: %d packets\n", bucket_size); 
} else { 
int overflow = (bucket_size + data_arrival) - MAX_BUCKET_SIZE; 
bucket_size = MAX_BUCKET_SIZE; // Bucket is full, add only up to the max size 
printf("Bucket overflow! %d packets discarded.\n", overflow); 
printf("Current bucket size: %d packets\n", bucket_size); 
} 
} 
// Function to simulate the leak process 
void leakData() { 
if (bucket_size > 0) { 
bucket_size -= LEAK_RATE; // Leak data at the fixed rate 
if (bucket_size < 0) { 
bucket_size = 0; // Bucket can't be negative 
} 
printf("Data leaked at rate %d. Current bucket size: %d packets\n", LEAK_RATE, 
bucket_size); 
} 
} 
int main() { 
printf("Leaky Bucket Congestion Control Simulation\n"); 
printf("Max bucket size: %d packets\n", MAX_BUCKET_SIZE); 
printf("Leak rate: %d packets per second\n", LEAK_RATE); 
while (1) { 
// Simulate incoming data every 1 second 
simulateIncomingData(); 
// Simulate the leaking of data every 1 second 
leakData(); 
// Wait for 1 second to simulate time intervals 
sleep(1); 
} 
return 0; 
} 