#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>


/// �Լ� ���� ///
void gotoxy(int x, int y) { //gotoxy�Լ� 
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setting(void);
void field(void);
void character(void);
void aim_laser(void);
void draw_laser(void);
void choose_laser(void);
void erase_laser(void);
void smash(void);
void next_stage(void);
void field_data(void);
int pause(void);
int INTRO_MENU(void);
void MENUAL(void);
void smash_view(void);

//void draw_cross(void);

/// ���� ///

int char_xpos = 13, char_ypos = 13; // �÷��̾� ��ǥ
int laser_xpos, laser_ypos; // �������� ��ǥ
int laser_num[19]; // 1~18 18���� ������ �� 9�ܰ�
int laser_safe = 1, erase_safe = 0, aim_safe = 1; //������ġ
int difficulty = 0; // ���̵� n = ������ n*2��. �ߺ��˻� x, �ִ�ġ�� ����Ŵ
int max_difficulty = 3;
int key;
int laser_on = 0, menu_on = 0;
int score = 0;
int menu_choice;
int intro_return; // INTRO_MENU()���� ��ȯ �� ���� ������ ����
int go_intro = 0; // pause. 3�� �ǿ��� intro�� ���ư������� ����.
int smash_check = 0; // ����� intro�� ���ư������� ����

int stage = 0; // �÷��� �ӵ��� ����. 1~5 �� 5���� ���������� ����
int left_count;


int ms_time;
int sub_ms_time;




/// MAIN START///
int main(void)
{
start:
	system("cls");
	intro_return = INTRO_MENU();
	if (intro_return == 2) { MENUAL(); system("cls");  intro_return = -737; goto start; } // MENUAL
	else if (intro_return == 3) { gotoxy(3, 2); return 0; } // EXIT

	else { // intro_return 1�϶��� ���� �� ����
		system("cls"); //intro ȭ���� ����� ����
		setting();
		field(); // �ʵ� ���
		gotoxy(char_xpos, char_ypos); printf("��"); // ĳ����

		while (1) {
			character(); // �޴� ���� �Լ� ����
			if (go_intro == 1) { go_intro = 0;  goto start; }
			if (menu_on == 1) continue;
			if (laser_on == 1) smash(); // �浹�˻�
			if (smash_check == 1) { smash_check = 0; goto start; }
			// ������ �߻� //
			if (clock() % ms_time == sub_ms_time && laser_safe == 1) { laser_on = 1; laser_safe = 0; draw_laser(); }

			if (clock() % ms_time == sub_ms_time + 50) { laser_safe = 1; erase_safe = 1; }

			// ������ ���� //
			if (clock() % ms_time == 0 && erase_safe == 1) {
				laser_on = 0;
				erase_safe = 0; erase_laser();

				left_count--; // �������������� �Ѿ�� ī��Ʈ
				score++;

				if (left_count == 0 && stage < 5) { next_stage(); continue; }
				field();
				if (difficulty < max_difficulty) difficulty++;

				// ������ ���� //
				choose_laser();
				aim_laser();
			}
		}
	}
	return 0;
}
///MAIN END///



/// �Լ� ///

int INTRO_MENU(void)
{
	int i, intro_choice = 1;
	gotoxy(3, 3); for (i = 21; i > 0; i--) printf("��");
	gotoxy(3, 23); for (i = 21; i > 0; i--) printf("��");
	for (i = 4; i < 23; i++) { gotoxy(2, i); printf("��"); gotoxy(24, i); printf("��"); }
	gotoxy(2, 3); printf("��"); gotoxy(24, 3); printf("��");
	gotoxy(2, 23); printf("��"); gotoxy(24, 23); printf("��");

	gotoxy(3, 2); printf("1.2 ver");

	gotoxy(4, 5); printf("�ᡡ�������ᡡ�����ᡡ���ᡡ���");
	gotoxy(4, 6); printf("�ᡡ�����ᡡ�ᡡ�ᡡ�����ᡡ�����ᡡ��");
	gotoxy(4, 7); printf("�ᡡ�������ᡡ���ᡡ���ᡡ���");
	gotoxy(4, 8); printf("�ᡡ�����ᡡ�ᡡ�����ᡡ�ᡡ�����ᡡ��");
	gotoxy(4, 9); printf("���ᡡ�ᡡ�ᡡ���ᡡ���ᡡ�ᡡ��");

	gotoxy(11, 15); printf("START GAME");
	gotoxy(12, 17); printf("MENUAL");
	gotoxy(12, 19); printf(" EXIT");

	while (1) {
		if (intro_choice == 1) { gotoxy(9, 15); printf("��"); }
		else if (intro_choice == 2) { gotoxy(10, 17); printf("��"); }
		else if (intro_choice == 3) { gotoxy(10, 19); printf(" ��"); }
		key = _getch();
		gotoxy(9, 15); printf("��");
		gotoxy(10, 17); printf("��");
		gotoxy(10, 19); printf(" ��");
		if (key == 72) {
			if (intro_choice == 1) intro_choice += 2;
			else
				intro_choice--;
		} // ��

		else if (key == 80) {
			if (intro_choice == 3) intro_choice -= 2;
			else
				intro_choice++;
		}// �Ʒ�

		else if (key == 32 || key == 13) {
			return intro_choice;
		} // SPACE or ENTER


	}
}

void MENUAL(void)
{
	int menual_exit = '|'; // menual�Լ��� �������������� ����. (_kbhit()�� ������ �����ϱ�����)
	system("cls");

	gotoxy(25, 3); printf("�ব����������");
	gotoxy(25, 4); printf("���� STAGE����");
	gotoxy(25, 5); printf("��������������");
	gotoxy(25, 6); printf("�ަ�����������");

	gotoxy(25, 8); printf("�ব����������");
	gotoxy(25, 9); printf("��������������");
	gotoxy(25, 10); printf("��������������");
	gotoxy(25, 11); printf("�ަ�����������");
	gotoxy(27, 9); printf(" COUNT");

	gotoxy(25, 13); printf("�ব����������");
	gotoxy(25, 14); printf("���� SCORE  ��");
	gotoxy(25, 15); printf("��������������");
	gotoxy(25, 16); printf("�ަ�����������");

	gotoxy(25, 18); printf("�ব����������");
	gotoxy(25, 19); printf("�� INTERVAL ��");
	gotoxy(25, 20); printf("��������(ms)��");
	gotoxy(25, 21); printf("��������(s) ��");
	gotoxy(25, 22); printf("�ަ�����������");

	gotoxy(26, 2); printf(" �� UI��");
	gotoxy(2, 3); printf("�� ���� ���� ��");
	gotoxy(2, 4); printf("�������� �߻�Ǵ� �ִ� 18���� �������� ����");
	gotoxy(2, 5); printf("���� ��Ƽ�� ���� �Դϴ�.");
	gotoxy(2, 7); printf("�� UI�� ���� ��");
	gotoxy(2, 8); printf("��STAGE��");
	gotoxy(2, 9); printf("���� ���������� �����ݴϴ�.");
	gotoxy(2, 10); printf("5 ������������ �����ϸ� ������������");
	gotoxy(2, 11); printf("�������� �߻�Ǵ� ���ݰ� ������ ��ȭ�մϴ�.");
	gotoxy(2, 13); printf("��COUNT��");
	gotoxy(2, 14); printf("0�� �� ������ ���� ���������� �Ѿ�ϴ�.");
	gotoxy(2, 16); printf("��SCORE��");
	gotoxy(2, 17); printf("�������� ���� Ƚ���� ����մϴ�.");
	gotoxy(2, 19); printf("��INTERVAL��");
	gotoxy(2, 20); printf("���� �� �߻�Ǳ� ������ �ð��� �����ݴϴ�.");

	while (1) {
		if (clock() % 2000 == 1000) { gotoxy(4, 22); printf("����ENTER�� �����ʽÿ�����"); }
		else if (clock() % 2000 == 0) { gotoxy(4, 22); printf("�������� ���� ��������������"); }
		if (_kbhit()) { getchar(); break; }
	}
	system("cls");

	//ui
	gotoxy(25, 3); printf("�ব����������");
	gotoxy(25, 4); printf("���� STAGE����");
	gotoxy(25, 5); printf("��������������");
	gotoxy(25, 6); printf("�ަ�����������");

	gotoxy(25, 8); printf("�ব����������");
	gotoxy(25, 9); printf("��������������");
	gotoxy(25, 10); printf("��������������");
	gotoxy(25, 11); printf("�ަ�����������");
	gotoxy(27, 9); printf(" COUNT");

	gotoxy(25, 13); printf("�ব����������");
	gotoxy(25, 14); printf("���� SCORE  ��");
	gotoxy(25, 15); printf("��������������");
	gotoxy(25, 16); printf("�ަ�����������");

	gotoxy(25, 18); printf("�ব����������");
	gotoxy(25, 19); printf("�� INTERVAL ��");
	gotoxy(25, 20); printf("��������(ms)��");
	gotoxy(25, 21); printf("��������(s) ��");
	gotoxy(25, 22); printf("�ަ�����������");

	gotoxy(2, 3); printf("�� ���� ��� ��");
	gotoxy(2, 4); printf("ĳ����(��)�� ����Ű�� ������ �� �ֽ��ϴ�.");
	gotoxy(2, 5); printf("�÷��� �� 'P'�� ������ �Ͻ������˴ϴ�.");
	gotoxy(2, 7); printf("�� �� ��  : ������");
	gotoxy(2, 8); printf("���� ���� : 1.2");
	gotoxy(2, 10); printf("�� ��ġ ��Ʈ ��");
	gotoxy(2, 11); printf("���� �� ��Ȥ ENTER�� ���� ���� �Է��ؾ� �ϴ�");
	gotoxy(2, 12); printf("���׸� ������.");
	gotoxy(2, 13); printf("���� �����ڸ����� �� ĭ �� ������ ��� �ݴ�");
	gotoxy(2, 14); printf("������ �̵��ϰԲ� ������.");
	gotoxy(2, 15); printf("INTERVAL�� ǥ�� ����� �˸°� ������.");
	gotoxy(2, 17); printf("�� �����ؾ� �� ���� ��");
	gotoxy(2, 18); printf("MANUAL �ι�° �������� �ڵ����� �Ѿ�� ����.");
	gotoxy(2, 19); printf("STAGE�� �Ѿ ���� ù ��° �������� ���� ����");
	gotoxy(2, 20); printf("��ٷ� �߻�Ǵ� ����");

	Sleep(500); // 0.5�� ª�� ������

	while (1) {
		if (clock() % 2000 == 1000) { gotoxy(4, 22); printf("����ENTER�� �����ʽÿ�����"); }
		else if (clock() % 2000 == 0) { gotoxy(4, 22); printf("����      ����      �� ��������������"); }
		if (_kbhit()) { getchar(); break; }
	}
}

void setting(void)
{
	if (stage<6) stage++; // 1~�ִ� 6���� ����
	left_count = 10;
	switch (stage) {
	case 1:
		ms_time = 2500; // �������� ������ ������ �� ( ��������, ���� �� �߻���� )
		sub_ms_time = 1500; // ���� �� �������� �߻�Ǳ� ������ �ð�
		break;
	case 2:
		ms_time = 2300;
		sub_ms_time = 1300;
		break;
	case 3:
		ms_time = 2100;
		sub_ms_time = 1100;
		break;
	case 4:
		ms_time = 1900;
		sub_ms_time = 900;
		break;
	case 5:
		ms_time = 1700;
		sub_ms_time = 700;
		break;
	}


}

void field(void)
{
	int i;

	/// ���� ȭ�� ///
	gotoxy(4, 3); for (i = 19; i > 0; i--) printf("��");
	gotoxy(4, 23); for (i = 19; i > 0; i--) printf("��");
	for (i = 4; i < 23; i++) { gotoxy(3, i); printf("��"); gotoxy(23, i); printf("��"); }
	gotoxy(3, 3); printf("��"); gotoxy(23, 3); printf("��");
	gotoxy(3, 23); printf("��"); gotoxy(23, 23); printf("��");

	/// U I �� ///
	gotoxy(25, 3); printf("�ব����������");
	gotoxy(25, 4); printf("���� STAGE����");
	gotoxy(25, 5); printf("��������������");
	gotoxy(25, 6); printf("�ަ�����������");

	gotoxy(25, 8); printf("�ব����������");
	gotoxy(25, 9); printf("��������������");
	gotoxy(25, 10); printf("��������������");
	gotoxy(25, 11); printf("�ަ�����������");
	gotoxy(27, 9); printf(" COUNT");

	gotoxy(25, 13); printf("�ব����������");
	gotoxy(25, 14); printf("���� SCORE  ��");
	gotoxy(25, 15); printf("��������������");
	gotoxy(25, 16); printf("�ަ�����������");

	gotoxy(25, 18); printf("�ব����������");
	gotoxy(25, 19); printf("�� INTERVAL ��");
	gotoxy(25, 20); printf("��������(ms)��");
	gotoxy(25, 21); printf("��������(s) ��");
	gotoxy(25, 22); printf("�ަ�����������");

	gotoxy(4, 2); printf("Press 'P' = PAUSE");

	field_data();
}

void field_data(void) // field()�� ������ ȭ���� UI�ǿ� ���� ��ġ�� ä�� �ִ� �Լ�
{
	gotoxy(28, 5); if (stage != 5) printf(" %d", stage);
	else if (stage == 5) printf("\b\b\bMAX STAGE");

	gotoxy(28, 10);
	if (stage == 5) printf("\b\bNoCount");
	else if (left_count == 10) printf("%d", left_count);
	else printf(" %d", left_count);

	gotoxy(28, 15); printf("%3d", score);

	gotoxy(27, 20); printf("%4d", sub_ms_time);
	gotoxy(27, 21); printf("%4.1f", (sub_ms_time / 1000.0));
}

void next_stage(void)
{
	if (max_difficulty < 9) max_difficulty += 2; // 3 5 7 9 9

	gotoxy(char_xpos, char_ypos); printf("��"); // �÷��̾� ����
	gotoxy(11, 13); printf("NEXT STAGE"); Sleep(2000); // �ȳ� ���� ��� ( 2�� ������ )
	gotoxy(11, 13); printf("          ");
	setting(); field(); gotoxy(char_xpos, char_ypos); printf("��");
	choose_laser();
	aim_laser();
}

void character(void)
{
	// �÷��̾� ��ǩ�� ���� //
	if (_kbhit()) {
		key = _getch();

		// �÷��̾� ���� //
		if (key == 72 || key == 80 || key == 75 || key == 77) {
			gotoxy(char_xpos, char_ypos); printf("��");
		}
		else if (key == 'p' || key == 'P') menu_choice = pause(); // pȤ�� P�� �Է½� pause() �Լ� ȣ��

		if (menu_choice == 1) { // Player want Back to Game
			menu_choice = -787; // ���� �� ���� ������ ��.

			gotoxy(9, 11); printf("��������������������");
			gotoxy(9, 12); printf("��������������������");
			gotoxy(9, 13); printf("��������������������");
			gotoxy(9, 14); printf("��������������������");
			gotoxy(9, 15); printf("��������������������");
			gotoxy(10, 16); printf("            "); // �޴� ����

			gotoxy(12, 13); printf(" READY");
			Sleep(2000); // 2�ʰ� ������
			gotoxy(12, 13); printf("      ");

		}

		else if (menu_choice == 2) { // Player want NEW_GAME
			menu_choice = -787; // ���� �� ���� ������ ��
			left_count = 10;
			score = 0; // ���� �ʱ�ȭ
			char_xpos = 13; char_ypos = 13; // Player ��ǥ �ʱ�ȭ
			difficulty = 0; // ���̵� �ʱ�ȭ (= �������� ������ ����)
			max_difficulty = 3; // ������ ���� ����
			stage = 0; // �������� �ʱ�ȭ
			laser_safe = 1; erase_safe = 0; aim_safe = 1; // ������ġ ������ �ʱ� �������� ����
			setting();

			system("cls");

			field(); // �ʵ� ���
			gotoxy(char_xpos, char_ypos); printf("��"); // ĳ���� ���
			gotoxy(12, 13); printf(" READY");
			Sleep(2000); // 2�ʰ� ������
			gotoxy(12, 13); printf("      ");
		}

		else if (menu_choice == 3) { // Player want go Intro_Page
			menu_choice = -787; // ���� �� ���� ������ ��
			left_count = 10;
			score = 0; // ���� �ʱ�ȭ
			char_xpos = 13; char_ypos = 13; // Player ��ǥ �ʱ�ȭ
			difficulty = 0; // ���̵� �ʱ�ȭ (= �������� ������ ����)
			max_difficulty = 3; // ������ ���� ����
			stage = 0; // �������� �ʱ�ȭ
			laser_safe = 1; erase_safe = 0; aim_safe = 1; // ������ġ ������ �ʱ� �������� ����
			go_intro = 1; // intro�� ���ư��� ���� �Լ�
		}


		if (key == 72) {
			if(char_ypos != 4) char_ypos--;
			else char_ypos = 22;
		}
		else if (key == 80) {
			if(char_ypos != 22) char_ypos++;
			else char_ypos = 4;
		}
		else if (key == 75) {
			if(char_xpos != 4) char_xpos--;
			else char_xpos = 22;
		}
		else if (key == 77) {
			if(char_xpos != 22) char_xpos++;
			else char_xpos = 4;
		}

		// �÷��̾� ��� //
		if (menu_on != 1) { gotoxy(char_xpos, char_ypos); printf("��"); }
	}


}

int pause(void)
{
	int pause_key;
	menu_on = 1;
	gotoxy(9, 11); printf("�ɢɦ���MENU�����ɢ�");
	gotoxy(9, 12); printf("�� 1. BACK����������");
	gotoxy(9, 13); printf("�� 2. NEW_GAME������");
	gotoxy(9, 14); printf("�� 3. QUIT_GAME�� ��");
	gotoxy(9, 15); printf("�ɢɢɢɢɢɢɢɢɢ�");
	gotoxy(10, 16); printf("SELECT : ");
	scanf("%d", &pause_key);

	if (pause_key == 1) { menu_on = 0; return 1; } // BACK
	else if (pause_key == 2) { menu_on = 0; return 2; } // NEW_GAME
	else if (pause_key == 3) { menu_on = 0; return 3; } // EXIT_GAME
}

void draw_laser(void) // difficulty 1
{
	int i, j, z;

	/// �������� �׷��� ///
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19�� ������ ( ���� )
			for (z = 4; z < 23; z++) { gotoxy(laser_num[i] + 3, z); printf("��"); }
		}

		else { // 20~38�� ������ ( ���� )
			gotoxy(4, laser_num[i] - 19 + 3); printf("��������������������������������������");
		}
	}
}

