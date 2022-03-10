#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<vector>
#include<stack>
#include<queue>
#include<cstdlib>
#include<random>
#include<time.h>
#include<random>

using namespace std;

int W, H;
char map[50][50];
char path[50][50];
bool visit[20][20];
int numSet[20][20];
int score;
int min_path;

int dx[4] = { 0, 0, 1, -1 };
int dy[4] = { 1, -1, 0, 0 };

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

	system("mode con:cols=100 lines=45");
	system("title [MazeGame] by. Ahyoung-Kim");
}

void DrawGame() {

	system("cls");
	Move(10, 6);
	cout << "===========================================================";
	Move(10, 8);
	cout << "=========================Maze Game=========================";
	Move(10, 10);
	cout << "===========================================================";
	Move(18, 14);
	cout << "'s' key를 누르면 시작합니다.";
	Move(18, 16);
	cout << "'q' key를 누르면 종료합니다.";
	Move(21, 37);
	cout << "by. Ahyoung-Kim" << endl;

}

void ReGame() {
	system("cls");
	Move(19, 6);
	cout << "AI: " << min_path << "    your score: " << score;
	Move(17, 8);
	cout << "================================";
	Move(17, 10);
	if (min_path < score)
		cout << "==           LOSE!            ==";
	else
		cout << "==            WIN!            ==";
	Move(17, 12);
	cout << "================================";

	Move(10, 16);
	cout << "===========================================================";
	Move(10, 18);
	cout << "====               다시 시작하겠습니까?                 ===";
	Move(10, 20);
	cout << "===========================================================";
	Move(20, 24);
	cout << "'s' key: 한 번 더!";
	Move(20, 26);
	cout << "'q' key: 종료";
}

bool SetGame() {
	int key;
	//DrawGame();
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
	Move(15, 12);
	cout << "< 2 이상, 15이하인 정수를 입력해 주세요. >";
	Move(18, 16);
	cout << "미로의 너비를 입력하세요: ";
	cin >> W;
	Move(18, 18);
	cout << "미로의 높이를 입력하세요: ";
	cin >> H;
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

void dfs(int x, int y, int num) {

	visit[y][x] = true;
	stack<node> s;
	s.push({ x, y, num });

	while (!s.empty()) {
		node temp = s.top();
		s.pop();

		for (int i = 0; i < 4; i++) {
			int tx = temp.x + dx[i];
			int ty = temp.y + dy[i];

			if (tx < 0 || tx >= W || ty < 0 || ty >= H)
				continue;
			if (visit[tx][ty])
				continue;
			if (map[temp.y * 2 + 1 + dy[i]][temp.x * 2 + 1 + dx[i]] != ' ') //두 방 사이에 벽이 있으면 continue
				continue;
			//if (numSet[ty][tx] == temp.num) //이미 같으면 continue
				//continue;

			numSet[ty][tx] = temp.num;
			visit[ty][tx] = true;
			s.push({ tx, ty, temp.num });
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

				/*memset(visit, false, sizeof(visit));
				for (int y = 0; y <= i; y++) {
					for (int x = 0; x < W; x++) {
						if (!visit[y][x])
							dfs(x, y, numSet[y][x]);
					}
				}*/
			}

		}

		/*cout << "===========수직 벽 제거 후==============" << endl;
		for (int i = 0; i < H * 2 + 1; i++) {
			for (int j = 0; j < W * 2 + 1; j++) {
				if (i % 2 == 1 && j % 2 == 1) {
					cout << numSet[(i - 1) / 2][(j - 1) / 2];
				}
				else {
					cout << map[i][j];
				}
			}
			cout << endl;
		}*/

		if (i == H - 1)
			break;

		//수평 벽 제거

		int cnt = 1; //현재 행에서 같은 집합 속 방의 개수 

		for (j = 0; j < W; j += cnt) {
			cnt = 1;

			if (j < W - 1) {
				for (k = j + 1; k < W; k++) {
					if (numSet[i][j] != numSet[i][k])
						break;
					cnt++;
				}
			}

			if (cnt == 1) {
				numSet[i + 1][j] = numSet[i][j]; //집합 리셋
				map[i * 2 + 2][j * 2 + 1] = ' '; //제거
			}
			else {
				int remove_idx = rand() % cnt;
				remove_idx += j; //벽 제거할 위치 

				numSet[i + 1][remove_idx] = numSet[i][remove_idx];
				map[i * 2 + 2][remove_idx * 2 + 1] = ' ';

				//하나 제거했으면 나머지 벽은 임의로 제거
				for (int s = j; s < j + cnt; s++) {
					if (s != remove_idx) {
						remove_wall = rand() % 2;

						if (remove_wall == 1) {
							numSet[i + 1][s] = numSet[i][s];
							map[i * 2 + 2][s * 2 + 1] = ' ';
						}
					}
				}
			}

		}

		/*cout << "===========수평 벽 제거 후==============" << endl;
		for (int i = 0; i < H * 2 + 1; i++) {
			for (int j = 0; j < W * 2 + 1; j++) {
				if (i%2==1 && j%2==1) {
					cout << numSet[(i - 1) / 2][(j - 1) / 2];
				}
				else {
					cout << map[i][j];
				}
			}
			cout << endl;
		}
		cout << "===============================================" << endl;

		/*memset(visit, false, sizeof(visit));
		for (int y = 0; y <= i + 1; y++) {
			for (int x = 0; x < W; x++) {
				if (!visit[y][x])
					dfs(x, y, numSet[y][x]);
			}
		}*/
	}
}

