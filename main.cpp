#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<vector>
#include<stack>
#include<queue>
#include<cstdlib>
#include<random>
#include<time.h>

using namespace std;

int W, H;
char** map;
bool** visit;
int** numSet;

typedef struct node {
	int x;
	int y;
	int num;
};

//�ܼ� ������ Ư�� ��ġ�� Ŀ���� �̵���Ű�� �Լ� 
void Move(int x, int y) {
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//�ܼ� â �����ϴ� �Լ�
void SetConsole() {

	system("mode con:cols=100 lines=40");
	system("title [MazeGame] by. Ahyoung-Kim");
}

void DrawGame() {

	system("cls");
	Move(10, 4);
	cout << "===========================================================";
	Move(10, 6);
	cout << "=========================Maze Game=========================";
	Move(10, 8);
	cout << "===========================================================";
	Move(18, 12);
	cout << "'s' key�� ������ �����մϴ�.";
	Move(18, 14);
	cout << "'q' key�� ������ �����մϴ�.";
	Move(21, 35);
	cout << "by. Ahyoung-Kim" << endl;

}

bool SetGame() {
	int key;
	DrawGame();
	while (true) {
		if (_kbhit()) {
			key = _getch();
		}
		else {
			key = 0;
		}

		if (key == 'S' || key == 's') {
			return true;
		}
		else if (key == 'Q' || key == 'q') {
			break;
		}
	}
	return false;
}

void Input() {
	system("cls");
	Move(18, 4);
	cout << "�̷��� �ʺ� �Է��ϼ���: ";
	cin >> W;
	Move(18, 6);
	cout << "�̷��� ���̸� �Է��ϼ���: ";
	cin >> H;
}

void DynamicAllocate() {
	map = new char* [H * 2 + 1];
	visit = new bool* [H];
	numSet = new int* [H];

	for (int i = 0; i < H * 2 + 1; i++) {
		for (int j = 0; j < W * 2 + 1; j++) {
			map[i] = new char[W * 2 + 1];
			if (i < H) {
				visit[i] = new bool[W];
				numSet[i] = new int[W];
			}
		}
	}
}

void InitMaze() {
	int i, j;

	for (i = 0; i < H; i++) {
		for (j = 0; j < W; j++) {
			numSet[i][j] = i * W + j;
			visit[i][j] = false;
		}
	}

	for (i = 0; i < H * 2 + 1; i++) {
		for (j = 0; j < W * 2 + 1; j++) {
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					map[i][j] = '+';
				}
				else {
					map[i][j] = '-';
				}
			}
			else {
				if (j % 2 == 0) {
					map[i][j] = '|';
				}
				else {
					map[i][j] = ' ';
				}
			}
		}
	}
}


//���� �˰������� ���� �̷� �����
void MadeMaze_Eller() {
	int i, j, k;

	for (i = 0; i < H; i++) { //�� �྿ ����
		int remove_wall; //�� �������� �����ϴ� flag

		for (j = 0; j < W - 1; j++) { //���� �� ����

			//������ ���� ��, ���� �ٸ� �� ��ȣ ���� ���� ��� ���� 
			if (i == H - 1) {
				remove_wall = 1;
			}
			else { //������ ���� �ƴ϶��, ���� �� ���� ���Ƿ� ����
				remove_wall = rand() % 2;
			}

			if ((remove_wall == 1) && (numSet[i][j + 1] != numSet[i][j])) {
				numSet[i][j + 1] = numSet[i][j];
				int tx = j * 2 + 2;
				int ty = i * 2 + 1;
				map[ty][tx] = ' ';
			}

		}

		if (i == H - 1)
			break;

		//���� �� ����

		int cnt = 0; //���� �࿡�� ���� ���� �� ���� ���� 
		int prev = numSet[i][0];
		int num_start = 0;

		for (j = 0; j < W; j++) {
			if (prev == numSet[i][j]) {
				cnt++; //���� ���� �� �� ���� ����

				if (prev == numSet[i][j + 1]) //���� ����� ���� �����̸� continue
					continue;
				else {
					int remove_idx = rand() % cnt; //���� �� ������ �� ���Ƿ� ����
					remove_idx += num_start;

					numSet[i + 1][remove_idx] = numSet[i][remove_idx];
					int tx = remove_idx * 2 + 1;
					int ty = i * 2 + 2;
					map[ty][tx] = ' ';

					for (k = 0; k < cnt; k++) {
						k += prev;
						if (k != remove_idx) {
							remove_wall = rand() % 2;

							if (remove_wall == 1) {
								numSet[i + 1][k] = numSet[i][k];
								tx = k * 2 + 1;
								ty = i * 2 + 2;
								map[ty][tx] = ' ';
							}
						}
					}

					prev = numSet[i][j + 1];
					cnt = 0;
					num_start = j + 1;
				}

			}
		}
	}
}

void PrintMaze() {

	//system("cls");
	for (int i = 0; i < H * 2 + 1; i++) {
		Move(20, 10 + i);
		for (int j = 0; j < W * 2 + 1; j++) {
			cout << map[i][j];
		}
		cout << endl;
	}
}

int PressKey() { //1. up 2. down 3. left 4. right
	int key = 0;
	while (true) {
		if (_kbhit()) {
			key = _getch();

			if (key == 224) {
				key = _getch();
				switch (key) {
				case 72: //��
					return 0;
					break;
				case 80: //�Ʒ�
					return 1;
					break;
				case 75: //����
					return 2;
					break;
				case 77: //������
					return 3;
					break;
				default:
					break;
				}
			}
			else
				break;
		}
	}
	return -1;
}

int SearchMaze() {

	system("cls");
	int dx[4] = { 0, 0, -1, 1 };
	int dy[4] = { -1, 1, 0, 0 };

	queue<node> q;
	node temp;
	int score = 0;

	temp.x = 0;
	temp.y = 0;

	visit[0][0] = 1;
	q.push(temp);
	map[1][1] = '*';
	PrintMaze();

	while (!q.empty()) {

		temp = q.front();

		if (temp.x == W - 1 && temp.y == H - 1) {
			cout << "�̷� Ż��!" << endl;
			return score;
		}

		int key = PressKey();

		int tx = temp.x + dx[key];
		int ty = temp.y + dy[key];


		if (tx < 0 || tx >= W || ty < 0 || ty >= H) {
			cout << "������ �� �����ϴ�. ����Ű�� �ٽ� �Է��ϼ���." << endl;
			continue;
		}
		if (map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '-' ||
			map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '|') {
			cout << "������ �� �����ϴ�. ����Ű�� �ٽ� �Է��ϼ���." << endl;
			continue;
		}

		map[temp.y * 2 + 1][temp.x * 2 + 1] = ' ';
		map[ty * 2 + 1][tx * 2 + 1] = '*';
		PrintMaze();

		score++;
		q.pop();
		q.push({ tx, ty });
	}

	return 0;
}

void StartGame() {


}

int main(void) {

	SetConsole(); //�ܼ� â ����

	srand((unsigned int)time(NULL));

	FILE* fp = fopen("maze.maz", "w+t");

	if (fp == NULL) {
		cout << "File Open Error." << endl;
		return 0;
	}

	Input();
	DynamicAllocate();
	InitMaze();

	MadeMaze_Eller();
	PrintMaze();

	int ret = SearchMaze();

	if (ret > 0) {
		cout << "score = " << ret << endl;
	}


	return 0;
}