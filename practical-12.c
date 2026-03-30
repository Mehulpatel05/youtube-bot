//Token Bucket  


c 
Copy code 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#define MAX_BUCKET_SIZE 10  // Maximum number of tokens in the bucket 
#define TOKEN_RATE 1        
#define DATA_SIZE 1         
// Number of tokens generated per second 
// Data size in packets (fixed size for simplicity) 
int bucket_tokens = 0;  // Current number of tokens in the bucket 
int max_bucket_size = MAX_BUCKET_SIZE;  // Maximum capacity of the token bucket 
int token_rate = TOKEN_RATE;  // Tokens generated per second 
// Function to simulate the generation of tokens 
void generateTokens() { 
if (bucket_tokens < max_bucket_size) { 
bucket_tokens += token_rate;  // Generate tokens at a fixed rate 
if (bucket_tokens > max_bucket_size) { 
bucket_tokens = max_bucket_size;  // Bucket cannot hold more than max size 
} 
printf("Tokens generated: %d. Current tokens in the bucket: %d\n", token_rate, 
bucket_tokens); 
} else { 
printf("Bucket is full. No new tokens generated.\n"); 
} 
} 
// Function to simulate data transmission 
void transmitData() { 
if (bucket_tokens >= DATA_SIZE) { 
bucket_tokens -= DATA_SIZE;  // Consume tokens for data transmission 
printf("Data transmitted: 1 packet. Remaining tokens in the bucket: %d\n", bucket_tokens); 
} else { 
printf("Insufficient tokens. Data transmission paused.\n"); 
} 
} 
int main() { 
printf("Token Bucket Congestion Control Simulation\n"); 
printf("Max bucket size: %d tokens\n", max_bucket_size); 
printf("Token rate: %d tokens per second\n", token_rate); 
while (1) { 
// Simulate token generation every second 
generateTokens(); 
// Simulate data transmission if tokens are available 
transmitData(); 
// Wait for 1 second to simulate time intervals 
sleep(1); 
} 
return 0; 
} 