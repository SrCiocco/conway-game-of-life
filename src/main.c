/* Rules:
	1. Any live cell with fewer than two live neighbours dies, as if by underpopulation. 1 -> 0, si n < 2.
	2. Any live cell with two or three live neighbours lives on to the next generation. 1 -> 1, si n >= 2 ^ n < 4.
	3. Any live cell with more than three live neighbours dies, as if by overpopulation. 1 -> 0, si n > 3.
	4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction. 0 -> 1, si n = 3.
*/
#include <stdio.h>
#include <stdlib.h> /* srand(), rand(). */
#include <time.h> /* time(). */
#include <unistd.h> /* sleep(). */

#define SIZE 100
#define ALIVE 1
#define DEAD 0
#define ALIVE_ICON "·" // Bloque sólido cian
#define DEAD_ICON  " " // Punto tenue para el "vacío"
#define WAIT 40000
#define CLEAR system("clear");

void populate_matrix(int matrix[SIZE][SIZE]);
int count_neighbors(int matrix[SIZE][SIZE], int row, int col);
void print_matrix(int matrix[SIZE][SIZE]);
void update_matrix(int currentMatrix[SIZE][SIZE], int nextMatrix[SIZE][SIZE]);

int main()
{
	srand(time(NULL));

	int matrix[SIZE][SIZE];
	int matrixNew[SIZE][SIZE];

	int (*pActual)[SIZE] = matrix;
	int (*pNext)[SIZE] = matrixNew;
	int (*tmp)[SIZE];

	populate_matrix(pActual);

	while (1) {
		CLEAR;
		print_matrix(pActual);
		update_matrix(pActual, pNext);
		tmp = pActual;
		pActual = pNext;
		pNext = tmp;
		usleep(WAIT);
	}
	return 0;
}

void populate_matrix(int matrix[SIZE][SIZE])
{
	int state;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			state = rand() % 2;
			matrix[i][j] = state;
		}
	}
}

void print_matrix(int matrix[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			(matrix[i][j] == ALIVE ) ? printf("%s", ALIVE_ICON) : printf("%s", DEAD_ICON);
		}
		printf("\n");
	}
}

int count_neighbors(int matrix[SIZE][SIZE], int row, int col)
{
	int neighbors = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue; /* We dont count ourselves as a neighbor. */

			int r = row + i;
			int c = col + j;

			if (r >= 0 && r < SIZE && c >= 0 && c < SIZE) neighbors += matrix[r][c];
		}
	}
	return neighbors;
}

void update_matrix(int currentMatrix[SIZE][SIZE], int nextMatrix[SIZE][SIZE])
{
	int state;
	int neighbors;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			neighbors = count_neighbors(currentMatrix, i, j);
			state = currentMatrix[i][j];
			
			if (state == ALIVE) {
				if (neighbors < 2 || neighbors > 3) nextMatrix[i][j] = DEAD;
				else nextMatrix[i][j] = ALIVE;

			} else {
				if (neighbors == 3) nextMatrix[i][j] = ALIVE;
				else nextMatrix[i][j] = DEAD;
			}
		}
	}
}
