//crc

c 
Copy code 
#include <stdio.h> 
#include <string.h> 
// Function to perform XOR operation 
void xorOperation(char *dividend, const char *divisor, int length) { 
for (int i = 0; i < length; i++) { 
dividend[i] = (dividend[i] == divisor[i]) ? '0' : '1'; 
} 
} 
// Function to perform CRC calculation 
void calculateCRC(const char *data, const char *generator, char *crc) { 
int dataLen = strlen(data); 
int genLen = strlen(generator); 
// Append zeros to the data (equal to generator length - 1) 
char dividend[100]; 
strcpy(dividend, data); 
for (int i = 0; i < genLen - 1; i++) { 
dividend[dataLen + i] = '0'; 
} 
dividend[dataLen + genLen - 1] = '\0'; 
// Perform binary division 
char temp[100]; 
strncpy(temp, dividend, genLen); 
temp[genLen] = '\0'; 
for (int i = 0; i < dataLen; i++) { 
if (temp[0] == '1') { 
xorOperation(temp, generator, genLen); 
} 
memmove(temp, temp + 1, genLen - 1); 
temp[genLen - 1] = dividend[i + genLen]; 
} 
strncpy(crc, temp, genLen - 1); 
crc[genLen - 1] = '\0'; 
} 
// Function to validate CRC 
int validateCRC(const char *data, const char *generator) { 
int dataLen = strlen(data); 
int genLen = strlen(generator); 
// Perform binary division 
char temp[100]; 
strncpy(temp, data, genLen); 
temp[genLen] = '\0'; 
for (int i = 0; i < dataLen - genLen + 1; i++) { 
if (temp[0] == '1') { 
xorOperation(temp, generator, genLen); 
} 
memmove(temp, temp + 1, genLen - 1); 
temp[genLen - 1] = data[i + genLen]; 
} 
// Check if remainder is zero 
for (int i = 0; i < genLen - 1; i++) { 
if (temp[i] != '0') { 
return 0; // Error detected 
} 
} 
return 1; // No error detected 
} 
int main() { 
char data[100], generator[100], transmittedData[100], crc[100]; 
// Input data and generator polynomial 
printf("Enter the data bits: "); 
scanf("%s", data); 
printf("Enter the generator polynomial: "); 
scanf("%s", generator); 
// Calculate CRC 
calculateCRC(data, generator, crc); 
// Append CRC to the data 
strcpy(transmittedData, data); 
strcat(transmittedData, crc); 
printf("Transmitted Data (Data + CRC): %s\n", transmittedData); 
// Validate at receiver 
if (validateCRC(transmittedData, generator)) { 
printf("CRC Check Passed: No Errors Detected.\n"); 
} else { 
printf("CRC Check Failed: Errors Detected.\n"); 
} 
return 0; 
}





1101
1011