#include <stdbool.h>
#include <lib/string.h>

/*
 * str_contains_char - checks if a string contains char
 */
bool str_contains_char(const char* s, const char c) {
    while (*s) {
        if (*s == c)
            return true;
        s++;
    }
    return false;
}

/*
 * str_shift_left - shifts a string left `shift` times
 */
void str_shift_left(char *str, const int shift) {
    int i = 0;
    
    /* Move the pointer `shift` positions to the left */
    while (str[i + shift] != '\0') {
        str[i] = str[i + shift];
        i++;
    }
    
    /* Null-terminate the new string */
    str[i] = '\0';
}

/*
 * char_to_int - converts a decimal character to an integer
 */
uint8_t char_to_int(const char c) {
    return c - '0';
}