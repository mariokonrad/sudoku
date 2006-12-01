// sudokusolve.cpp
//
// Mario Konrad
// 2006-05-30

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

enum { NS = 3, N = NS*NS };

typedef int Element;
typedef Element Field[N][N];

static void print(const Field g)
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

static void swap(Field & g, int x, int y)
{
	Element tmp = g[x][y];
	g[x][y] = g[y][x];
	g[y][x] = tmp;
}

static void transpose(Field & g)
{
	for (int y = 0; y < N; ++y)
		for (int x = 0; x < N; ++x)
			swap(g, x, y);
}

static bool row_contains(const Field g, int row, int ignore_col, Element e)
{
	for (int col = 0; col < N; ++col)
		if ((col != ignore_col) && (g[row][col] == e)) return true;
	return false;
}

static bool col_contains(const Field g, int ignore_row, int col, Element e)
{
	for (int row = 0; row < N; ++row)
		if ((row != ignore_row) && (g[row][col] == e)) return true;
	return false;
}

static bool sub_contains(const Field g, int row, int col, Element e)
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

static bool check(const Field f)
{
	for (int row = 0; row < N; ++row) {
		for (int col = 0; col < N; ++col) {
			if (f[row][col] == 0) continue;
			if (col_contains(f, row, col, f[row][col])) return false;
			if (row_contains(f, row, col, f[row][col])) return false;
			if (sub_contains(f, row, col, f[row][col])) return false;
		}
	}
	return true;
}

static void randomize(Element e[], size_t n)
{
	srand(time(0)); // seed

	size_t a;
	size_t b;
	for (size_t i = 100 + (rand() % 500); i; --i) {
		a = rand() % n;
		b = rand() % n;
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

			// prepare elements
			Element e[N];
			Element elem = 1;
			for (size_t i = 0; i < N; ++i, ++elem) e[i] = elem;
			randomize(e, N);

			// try elements
			bool solution = false;
			for (size_t i = 0; !solution && (i < N); ++i) {
				g[y][x] = e[i];

				// check
				if (   row_contains(g, y, x, e[i])
				    || col_contains(g, y, x, e[i])
				    || sub_contains(g, y, x, e[i])
				    ) {
					solution = false;
				} else {
					// if not found, search deeper
					solution = solve(g);
				}
			}

			// check for (sub)solution
			if (!solution) {
				g[y][x] = 0; // then restore
				return false; // still found!
			}
		}
	}
	return true; // no duplicates found, might be a good solution
}

int main(int, char **)
{
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

	transpose(f); // needed because of the human readable initialization

	if (!check(f)) {
		printf("Error in entry field. exit.\n");
		return -1;
	}

	print(f);
	solve(f);
	printf("\n");
	print(f);

	return 0;
}

