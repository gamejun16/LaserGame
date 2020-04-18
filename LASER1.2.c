#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>


/// 함수 원형 ///
void gotoxy(int x, int y) { //gotoxy함수 
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

/// 변수 ///

int char_xpos = 13, char_ypos = 13; // 플레이어 좌표
int laser_xpos, laser_ypos; // 레이저의 좌표
int laser_num[19]; // 1~18 18개의 레이저 총 9단계
int laser_safe = 1, erase_safe = 0, aim_safe = 1; //안전장치
int difficulty = 0; // 난이도 n = 레이저 n*2개. 중복검사 x, 최대치를 가리킴
int max_difficulty = 3;
int key;
int laser_on = 0, menu_on = 0;
int score = 0;
int menu_choice;
int intro_return; // INTRO_MENU()에서 반환 된 값을 저장할 변수
int go_intro = 0; // pause. 3번 탭에서 intro로 돌아가기위한 변수.
int smash_check = 0; // 사망시 intro로 돌아가기위한 변수

int stage = 0; // 플레이 속도와 관련. 1~5 총 5개의 스테이지가 존재
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

	else { // intro_return 1일때만 들어올 수 있음
		system("cls"); //intro 화면을 지우고 시작
		setting();
		field(); // 필드 출력
		gotoxy(char_xpos, char_ypos); printf("◎"); // 캐릭터

		while (1) {
			character(); // 메뉴 선택 함수 포함
			if (go_intro == 1) { go_intro = 0;  goto start; }
			if (menu_on == 1) continue;
			if (laser_on == 1) smash(); // 충돌검사
			if (smash_check == 1) { smash_check = 0; goto start; }
			// 레이저 발사 //
			if (clock() % ms_time == sub_ms_time && laser_safe == 1) { laser_on = 1; laser_safe = 0; draw_laser(); }

			if (clock() % ms_time == sub_ms_time + 50) { laser_safe = 1; erase_safe = 1; }

			// 레이저 삭제 //
			if (clock() % ms_time == 0 && erase_safe == 1) {
				laser_on = 0;
				erase_safe = 0; erase_laser();

				left_count--; // 다음스테이지로 넘어가는 카운트
				score++;

				if (left_count == 0 && stage < 5) { next_stage(); continue; }
				field();
				if (difficulty < max_difficulty) difficulty++;

				// 레이저 조준 //
				choose_laser();
				aim_laser();
			}
		}
	}
	return 0;
}
///MAIN END///



/// 함수 ///

int INTRO_MENU(void)
{
	int i, intro_choice = 1;
	gotoxy(3, 3); for (i = 21; i > 0; i--) printf("▩");
	gotoxy(3, 23); for (i = 21; i > 0; i--) printf("▩");
	for (i = 4; i < 23; i++) { gotoxy(2, i); printf("▩"); gotoxy(24, i); printf("▩"); }
	gotoxy(2, 3); printf("┏"); gotoxy(24, 3); printf("┓");
	gotoxy(2, 23); printf("┗"); gotoxy(24, 23); printf("┛");

	gotoxy(3, 2); printf("1.2 ver");

	gotoxy(4, 5); printf("■　　　　■　　■■■　■■■　■■");
	gotoxy(4, 6); printf("■　　　■　■　■　　　■　　　■　■");
	gotoxy(4, 7); printf("■　　　■■■　■■■　■■■　■■");
	gotoxy(4, 8); printf("■　　　■　■　　　■　■　　　■　■");
	gotoxy(4, 9); printf("■■■　■　■　■■■　■■■　■　■");

	gotoxy(11, 15); printf("START GAME");
	gotoxy(12, 17); printf("MENUAL");
	gotoxy(12, 19); printf(" EXIT");

	while (1) {
		if (intro_choice == 1) { gotoxy(9, 15); printf("▶"); }
		else if (intro_choice == 2) { gotoxy(10, 17); printf("▶"); }
		else if (intro_choice == 3) { gotoxy(10, 19); printf(" ▶"); }
		key = _getch();
		gotoxy(9, 15); printf("　");
		gotoxy(10, 17); printf("　");
		gotoxy(10, 19); printf(" 　");
		if (key == 72) {
			if (intro_choice == 1) intro_choice += 2;
			else
				intro_choice--;
		} // 위

		else if (key == 80) {
			if (intro_choice == 3) intro_choice -= 2;
			else
				intro_choice++;
		}// 아래

		else if (key == 32 || key == 13) {
			return intro_choice;
		} // SPACE or ENTER


	}
}

