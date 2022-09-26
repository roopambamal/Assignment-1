#include <stdio.h>

static int compare_char(char *a, char *b) {
	if (*a > *b) {
		return a;
	}
}

int search(char *p, size_t len) {
	char *g = malloc(sizeof(char) * len);
	if (!g) { return -1; }

	for (int i = 0; i < len; i++) {
		
	}

}

int main(int argc, void **argv) {

}