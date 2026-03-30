//Password Policy Enforcement System

c 
Copy code 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#define MIN_PASSWORD_LENGTH 8 
#define PASSWORD_EXPIRY_DAYS 30 
#define MAX_HISTORY 5 
// Structure to store user password and history 
struct User { 
char current_password[100]; 
char password_history[MAX_HISTORY][100]; 
time_t last_changed; 
}; 
// Function to check password length 
int checkPasswordLength(char *password) { 
return strlen(password) >= MIN_PASSWORD_LENGTH; 
} 
// Function to check password complexity (contains uppercase, lowercase, digit, special char) 
int checkPasswordComplexity(char *password) { 
int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0; 
for (int i = 0; password[i] != '\0'; i++) { 
if (isupper(password[i])) has_upper = 1; 
if (islower(password[i])) has_lower = 1; 
if (isdigit(password[i])) has_digit = 1; 
if (ispunct(password[i])) has_special = 1; 
} 
return has_upper && has_lower && has_digit && has_special; 
} 
// Function to check if the password is in history 
int checkPasswordHistory(char *password, struct User *user) { 
for (int i = 0; i < MAX_HISTORY; i++) { 
if (strcmp(password, user->password_history[i]) == 0) { 
return 1; // Password found in history 
} 
} 
return 0; 
} 
// Function to update password history 
void updatePasswordHistory(struct User *user, char *new_password) { 
for (int i = MAX_HISTORY - 1; i > 0; i--) { 
strcpy(user->password_history[i], user->password_history[i - 1]); 
} 
strcpy(user->password_history[0], new_password); // Add new password to history 
} 
// Function to check if the password has expired 
int isPasswordExpired(struct User *user) { 
time_t now = time(NULL); 
double seconds = difftime(now, user->last_changed); 
return (seconds / (60 * 60 * 24)) > PASSWORD_EXPIRY_DAYS; // Compare days 
} 
// Function to set a new password 
int setPassword(struct User *user, char *new_password) { 
// Check if password length is sufficient 
if (!checkPasswordLength(new_password)) { 
printf("Error: Password must be at least %d characters long.\n", 
MIN_PASSWORD_LENGTH); 
return 0; 
} 
// Check if password meets complexity requirements 
if (!checkPasswordComplexity(new_password)) { 
printf("Error: Password must contain an uppercase letter, a lowercase letter, a digit, and a 
special character.\n"); 
return 0; 
} 
// Check if password is in history 
if (checkPasswordHistory(new_password, user)) { 
printf("Error: You cannot reuse your previous passwords.\n"); 
return 0; 
} 
// Update password history and set new password 
updatePasswordHistory(user, new_password); 
strcpy(user->current_password, new_password); 
user->last_changed = time(NULL); 
printf("Password successfully changed.\n"); 
return 1; 
} 
// Function to authenticate user 
int authenticateUser(struct User *user, char *password) { 
if (strcmp(user->current_password, password) == 0) { 
printf("Authentication successful.\n"); 
return 1; 
} else { 
printf("Authentication failed.\n"); 
return 0; 
} 
} 
int main() { 
struct User user = {0};  // Initialize user structure 
char password[100]; 
char new_password[100]; 
printf("Set your initial password:\n"); 
while (1) { 
printf("Enter password: "); 
scanf("%s", password); 
// Try to set the password 
if (setPassword(&user, password)) { 
break; // Successfully set password 
} 
} 
// Simulate checking password expiration 
if (isPasswordExpired(&user)) { 
printf("Your password has expired. Please change it.\n"); 
} 
// Authentication simulation 
printf("Authenticate with your password:\n"); 
printf("Enter password: "); 
scanf("%s", password); 
authenticateUser(&user, password); 
// Change password after expiration (for simulation) 
printf("\nChange your password:\n"); 
while (1) { 
printf("Enter new password: "); 
scanf("%s", new_password); 
// Try to set the new password 
if (setPassword(&user, new_password)) { 
break; // Successfully set new password 
} 
} 
return 0; 
} 