#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

typedef struct {
    int row;
    int col;
    int next_dir;
} Point;

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

int main(void)
{
	Point stack[MAX_ROW * MAX_COL];
	int top = 0;
	int visited[MAX_ROW][MAX_COL] = {0};
	int dr[4] = {-1, 0, 1, 0};
	int dc[4] = {0, 1, 0, -1};
	int found = 0;

	stack[0].row = 0;
	stack[0].col = 0;
	stack[0].next_dir = 0;
	visited[0][0] = 1;

	while (top >= 0) {
		int row = stack[top].row;
		int col = stack[top].col;

		if (row == MAX_ROW - 1 && col == MAX_COL - 1) {
			found = 1;
			break;
		}

		if (stack[top].next_dir == 4) {
			top--;
			continue;
		}

		int dir = stack[top].next_dir;
		int next_row;
		int next_col;

		stack[top].next_dir++;

		next_row = row + dr[dir];
		next_col = col + dc[dir];

		if (next_row < 0 || next_row >= MAX_ROW || next_col < 0 || next_col >= MAX_COL) {
			continue;
		}

		if (maze[next_row][next_col] == 1 || visited[next_row][next_col]) {
			continue;
		}

		top++;
		stack[top].row = next_row;
		stack[top].col = next_col;
		stack[top].next_dir = 0;
		visited[next_row][next_col] = 1;
	}

	if (!found) {
		printf("No path!\n");
		return 0;
	}

	for (int i = top; i >= 0; i--) {
		printf("(%d, %d)\n", stack[i].row, stack[i].col);
	}

	return 0;
}
