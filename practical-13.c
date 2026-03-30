//Password Policy Enforcement System

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MIN_PASSWORD_LENGTH 8
#define PASSWORD_EXPIRY_DAYS 30
#define MAX_HISTORY 5

// Structure
struct User {
    char current_password[100];
    char password_history[MAX_HISTORY][100];
    time_t last_changed;
};

// Length check
int checkPasswordLength(const char *password) {
    return strlen(password) >= MIN_PASSWORD_LENGTH;
}

// Complexity check
int checkPasswordComplexity(const char *password) {
    int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0;

    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
        if (isdigit(password[i])) has_digit = 1;
        if (ispunct(password[i])) has_special = 1;
    }

    return has_upper && has_lower && has_digit && has_special;
}

// History check
int checkPasswordHistory(const char *password, struct User *user) {
    for (int i = 0; i < MAX_HISTORY; i++) {
        if (strcmp(password, user->password_history[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Update history
void updatePasswordHistory(struct User *user, const char *new_password) {
    for (int i = MAX_HISTORY - 1; i > 0; i--) {
        strcpy(user->password_history[i], user->password_history[i - 1]);
    }
    strcpy(user->password_history[0], new_password);
}

// Expiry check
int isPasswordExpired(struct User *user) {
    time_t now = time(NULL);
    double days = difftime(now, user->last_changed) / (60 * 60 * 24);
    return days > PASSWORD_EXPIRY_DAYS;
}

// Set password
int setPassword(struct User *user, const char *new_password) {

    if (!checkPasswordLength(new_password)) {
        printf("Error: Password must be at least %d characters long.\n", MIN_PASSWORD_LENGTH);
        return 0;
    }

    if (!checkPasswordComplexity(new_password)) {
        printf("Error: Password must contain uppercase, lowercase, digit and special character.\n");
        return 0;
    }

    if (checkPasswordHistory(new_password, user)) {
        printf("Error: Cannot reuse old passwords.\n");
        return 0;
    }

    updatePasswordHistory(user, new_password);
    strcpy(user->current_password, new_password);
    user->last_changed = time(NULL);

    printf("Password successfully changed.\n");
    return 1;
}

// Authentication
int authenticateUser(struct User *user, const char *password) {
    if (strcmp(user->current_password, password) == 0) {
        printf("Authentication successful.\n");
        return 1;
    } else {
        printf("Authentication failed.\n");
        return 0;
    }
}

int main() {
    struct User user;
    memset(&user, 0, sizeof(user));  // Safe initialization

    char password[100];
    char new_password[100];

    printf("Set your initial password:\n");

    while (1) {
        printf("Enter password: ");
        scanf("%99s", password);

        if (setPassword(&user, password)) {
            break;
        }
    }

    // Expiry check
    if (isPasswordExpired(&user)) {
        printf("Password expired.\n");
    }

    // Authentication
    printf("\nLogin:\n");
    printf("Enter password: ");
    scanf("%99s", password);
    authenticateUser(&user, password);

    // Change password
    printf("\nChange password:\n");

    while (1) {
        printf("Enter new password: ");
        scanf("%99s", new_password);

        if (setPassword(&user, new_password)) {
            break;
        }
    }

    return 0;
}