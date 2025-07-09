#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
// Fill in the TODO sections as required and DO NOT TOUCH any of the fprintf statements

// Function to create the Bad Character Shift Table
int* bcst_create(const char* pattern, int pattern_len) {
    int* bcst = (int*)malloc(256 * sizeof(int));
    if (bcst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for(int j = 0; j < 256; j++) 
        bcst[j] = pattern_len;

    for (int j = 0; j < pattern_len - 1; j++) 
        bcst[(unsigned char)pattern[j]] = pattern_len - 1 - j;

    return bcst;
}
// Function to create the Good Suffix Shift Table
int* gsst_create(const char* pattern, int pattern_len) {
    int* gsst = (int*)malloc((pattern_len + 1) * sizeof(int));
    if (gsst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int last_prefix_position = pattern_len;
    for (int j = pattern_len; j > 0; j--) {
        if (strncmp(pattern, pattern + j, pattern_len - j) == 0) {
            last_prefix_position = j;
        }
        gsst[pattern_len - j] = last_prefix_position - j + pattern_len;
    }

    for (int j = 0; j < pattern_len - 1; j++) {
        int suffix_length = 0;
        while (suffix_length < j + 1 && pattern[pattern_len - 1 - suffix_length] == pattern[j - suffix_length]) {
            suffix_length++;
        }
        gsst[suffix_length] = pattern_len - 1 - j + suffix_length;
    }

    return gsst;
}
// Boyer-Moore search function
int boyer_moore(const char* text, const char* pattern, int* bcst, int* gsst, FILE* output_file) {
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    int comparisons = 0;
    int pos = 0;

    fprintf(output_file, "Occurrences:");
    
    while (pos <= text_len - pattern_len) {
        int j = pattern_len - 1;
        
        while (j >= 0 && pattern[j] == text[pos + j]) {
            j--;
            comparisons++;
        }

        if (j < 0) { 
            fprintf(output_file, "%d,", pos);
            pos += gsst[1];  
        } else { 
            int bad_char_shift = bcst[(unsigned char)text[pos + j]];
            int good_suffix_shift = gsst[pattern_len - j];
            pos += (bad_char_shift > good_suffix_shift) ? bad_char_shift : good_suffix_shift;
            comparisons++;
        }
    }

    fprintf(output_file, "\n");
    return comparisons;
}


void testcase(FILE* values_file, FILE* input_file, FILE* output_file) {
    char text[2000];
    char pattern[500];
    fscanf(input_file, "%s", text);
    fscanf(input_file, "%s", pattern);

    int pattern_len = strlen(pattern);
    int* bcst = bcst_create(pattern, pattern_len);
    fprintf(output_file, "BCST:\n");
    for (int j = 0; j < 26; j++) {
        fprintf(output_file, "%c:%d, ", (char)(j + 'a'), bcst[j]);
    }
    fprintf(output_file, "\n");

    int* gsst = gsst_create(pattern, pattern_len);
    fprintf(output_file, "GSST:\n");
    for (int j = 1; j <= pattern_len; j++) {
        fprintf(output_file, "%d:%d, ", j, gsst[j]);
    }
    fprintf(output_file, "\n");

    clock_t start = clock();
    int comparisons = boyer_moore(text, pattern, bcst, gsst, output_file);
    fprintf(output_file, "Comparisons:%d\n\n", comparisons);

    free(bcst);
    free(gsst);

    clock_t end = clock();
    int elapse = (int)(((double)(end - start)) / CLOCKS_PER_SEC * 1000000000); 
    fprintf(values_file, "%d,%ld,%d,%d\n", pattern_len, strlen(text), comparisons, elapse);
}


int main() {
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("boyermoore_output.txt", "w");
    FILE *values_file = fopen("boyermoore_values.txt", "w");

    if (!input_file || !output_file || !values_file) {
        printf("Error opening file!\n");
        return 1;
    }

    int testcases;
    fscanf(input_file, "%d", &testcases);
    int count = 0;
    fprintf(values_file, "patternlen,textlen,cmp,timetaken\n");
    
    while (count < testcases) {
        testcase(values_file, input_file, output_file);
        count++;
    }

    fclose(input_file);
    fclose(output_file);
    fclose(values_file);
    
    return 0;
}
