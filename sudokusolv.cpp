// sudokusolv.cpp
//
// Mario Konrad
// 2006-05-30

#include <cstdio>
#include <cstdlib>
#include <time.h>

enum { NS = 3, N = NS*NS };

typedef int Element;
typedef Element Field[N][N];

static void print(const Field & g)
{
	printf("+-----------+-----------+---------+\n");
	for (int y = 0; y < N; ++y) {
		printf("|");
		for (int x = 0; x < N; ++x) {
			printf(" %c ", (g[y][x] > 0) ? g[y][x]+'0' : '.');
			if ((x < (N-1)) && ((x % NS) == (NS-1))) printf(" | ");
		}
		printf("|\n");
		if ((y < (N-1)) && ((y % NS) == (NS-1)))
			printf("+----------+-----------+----------+\n");
	}
	printf("+----------+-----------+----------+\n");
}

static bool row_contains(const Field & g, int row, int ignore_col, Element e)
{
	for (int col = 0; col < N; ++col)
		if ((col != ignore_col) && (g[row][col] == e)) return true;
	return false;
}

static bool col_contains(const Field & g, int ignore_row, int col, Element e)
{
	for (int row = 0; row < N; ++row)
		if ((row != ignore_row) && (g[row][col] == e)) return true;
	return false;
}

static bool sub_contains(const Field & g, int row, int col, Element e)
{
	int sx = col - (col % NS);
	int sy = row - (row % NS);
	for (int y = 0; y < NS; ++y) {
		for (int x = 0; x < NS; ++x) {

			// do not check itself
			if (((sy+y) == row) && ((sx+x) == col)) continue;

			// check
			if (g[sy+y][sx+x] == e) return true;
		}
	}
	return false;
}

static bool check_element(const Field & f, int row, int col, Element e)
{
	if (col_contains(f, row, col, e)) return false;
	if (row_contains(f, row, col, e)) return false;
	if (sub_contains(f, row, col, e)) return false;
	return true;
}

static bool check_field(const Field & f)
{
	for (int row = 0; row < N; ++row) {
		for (int col = 0; col < N; ++col) {
			if (f[row][col] == 0) continue;
			if (!check_element(f, row, col, f[row][col])) return false;
		}
	}
	return true;
}

static void random_elements(Element e[], size_t n)
{
	for (size_t i = 0; i < n; ++i) e[i] = i + 1;

	// loop with enough entropy to generate new possible solutions
	// for cells, maybe use of std::next_permutation would be better...
	for (size_t i = (n * 2) + (rand() % (n * 5)); i; --i) {
		size_t a = rand() % n;
		size_t b = rand() % n;
		Element tmp = e[a];
		e[a] = e[b];
		e[b] = tmp;
	}
}

static bool solve(Field & g)
{
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {

			if (g[y][x] != 0) continue; // already occupied?

			// prepare elements, randomization is used to
			// find solutions for arbitrarily defined
			// fields (even empty ones)
			Element e[N];
			random_elements(e, N);

			// try elements
			bool solution = false;
			for (size_t i = 0; !solution && (i < N); ++i) {
				g[y][x] = e[i];

				if (check_element(g, y, x, e[i])) {
					// element not found, solution might be good, search deeper
					solution = solve(g);
				} else {
					solution = false;
				}
			}

			// backtracking: restore state, try another solution
			if (!solution) {
				g[y][x] = 0;
				return false;
			}
		}
	}
	return true; // no duplicates found, might be a good solution
}

int main(int, char **)
{
	srand(time(0));

	Field f = {
		{ 0, 5, 7, 9, 0, 8, 6, 1, 0 },
		{ 0, 0, 0, 0, 0, 7, 0, 9, 4 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 8 },
		{ 0, 7, 6, 5, 3, 0, 0, 8, 1 },
		{ 0, 2, 0, 0, 9, 0, 0, 0, 7 },
		{ 0, 4, 0, 0, 0, 2, 0, 0, 0 },
		{ 6, 0, 0, 0, 8, 0, 5, 4, 3 },
		{ 0, 8, 4, 0, 0, 0, 1, 0, 0 },
		{ 1, 0, 0, 4, 0, 9, 0, 0, 0 },
	};

	if (!check_field(f)) {
		printf("Error in input field. exit.\n");
		return -1;
	}

	print(f);
	solve(f);
	printf("\n");
	print(f);

	return 0;
}

