#include <stdio.h> // Standard I/O Library
#include <time.h> // For measurement
#include <pthread.h> // For message passing & computation

#include <mpi.h> // OpenMPI
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

int sequential(size_t len, char *str) {
	// Execute
	clock_t start = clock();
	int res = search(str, len, -128, 127);
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
	if (argc >= 2) { len = (size_t) atoi(argv[1]); } else { len = 4; }
	char *str;
	if (argc >= 3) { str = create_string(argv[2], len); } else { str = create_string("", len); }
	if (!str) { return -1; } // Null check
	if (argc >= 4 && atoi(argv[3]) == 1) { res = sequential(len, str); }
	else {
		int world_rank, world_size;
		MPI_Init(NULL, NULL);

		MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Scope of the world
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // Scope of me.

		printf("Process %d: Starting up.\n", world_rank);
		// Calculate amount of work
		int work_range = (int) (256 / world_size);
		int work_rest = (int) (256 % world_size); // World size always above 0 (or program doesn't run lol)
		int a = -128 + (work_range * world_rank), b = (a + work_range - 1); // Create a range offset from -128 to 127. Process (1 / 3) -> -128 - -42

		// Correction for off-by-n
		//printf("p: %d | 2p: %d\n", (work_rest) + (work_rest * world_rank), 2 * ((work_rest) + (work_rest * world_rank)));
		if ((2 * (work_rest + (work_rest * world_rank)) >= world_size)) {
			b++;
		}

		double start, stop;
		MPI_Barrier(MPI_COMM_WORLD); // Sync
		printf("Process %d: Checking range %d to %d\n", world_rank, a, b);
		start = MPI_Wtime();
		res = search(str, len, a, b); // Actual search
		if (res == 0) {
			stop = MPI_Wtime();
			printf("Process %d: The string | %s | found in %f seconds.\n", world_rank, str, (stop - start));
			MPI_Abort(MPI_COMM_WORLD, res);
		}
		stop = MPI_Wtime();
		printf("Process %d: String not found in range %d - %d in %f seconds\n", world_rank, a, b, (stop - start));
		MPI_Barrier(MPI_COMM_WORLD); // Sync

		MPI_Finalize();
		//save_result(len, str, (stop - start));
	}

	
	// Cleanup
	destroy_string(str);
	return res;
}