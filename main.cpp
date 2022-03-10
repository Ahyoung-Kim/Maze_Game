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

//�ܼ� ������ Ư�� ��ġ�� Ŀ���� �̵���Ű�� �Լ� 
void Move(int x, int y) {
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//�ܼ� â �����ϴ� �Լ�
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
	cout << "'s' key�� ������ �����մϴ�.";
	Move(18, 16);
	cout << "'q' key�� ������ �����մϴ�.";
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
	cout << "====               �ٽ� �����ϰڽ��ϱ�?                 ===";
	Move(10, 20);
	cout << "===========================================================";
	Move(20, 24);
	cout << "'s' key: �� �� ��!";
	Move(20, 26);
	cout << "'q' key: ����";
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
	cout << "< 2 �̻�, 15������ ������ �Է��� �ּ���. >";
	Move(18, 16);
	cout << "�̷��� �ʺ� �Է��ϼ���: ";
	cin >> W;
	Move(18, 18);
	cout << "�̷��� ���̸� �Է��ϼ���: ";
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
			if (map[temp.y * 2 + 1 + dy[i]][temp.x * 2 + 1 + dx[i]] != ' ') //�� �� ���̿� ���� ������ continue
				continue;
			//if (numSet[ty][tx] == temp.num) //�̹� ������ continue
				//continue;

			numSet[ty][tx] = temp.num;
			visit[ty][tx] = true;
			s.push({ tx, ty, temp.num });
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

				/*memset(visit, false, sizeof(visit));
				for (int y = 0; y <= i; y++) {
					for (int x = 0; x < W; x++) {
						if (!visit[y][x])
							dfs(x, y, numSet[y][x]);
					}
				}*/
			}

		}

		/*cout << "===========���� �� ���� ��==============" << endl;
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

		//���� �� ����

		int cnt = 1; //���� �࿡�� ���� ���� �� ���� ���� 

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
				numSet[i + 1][j] = numSet[i][j]; //���� ����
				map[i * 2 + 2][j * 2 + 1] = ' '; //����
			}
			else {
				int remove_idx = rand() % cnt;
				remove_idx += j; //�� ������ ��ġ 

				numSet[i + 1][remove_idx] = numSet[i][remove_idx];
				map[i * 2 + 2][remove_idx * 2 + 1] = ' ';

				//�ϳ� ���������� ������ ���� ���Ƿ� ����
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

		/*cout << "===========���� �� ���� ��==============" << endl;
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
	cout << "==   ����: q   ==";
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
	int ex, ey; //������ �����ϰ� ����
	score = 0;
	temp.x = 0;
	temp.y = 0;

	//start point�� �׻� (1, 1)
	visit[0][0] = 1;
	q.push(temp);
	map[1][1] = '*';

	ex = dis_x(gen);
	ey = dis_y(gen);

	map[ey * 2 + 1][ex * 2 + 1] = '#';

	min_path = bfs(ex, ey);
	cout << "�ִ� ��� ����: " << min_path << endl;

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
			cout << "< ������ �� �����ϴ�. ����Ű�� �ٽ� �Է��ϼ���. >";
			continue;
		}
		if (map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '-' ||
			map[temp.y * 2 + 1 + dy[key]][temp.x * 2 + 1 + dx[key]] == '|') {
			Move(12, 3);
			cout << "< ������ �� �����ϴ�. ����Ű�� �ٽ� �Է��ϼ���. >";
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

	SetConsole(); //�ܼ� â ����

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