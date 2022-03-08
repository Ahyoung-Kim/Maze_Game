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

//콘솔 내부의 특정 위치로 커서를 이동시키는 함수 
void Move(int x, int y) {
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//콘솔 창 세팅하는 함수
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
	cout << "'s' key를 누르면 시작합니다.";
	Move(18, 14);
	cout << "'q' key를 누르면 종료합니다.";
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
	cout << "미로의 너비를 입력하세요: ";
	cin >> W;
	Move(18, 6);
	cout << "미로의 높이를 입력하세요: ";
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


//엘러 알고리즘으로 완전 미로 만들기
void MadeMaze_Eller() {
	int i, j, k;

	for (i = 0; i < H; i++) { //한 행씩 진행
		int remove_wall; //벽 제거할지 결정하는 flag

		for (j = 0; j < W - 1; j++) { //수직 벽 제거

			//마지막 행일 때, 서로 다른 방 번호 가진 벽들 모두 제거 
			if (i == H - 1) {
				remove_wall = 1;
			}
			else { //마지막 행이 아니라면, 수직 벽 제거 임의로 결정
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

		//수평 벽 제거

		int cnt = 0; //현재 행에서 같은 집합 속 방의 개수 
		int prev = numSet[i][0];
		int num_start = 0;

		for (j = 0; j < W; j++) {
			if (prev == numSet[i][j]) {
				cnt++; //같은 집합 속 방 개수 증가

				if (prev == numSet[i][j + 1]) //다음 방과도 같은 집합이면 continue
					continue;
				else {
					int remove_idx = rand() % cnt; //수평 벽 제거할 방 임의로 선택
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
				case 72: //위
					return 0;
					break;
				case 80: //아래
					return 1;
					break;
				case 75: //왼쪽
					return 2;
					break;
				case 77: //오른쪽
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
			cout << "미로 탈출!" << endl;
			return score;
		}

		int key = PressKey();

		int tx = temp.x + dx[key];
		int ty = temp.y + dy[key];


		if (tx < 0 || tx >= W || ty < 0 || ty >= H) {
			cout << "움직일 수 없습니다. 방향키를 다시 입력하세요." << endl;
			continue;
		}
		if (map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '-' ||
			map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '|') {
			cout << "움직일 수 없습니다. 방향키를 다시 입력하세요." << endl;
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

	SetConsole(); //콘솔 창 세팅

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