void MENUAL(void)
{
	int menual_exit = '|'; // menual함수를 빠져나가기위한 변수. (_kbhit()의 오류를 개선하기위함)
	system("cls");

	gotoxy(25, 3); printf("╆━━━━━╅");
	gotoxy(25, 4); printf("┃　 STAGE　┃");
	gotoxy(25, 5); printf("┃　　　　　┃");
	gotoxy(25, 6); printf("╄━━━━━╃");

	gotoxy(25, 8); printf("╆━━━━━╅");
	gotoxy(25, 9); printf("┃　　　　　┃");
	gotoxy(25, 10); printf("┃　　　　　┃");
	gotoxy(25, 11); printf("╄━━━━━╃");
	gotoxy(27, 9); printf(" COUNT");

	gotoxy(25, 13); printf("╆━━━━━╅");
	gotoxy(25, 14); printf("┃　 SCORE  ┃");
	gotoxy(25, 15); printf("┃　　　　　┃");
	gotoxy(25, 16); printf("╄━━━━━╃");

	gotoxy(25, 18); printf("╆━━━━━╅");
	gotoxy(25, 19); printf("┃ INTERVAL ┃");
	gotoxy(25, 20); printf("┃　　　(ms)┃");
	gotoxy(25, 21); printf("┃　　　(s) ┃");
	gotoxy(25, 22); printf("╄━━━━━╃");

	gotoxy(26, 2); printf(" ↓ UI탭");
	gotoxy(2, 3); printf("【 게임 설명 】");
	gotoxy(2, 4); printf("랜덤으로 발사되는 최대 18개의 레이저를 피해");
	gotoxy(2, 5); printf("오래 버티는 게임 입니다.");
	gotoxy(2, 7); printf("【 UI탭 설명 】");
	gotoxy(2, 8); printf("「STAGE」");
	gotoxy(2, 9); printf("현재 스테이지를 보여줍니다.");
	gotoxy(2, 10); printf("5 스테이지까지 존재하며 스테이지별로");
	gotoxy(2, 11); printf("레이저가 발사되는 간격과 갯수가 변화합니다.");
	gotoxy(2, 13); printf("「COUNT」");
	gotoxy(2, 14); printf("0이 될 때마다 다음 스테이지로 넘어갑니다.");
	gotoxy(2, 16); printf("「SCORE」");
	gotoxy(2, 17); printf("레이저를 피한 횟수를 기록합니다.");
	gotoxy(2, 19); printf("「INTERVAL」");
	gotoxy(2, 20); printf("조준 후 발사되기 까지의 시간을 보여줍니다.");

	while (1) {
		if (clock() % 2000 == 1000) { gotoxy(4, 22); printf("▶▶ENTER를 누르십시오▶▶"); }
		else if (clock() % 2000 == 0) { gotoxy(4, 22); printf("　　　　 　　 　　　　　　　"); }
		if (_kbhit()) { getchar(); break; }
	}
	system("cls");

	//ui
	gotoxy(25, 3); printf("╆━━━━━╅");
	gotoxy(25, 4); printf("┃　 STAGE　┃");
	gotoxy(25, 5); printf("┃　　　　　┃");
	gotoxy(25, 6); printf("╄━━━━━╃");

	gotoxy(25, 8); printf("╆━━━━━╅");
	gotoxy(25, 9); printf("┃　　　　　┃");
	gotoxy(25, 10); printf("┃　　　　　┃");
	gotoxy(25, 11); printf("╄━━━━━╃");
	gotoxy(27, 9); printf(" COUNT");

	gotoxy(25, 13); printf("╆━━━━━╅");
	gotoxy(25, 14); printf("┃　 SCORE  ┃");
	gotoxy(25, 15); printf("┃　　　　　┃");
	gotoxy(25, 16); printf("╄━━━━━╃");

	gotoxy(25, 18); printf("╆━━━━━╅");
	gotoxy(25, 19); printf("┃ INTERVAL ┃");
	gotoxy(25, 20); printf("┃　　　(ms)┃");
	gotoxy(25, 21); printf("┃　　　(s) ┃");
	gotoxy(25, 22); printf("╄━━━━━╃");

	gotoxy(2, 3); printf("【 조작 방법 】");
	gotoxy(2, 4); printf("캐릭터(◎)는 방향키로 움직일 수 있습니다.");
	gotoxy(2, 5); printf("플레이 중 'P'를 누르면 일시정지됩니다.");
	gotoxy(2, 7); printf("제 작 자  : 진민준");
	gotoxy(2, 8); printf("현재 버전 : 1.2");
	gotoxy(2, 10); printf("【 패치 노트 】");
	gotoxy(2, 11); printf("죽은 후 간혹 ENTER를 여러 차례 입력해야 하던");
	gotoxy(2, 12); printf("버그를 수정함.");
	gotoxy(2, 13); printf("맵의 가장자리에서 한 칸 더 움직일 경우 반대");
	gotoxy(2, 14); printf("쪽으로 이동하게끔 수정함.");
	gotoxy(2, 15); printf("INTERVAL의 표기 방식을 알맞게 수정함.");
	gotoxy(2, 17); printf("【 수정해야 할 사항 】");
	gotoxy(2, 18); printf("MANUAL 두번째 페이지가 자동으로 넘어가는 오류.");
	gotoxy(2, 19); printf("STAGE가 넘어간 직후 첫 번째 레이저가 조준 없이");
	gotoxy(2, 20); printf("곧바로 발사되는 오류");

	Sleep(500); // 0.5초 짧은 딜레이

	while (1) {
		if (clock() % 2000 == 1000) { gotoxy(4, 22); printf("▶▶ENTER를 누르십시오▶▶"); }
		else if (clock() % 2000 == 0) { gotoxy(4, 22); printf("　　      　　      　 　　　　　　　"); }
		if (_kbhit()) { getchar(); break; }
	}
}

