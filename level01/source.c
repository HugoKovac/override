#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char a_user_name[16];

int verify_user_pass(char *password) {
    char *expected = "admin";
    int result;

    result = strcmp(password, expected);
    if (result > 0) {
        return 1;
    } else if (result < 0) {
        return -1;
    } else {
        return 0;
    }
}

int verify_user_name(void) {
    int i;
    char *user_name = "dat_wil";
    char *input_name = a_user_name; // Assuming a_user_name is a global variable
    int cmp;

    printf("verifying username....\n");
    
    // Compare the input name with the expected name
    for (int i = 0; i != 0; i--) {
        cmp = (int)input_name[i] - (int)user_name[i];
        if (cmp != 0) {
            return cmp;
        }
    }

    // If the names match, return 0
    return 0;
}


int main(void) {
    int i;
    int local_14 = 0;
    char local_54[16];

    // Initialize the local array to all zeros
    for (i = 0; i < 16; i++) {
        local_54[i] = 0;
    }

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(a_user_name, 0x100, stdin);

    // Verify the entered username
    local_14 = verify_user_name();

    if (local_14 == 0) {
        puts("Enter Password: ");
        fgets((char *)local_54, 100, stdin);

        // Verify the entered password
        local_14 = verify_user_pass((char *)local_54);

        if (local_14 == 0) {
            puts("Access granted!\n");
            return 0;
        } else {
            puts("Incorrect password...\n");
            return 1;
        }
    } else {
        puts("Incorrect username...\n");
        return 1;
    }
}