void PrintMaze() {

	system("cls");

	Move(40, 2);
	cout << "=================";
	Move(40, 3);
	cout << "==   종료: q   ==";
	Move(40, 4);
	cout << "=================";

	Move(38, 8);
	cout << "===================";
	Move(38, 10);
	cout << "==   score: " << score + 1 << "   ==";
	Move(38, 12);
	cout << "===================";


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

			if (key == 'Q' || key == 'q') {
				return -1;
			}

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

int bfs(int ex, int ey) {
	
	memset(visit, false, sizeof(visit));
	queue<node> q;

	int ans = W * H;

	q.push({ 0, 0, 0 });
	visit[0][0] = true;

	while (!q.empty()) {
		node temp = q.front();
		q.pop();

		if (temp.x == ex && temp.y == ey) {
			ans = min(ans, temp.num + 1);
		}

		for (int i = 0; i < 4; i++) {
			int tx = temp.x + dx[i];
			int ty = temp.y + dy[i];

			if (tx < 0 || tx >= W || ty < 0 || ty >= W)
				continue;
			if (visit[ty][tx])
				continue;
			if (map[temp.y * 2 + 1 + dy[i]][temp.x * 2 + 1 + dx[i]] != ' ')
				continue;

			visit[ty][tx] = true;
			q.push({ tx, ty, temp.num + 1 });
		}

	}
	return ans;

}

bool SearchMaze() {

	//system("cls");
	int dx[4] = { 0, 0, -1, 1 };
	int dy[4] = { -1, 1, 0, 0 };

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis_x(1, W - 1);
	uniform_int_distribution<int> dis_y(1, H - 1);

	queue<node> q;
	node temp;
	int ex, ey; //목적지 랜덤하게 설정
	score = 0;
	temp.x = 0;
	temp.y = 0;

	//start point는 항상 (1, 1)
	visit[0][0] = 1;
	q.push(temp);
	map[1][1] = '*';

	ex = dis_x(gen);
	ey = dis_y(gen);

	map[ey * 2 + 1][ex * 2 + 1] = '#';

	min_path = bfs(ex, ey);
	cout << "최단 경로 길이: " << min_path << endl;

	PrintMaze();

	score++;

	while (!q.empty()) {

		temp = q.front();

		if (temp.x == ex && temp.y == ey) {
			return true;
		}

		int key = PressKey();

		if (key == -1) {
			return false;
		}

		int tx = temp.x + dx[key];
		int ty = temp.y + dy[key];


		if (tx < 0 || tx >= W || ty < 0 || ty >= H) {
			Move(12, 3);
			cout << "< 움직일 수 없습니다. 방향키를 다시 입력하세요. >";
			continue;
		}
		if (map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '-' ||
			map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '|') {
			Move(12, 3);
			cout << "< 움직일 수 없습니다. 방향키를 다시 입력하세요. >";
			continue;
		}

		map[temp.y * 2 + 1][temp.x * 2 + 1] = ' ';
		map[ty * 2 + 1][tx * 2 + 1] = '*';
		PrintMaze();

		score++;
		q.pop();
		q.push({ tx, ty });
	}
}


int main(void) {

	SetConsole(); //콘솔 창 세팅

	srand((unsigned int)time(NULL));
	DrawGame();
	bool start_flag = SetGame();
	bool end_flag = false;

	while (true) {

		if (start_flag) {
			Input();
			InitMaze();

			MadeMaze_Eller();
			PrintMaze();

			end_flag = SearchMaze();

			if (end_flag) {
				ReGame();
				start_flag = SetGame();
			}
			else {
				DrawGame();
				start_flag = SetGame();
			}
		}
		else
			break;
	}


	return 0;
}