void setting(void)
{
	if (stage<6) stage++; // 1~최대 6까지 증가
	left_count = 10;
	switch (stage) {
	case 1:
		ms_time = 2500; // 레이저와 레이저 사이의 텀 ( 지워지고, 조준 후 발사까지 )
		sub_ms_time = 1500; // 조준 후 레이저가 발사되기 까지의 시간
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

	/// 게임 화면 ///
	gotoxy(4, 3); for (i = 19; i > 0; i--) printf("━");
	gotoxy(4, 23); for (i = 19; i > 0; i--) printf("━");
	for (i = 4; i < 23; i++) { gotoxy(3, i); printf("┃"); gotoxy(23, i); printf("┃"); }
	gotoxy(3, 3); printf("┏"); gotoxy(23, 3); printf("┓");
	gotoxy(3, 23); printf("┗"); gotoxy(23, 23); printf("┛");

	/// U I 탭 ///
	gotoxy(25, 3); printf("╆━━━━━╅");
	gotoxy(25, 4); printf("┃　 STAGE　┃");
	gotoxy(25, 5); printf("┃　　　　　┃");
	gotoxy(25, 6); printf("╄━━━━━╃");

	gotoxy(25, 8); printf("╆━━━━━╅");
	gotoxy(25, 9); printf("┃　　　　　┃");
	gotoxy(25, 10); printf("┃　　　　　┃");
	gotoxy(25, 11); printf("╄━━━━━╃");
	gotoxy(27, 9); printf(" COUNT");

	gotoxy(25, 13); printf("╆━━━━━╅");
	gotoxy(25, 14); printf("┃　 SCORE  ┃");
	gotoxy(25, 15); printf("┃　　　　　┃");
	gotoxy(25, 16); printf("╄━━━━━╃");

	gotoxy(25, 18); printf("╆━━━━━╅");
	gotoxy(25, 19); printf("┃ INTERVAL ┃");
	gotoxy(25, 20); printf("┃　　　(ms)┃");
	gotoxy(25, 21); printf("┃　　　(s) ┃");
	gotoxy(25, 22); printf("╄━━━━━╃");

	gotoxy(4, 2); printf("Press 'P' = PAUSE");

	field_data();
}

void field_data(void) // field()로 생성된 화면의 UI탭에 각종 수치를 채워 넣는 함수
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

	gotoxy(char_xpos, char_ypos); printf("　"); // 플레이어 삭제
	gotoxy(11, 13); printf("NEXT STAGE"); Sleep(2000); // 안내 문구 출력 ( 2초 딜레이 )
	gotoxy(11, 13); printf("          ");
	setting(); field(); gotoxy(char_xpos, char_ypos); printf("◎");
	choose_laser();
	aim_laser();
}