void erase_laser(void)
{
	int i, j, z;
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19�� ������ ( ���� )
			for (z = 4; z < 23; z++) { gotoxy(laser_num[i] + 3, z); printf("��"); }
		}
		else { // 20~38�� ������ ( ���� )
			gotoxy(4, laser_num[i] - 19 + 3); printf("��������������������������������������");
		}
	}
}

void choose_laser(void)
{
	srand((unsigned)time(NULL));

	switch (difficulty) {
	case 9:
		laser_num[18] = rand() % 38 + 1;
		laser_num[17] = rand() % 38 + 1;
	case 8:
		laser_num[16] = rand() % 38 + 1;
		laser_num[15] = rand() % 38 + 1;
	case 7:
		laser_num[14] = rand() % 38 + 1;
		laser_num[13] = rand() % 38 + 1;
	case 6:
		laser_num[12] = rand() % 38 + 1;
		laser_num[11] = rand() % 38 + 1;
	case 5:
		laser_num[10] = rand() % 38 + 1; // 1~38
		laser_num[9] = rand() % 38 + 1;
	case 4:
		laser_num[8] = rand() % 38 + 1;
		laser_num[7] = rand() % 38 + 1;
	case 3:
		laser_num[6] = rand() % 38 + 1;
		laser_num[5] = rand() % 38 + 1;
	case 2:
		laser_num[4] = rand() % 38 + 1;
		laser_num[3] = rand() % 38 + 1;
	case 1:
		laser_num[2] = rand() % 38 + 1;
		laser_num[1] = rand() % 38 + 1;
		break;
	}
}

