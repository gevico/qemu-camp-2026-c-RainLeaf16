#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

typedef struct {
    int row;
    int col;
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
	Point queue[MAX_ROW * MAX_COL];
	Point parent[MAX_ROW][MAX_COL];
	int visited[MAX_ROW][MAX_COL] = {0};
	int dr[4] = {1, 0, -1, 0};
	int dc[4] = {0, 1, 0, -1};
	int front = 0;
	int rear = 0;
	int found = 0;

	parent[0][0].row = -1;
	parent[0][0].col = -1;
	queue[rear++] = (Point){0, 0};
	visited[0][0] = 1;

	while (front < rear) {
		Point current = queue[front++];

		if (current.row == MAX_ROW - 1 && current.col == MAX_COL - 1) {
			found = 1;
			break;
		}

		for (int i = 0; i < 4; i++) {
			int next_row = current.row + dr[i];
			int next_col = current.col + dc[i];

			if (next_row < 0 || next_row >= MAX_ROW || next_col < 0 || next_col >= MAX_COL) {
				continue;
			}

			if (maze[next_row][next_col] == 1 || visited[next_row][next_col]) {
				continue;
			}

			visited[next_row][next_col] = 1;
			parent[next_row][next_col] = current;
			queue[rear++] = (Point){next_row, next_col};
		}
	}

	if (!found) {
		printf("No path!\n");
		return 0;
	}

	Point current = (Point){MAX_ROW - 1, MAX_COL - 1};
	while (current.row != -1 && current.col != -1) {
		printf("(%d, %d)\n", current.row, current.col);
		current = parent[current.row][current.col];
	}

	return 0;
}