void character(void)
{
	// 플레이어 좌푯값 설정 //
	if (_kbhit()) {
		key = _getch();

		// 플레이어 삭제 //
		if (key == 72 || key == 80 || key == 75 || key == 77) {
			gotoxy(char_xpos, char_ypos); printf("　");
		}
		else if (key == 'p' || key == 'P') menu_choice = pause(); // p혹은 P를 입력시 pause() 함수 호출

		if (menu_choice == 1) { // Player want Back to Game
			menu_choice = -787; // 나올 수 없는 임의의 값.

			gotoxy(9, 11); printf("　　　　　　　　　　");
			gotoxy(9, 12); printf("　　　　　　　　　　");
			gotoxy(9, 13); printf("　　　　　　　　　　");
			gotoxy(9, 14); printf("　　　　　　　　　　");
			gotoxy(9, 15); printf("　　　　　　　　　　");
			gotoxy(10, 16); printf("            "); // 메뉴 삭제

			gotoxy(12, 13); printf(" READY");
			Sleep(2000); // 2초간 딜레이
			gotoxy(12, 13); printf("      ");

		}

		else if (menu_choice == 2) { // Player want NEW_GAME
			menu_choice = -787; // 나올 수 없는 임의의 값
			left_count = 10;
			score = 0; // 점수 초기화
			char_xpos = 13; char_ypos = 13; // Player 좌표 초기화
			difficulty = 0; // 난이도 초기화 (= 레이저의 개수와 관련)
			max_difficulty = 3; // 레이저 갯수 제한
			stage = 0; // 스테이지 초기화
			laser_safe = 1; erase_safe = 0; aim_safe = 1; // 안전장치 설정을 초기 설정으로 변경
			setting();

			system("cls");

			field(); // 필드 출력
			gotoxy(char_xpos, char_ypos); printf("◎"); // 캐릭터 출력
			gotoxy(12, 13); printf(" READY");
			Sleep(2000); // 2초간 딜레이
			gotoxy(12, 13); printf("      ");
		}

		else if (menu_choice == 3) { // Player want go Intro_Page
			menu_choice = -787; // 나올 수 없는 임의의 값
			left_count = 10;
			score = 0; // 점수 초기화
			char_xpos = 13; char_ypos = 13; // Player 좌표 초기화
			difficulty = 0; // 난이도 초기화 (= 레이저의 개수와 관련)
			max_difficulty = 3; // 레이저 갯수 제한
			stage = 0; // 스테이지 초기화
			laser_safe = 1; erase_safe = 0; aim_safe = 1; // 안전장치 설정을 초기 설정으로 변경
			go_intro = 1; // intro로 돌아가기 위한 함수
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

		// 플레이어 출력 //
		if (menu_on != 1) { gotoxy(char_xpos, char_ypos); printf("◎"); }
	}


}

int pause(void)
{
	int pause_key;
	menu_on = 1;
	gotoxy(9, 11); printf("▨▨──MENU──▨▨");
	gotoxy(9, 12); printf("▨ 1. BACK　　　　▨");
	gotoxy(9, 13); printf("▨ 2. NEW_GAME　　▨");
	gotoxy(9, 14); printf("▨ 3. QUIT_GAME　 ▨");
	gotoxy(9, 15); printf("▨▨▨▨▨▨▨▨▨▨");
	gotoxy(10, 16); printf("SELECT : ");
	scanf("%d", &pause_key);

	if (pause_key == 1) { menu_on = 0; return 1; } // BACK
	else if (pause_key == 2) { menu_on = 0; return 2; } // NEW_GAME
	else if (pause_key == 3) { menu_on = 0; return 3; } // EXIT_GAME
}

void draw_laser(void) // difficulty 1
{
	int i, j, z;

	/// 레이저를 그려냄 ///
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19번 레이저 ( 세로 )
			for (z = 4; z < 23; z++) { gotoxy(laser_num[i] + 3, z); printf("│"); }
		}

		else { // 20~38번 레이저 ( 가로 )
			gotoxy(4, laser_num[i] - 19 + 3); printf("───────────────────");
		}
	}
}