void aim_laser(void)
{
	int i, j;
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19�� ������ ( ���� )
			gotoxy(laser_num[i] + 3, 3);  printf("��");
			gotoxy(laser_num[i] + 3, 23); printf("��");
		}
		else { // 20~38�� ������ ( ���� )
			gotoxy(3, laser_num[i] - 19 + 3); printf("��");
			gotoxy(23, laser_num[i] - 19 + 3); printf("��");
		}
	}
}

void smash(void) // �浹�˻�
{
	int i, j;
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19�� ������ ( ���� )
			if (char_xpos == laser_num[i] + 3) {
				gotoxy(13, 13);
				smash_view();
			}
		}
		else { // 20~38�� ������ ( ���� )
			if (char_ypos == laser_num[i] - 19 + 3) {
				gotoxy(13, 13);
				smash_view();
			}
		}
	}
}

void smash_view(void)
{

	int exit_died_view; // died â�� Ż���ϱ����� ����
	gotoxy(9, 11); printf("�ɢɦ�YOU_DIED���ɢ�");
	gotoxy(9, 12); printf("��   SCORE :      ��");
	gotoxy(9, 13); printf("��   PUSH ENTER   ��");
	gotoxy(9, 14); printf("�ɢɢɢɢɢɢɢɢɢ�");
	gotoxy(15, 12); printf("%3d", score);

	//PRESS:

	while (1) {
		exit_died_view = _getch();
		if (exit_died_view == 13) {
			smash_check = 1;
			left_count = 10;
			score = 0; // ���� �ʱ�ȭ
			char_xpos = 13; char_ypos = 13; // Player ��ǥ �ʱ�ȭ
			difficulty = 0; // ���̵� �ʱ�ȭ (= �������� ������ ����)
			stage = 0; // �������� �ʱ�ȭ
			laser_safe = 1; erase_safe = 0; aim_safe = 1; // ������ġ ������ �ʱ� �������� ����
			break;
		}
	}
	//else { getchar(); goto PRESS; } // smash_view ���ο� �����ϴ� PRESS: �� �̵�
}