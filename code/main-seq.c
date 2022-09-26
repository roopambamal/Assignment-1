#include <stdio.h> // Standard I/O Library
#include <time.h> // For measurement
#include <pthread.h> // For message passing & computation

#include "crackme.h" // The task.

#define DEBUG 1
#define min(a, b) a < b ? a : b

static void print_debug(char *string) {
	if (DEBUG) { printf("%s\n", string); }
}

// Create a string from memory, setting all bits to 0 and copying from previous string if any.
static char *create_string(char *pre_str, size_t len) {
	char *str = malloc(sizeof(char) * len + 1); // Allocate
	if (!str) { return NULL; } // NULLPTR check
	memset((void *) str, 0, sizeof(char) * len + 1); // MEMSET to 0
	if (pre_str) { memccpy((void *) str, (void *) pre_str, sizeof(char), len); }
	return str; // Give string.
}

// Resize an active string, providing the same pointer if possible or moving it if not.
static char *resize_string(char *string, size_t new_len) {
	return (char *) realloc((void *) string, sizeof(char) * new_len);
}

// If we allocate, we must unallocate.
static void destroy_string(char *string) {
	free((void *) string);
	return;
}

// Opens solution.txt and appends a line.
static int save_result(int len, char *str, double time) {
	FILE *f = fopen("solution.txt", "a");
	if (!f) { return -1; }
	int res = fprintf(f, "%d %s %f\n", len, str, time);
	fclose(f);
	return res;
}

/**
 * @brief Recursive search on the string using the p function.
 * Each loop of the function identifies it's position and runs a subsequent recursion on the next position.
 * Thereby the string checks each range of letters from A - B for it's position and all subsequent position.
 * The search is brute force, O(N) for the range.
 * Each new letter increases the complexity by 255^len, which takes time.
 * 
 * @param str 
 * @param len 
 * @param pos 
 * @param a 
 * @param b 
 * @return int 
 */
static int _search(char *str, size_t len, int pos, int a, int b) {
	int res = 1;
	for (char c = (char) a; c < b; c++) {
		if (pos < len - 1) { 
			res = _search(str, len, pos + 1, -128, 127);
			if (res == 0) { return res; }
		}
		*(str+pos) = (char) c;
		//printf("Testing: %s\n", str);
		res = p(len, str);
		if (res == 0) { return res; }
	}
	return res;
}

/**
 * @brief Sets up a recursive function that brute forces each segment of a string to test.
 * It does so by taking a range from char A to B to test on the string.
 * 
 * @param str 
 * @param len 
 * @param a 
 * @param b 
 * @return int 
 */
int search(char *str, size_t len, int a, int b) {
	int res = 1;
	res = _search(str, len, 0, a, b);
	return res;
}

// Sets up a result, a string and does a loop for the length, attempting to identify the p string by brute force.
int main(int argc, char *argv[]) {
	printf("Arguments:\n");
	for (int i = 0; i < argc; i++) {
		printf("%d : %s\n", i, argv[i]);
	}
	// Setup
	size_t len;
	int res;
	if (argc >= 2) { len = (size_t) atoi(argv[1]); } else { len = 2; }
	char *str;
	if (argc >= 3) { str = create_string(argv[2], len); } else { str = create_string("", len); }
	if (!str) { return -1; } // Null check

	// Execute
	clock_t start = clock();
	res = search(str, len, -128, 127);
	clock_t stop = clock();

	// Report
	double time_spent = (double)(stop - start) / CLOCKS_PER_SEC;
	if (res == 0) {
		printf("String: %s found in %f seconds.\n", str, time_spent);
	}
	else {
		printf("String not found in %f seconds...\n", time_spent);
	}
	//save_result(len, str, time_spent);

	// Cleanup
	destroy_string(str);
	return res;
}