void erase_laser(void)
{
	int i, j, z;
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19번 레이저 ( 세로 )
			for (z = 4; z < 23; z++) { gotoxy(laser_num[i] + 3, z); printf("　"); }
		}
		else { // 20~38번 레이저 ( 가로 )
			gotoxy(4, laser_num[i] - 19 + 3); printf("　　　　　　　　　　　　　　　　　　　");
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
		if (laser_num[i] < 20) { // 1~19번 레이저 ( 세로 )
			gotoxy(laser_num[i] + 3, 3);  printf("┳");
			gotoxy(laser_num[i] + 3, 23); printf("┻");
		}
		else { // 20~38번 레이저 ( 가로 )
			gotoxy(3, laser_num[i] - 19 + 3); printf("┣");
			gotoxy(23, laser_num[i] - 19 + 3); printf("┫");
		}
	}
}

void smash(void) // 충돌검사
{
	int i, j;
	for (i = 1, j = 2; i <= j*difficulty; i++) {
		if (laser_num[i] < 20) { // 1~19번 레이저 ( 세로 )
			if (char_xpos == laser_num[i] + 3) {
				gotoxy(13, 13);
				smash_view();
			}
		}
		else { // 20~38번 레이저 ( 가로 )
			if (char_ypos == laser_num[i] - 19 + 3) {
				gotoxy(13, 13);
				smash_view();
			}
		}
	}
}

void smash_view(void)
{

	int exit_died_view; // died 창을 탈출하기위한 변수
	gotoxy(9, 11); printf("▨▨─YOU_DIED─▨▨");
	gotoxy(9, 12); printf("▨   SCORE :      ▨");
	gotoxy(9, 13); printf("▨   PUSH ENTER   ▨");
	gotoxy(9, 14); printf("▨▨▨▨▨▨▨▨▨▨");
	gotoxy(15, 12); printf("%3d", score);

	//PRESS:

	while (1) {
		exit_died_view = _getch();
		if (exit_died_view == 13) {
			smash_check = 1;
			left_count = 10;
			score = 0; // 점수 초기화
			char_xpos = 13; char_ypos = 13; // Player 좌표 초기화
			difficulty = 0; // 난이도 초기화 (= 레이저의 개수와 관련)
			stage = 0; // 스테이지 초기화
			laser_safe = 1; erase_safe = 0; aim_safe = 1; // 안전장치 설정을 초기 설정으로 변경
			break;
		}
	}
	//else { getchar(); goto PRESS; } // smash_view 내부에 존재하는 PRESS: 로 이동
}