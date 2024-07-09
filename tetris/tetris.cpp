/**
 * 추가할 아이더어 모음
 *초기 아이디어
 * 
 * =알아할점 컨셉: 운으로하는 테트리스 게임판은 가로 12 세로 22이다(외부벽 포함) =
 * 
 * 1: 점수 추가 10점씩 오르게하는게 영상 촬영에 좋을듯(랜덤으로 오르게 하는 아이디어는 보류)
 * 
 * 2: 새 벽돌이 생성될때마다 10%의 확률로 벽돌이 떨어지는 속도가 증가하게한다()
 * 
 * 3: 점수가 100의 배수가 될때마다 랜덤으로 방해공작이 들어온다
 * 
 * 4: 만약 방해공작이 들어오지 않는다면 그림을 출력한다
 * 
 * cxc
 * 
 * 
**/ 




/****************************************************************************
* @mainpage Tetris : 테트리스 게임 개작
* @version 1.0
* @date 1985년
* @author 알렉세이 파지노프 (러시아)
* @brief 고전 테트리스 게임을 개선하여 고급진 테트리스 게임을 만든다.
* @detail 출처 : 혼자 연구하는 C/C++ (http://www.soen.kr/)
*            가.테트리스 http://www.soen.kr/lecture/ccpp/cpp2/14-3-1.htm
*            나.벽돌의 모양 정의 http://www.soen.kr/lecture/ccpp/cpp2/14-3-2.htm
*            다.게임판 http://www.soen.kr/lecture/ccpp/cpp2/14-3-3.htm
*            라.Turboc.h http://soen.kr/lecture/ccpp/cpp1/6-4-4.htm
*이미지 변환 : https://glan.tistory.com/33
*****************************************************************************/

#include "turboc.h"

/* 상수 */
#define LEFT 75		//←키
#define RIGHT 77	//→키
#define UP 72		//↑키
#define DOWN 80		//↓키   
#define ESC 27
#define BX 5		//게임판의 좌상단 좌표 X (외부 벽의 좌상단 좌표 X)
#define BY 1		//게임판의 좌상단 좌표 Y (외부 벽의 좌상단 좌표 Y)
#define BW 10		//게임판의 폭 (외부벽 제외)
#define BH 20		//게임판의 높이 (외부벽 제외)
#define max(a,b) (((a) > (b)) ? (a) : (b))

/* 함수 */
void drawScreen();			//게임의 배경화면인 게임판을 그린다.
void drawBoard();			//게임판의 내부화면을 그린다. (외부벽과 문자열 제외)
void printBrick(bool Show);	//벽돌을 그린다.
bool moveDown();			//벽돌을 좌우, 아래로 움직인다.
bool processKey();			//키를 눌렀을 때 동작을 정의한다.
int getAround(int x, int y, int b, int r);	//벽돌의 주변을 점검하여 빈공간이 있는지 체크한다.
void testFull();			//게임판이 벽돌로 다 찼는지 체크한다.
void update_score(int* score); // 스코어를 랜덤으로 업데이트 합니다.
void random_clear_line(); //랜덤으로 지울 라인을 선택
int random_nFrame(int nFrame); //랜덤으로 속도를 조절한다.
void print_score_nFrame(int score, int nFrame); // 스코어를 출력한다.
void test_score(); //스코어가 1000의 배수인지 확인 후 그림을 출력할지 방해를 할지 정한다.
void random_picture();// 랜덤한 그림을 출력
void print_qwerty(); //기본이미지를 출력
	
/* 구조체 */
typedef struct point
{
	int x, y;
} point;


/* 전역 변수 */
//shape[brick][rot][tile], shape[벽돌번호][회전번호][타일번호]
//기준점(0,0)으로부터의 상대좌표를 지정하는 방식
point shape[][4][4] =
{               
	{ 
		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} },
 		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} } 
	},	//0번 ─
	{ 
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} },
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} }
	},	//1번 ㅁ
	{ 
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} },
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} }
	},	//2번 ┘┌
	{ 
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} },
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} }
	},	//3번 ┐└
	{ 
		{ {0,0},{-1,0},{1,0},{-1,-1} }, { {0,0},{0,-1},{0,1},{-1,1} },
		{ {0,0},{-1,0},{1,0},{1,1} }, { {0,0},{0,-1},{0,1},{1,-1} }
	},	//4번 └─
	{ 
		{ {0,0},{1,0},{-1,0},{1,-1} }, { {0,0},{0,1},{0,-1},{-1,-1} },
		{ {0,0},{1,0},{-1,0},{-1,1} }, { {0,0},{0,-1},{0,1},{1,1} }
	},	//5번 ─┘
	{ 
		{ {0,0},{-1,0},{1,0},{0,1} }, { {0,0},{0,-1},{0,1},{1,0} },
		{ {0,0},{-1,0},{1,0},{0,-1} }, { {0,0},{-1,0},{0,-1},{0,1} }
	}	//6번 ┬
};


enum { EMPTY, BRICK, WALL }; //0-EMPTY-공백, 1-BRICK-다른 벽돌, 2-WALL-벽

// □
const char *arTile[] = { ".","■","벽","z" }; //공간, 벽돌, 벽 모양을 정의하는 문자열 배열, 타일 1개는 2문자로 모양 표시 

int board[BW+2][BH+2]; //전체 게임판의 상태

int nx, ny; //이동중인 벽돌의 배열상의 현재 좌표 (화면상의 좌표 BX+nx*2, BY+ny)
int brick; //이동중인 벽돌의 번호
int rot; //이동중인 벽돌의 회전 모양 번호
int score = 0;

/* main함수 */
int main()

{
	int nFrame, nStay; //벽돌이 내려가는 속도
	int x, y;

	/*게임 초기화*/
	setCursorType(NOCURSOR); //커서 숨김
	srand((unsigned)time(NULL)); //난수생성기 초기화
	clearScreen(); //화면 삭제

	//보드 배열의 바깥쪽을 벽으로 초기화
	for (x=0 ; x<BW+2 ; x++) 
	{
		for (y=0 ; y<BH+2 ; y++) 
		{
			board[x][y] = ( y==0 || y==BH+1 || x==0 || x==BW+1 ) ? WALL : EMPTY;
		}
	}

	nFrame = 10; //벽돌이 떨어지는 속도변수
	drawScreen(); //화면 전체 그리기
	print_qwerty(); //기본 이미지 출력
	while(1)

	{
		/*새 벽돌 생성*/
		nFrame = random_nFrame(nFrame); // 랜덤 속도 변환
		print_score_nFrame(score, nFrame); //점수와 속도 출력
		brick = rand() % ( sizeof(shape) / sizeof(shape[0]) ); // 0~6번 벽돌 선택하기
		nx = BW/2; //벽돌의 초기 X좌표
		ny = 2; //벽돌의 초기 Y좌표
		rot = 0; //벽돌의 초기 회전모양 번호
		printBrick(true); //벽돌 그리기 함수 호출
		if (getAround(nx, ny, brick, rot) != EMPTY) //벽돌 주변이 비었는지 체크
			break;
		nStay = nFrame;

		while(1)
		{
			/*벽돌내림*/
			if (--nStay == 0)
			{
				nStay = nFrame;
				if (moveDown()) //바닥에 닿으면 true
					break;
			}
			/*키입력 처리*/
			if (processKey()) //다른 벽돌에 닿으면 true
				break;
			/*시간 지연*/
			delay(1000/20);  //0.05초씩 지연 (1000=1초)
		}
	}
	/*게임 끝 처리*/
	clearScreen();
	gotoXY(30, 12); 
	puts("G A M E  O V E R");
	gotoXY(32, 15);
	printf("최종 점수 %d점", score);
	score = 0;
	setCursorType(NORMALCURSOR);
	
}


/**
* @brief drawScreen 게임의 배경인 게임판을 그린다.
* @detail board[x][y]에 EMPTY(0), BRICK(1), WALL(2)로 설정된 것에 맞게
*         arTile[]을 통해 ". "(0), "■"(1), "□"(2)로 화면에 그린다.
*/
void drawScreen()
{
	int x, y;

	for (x=0 ; x<BW+2 ; x++) 
	{
		for (y=0 ; y<BH+2 ; y++) 
		{
			gotoXY(BX+x*2, BY+y);
			puts(arTile[board[x][y]]);
		}
	}

	gotoXY(50, 3); 
	puts("Tetris Ver 1.0");
	
	gotoXY(50, 5); 
	puts("좌우:이동, 위:회전, 아래:내림");
	
	gotoXY(50, 6); 
	puts("공백:전부 내림");

}


/**
* @brief drawBoard 게임판의 내부를 그린다.
* @detail 외부벽과 문자열을 제외한 쌓여 있는 벽돌만 그린다.
*         board[x][y]에 EMPTY(0), BRICK(1), WALL(2)로 설정된 것에 맞게
*         arTile[]을 통해 ". "(0), "■"(1), "□"(2)로 화면에 그린다.
*/
void drawBoard()
{
	int x, y;

	for (x=1 ; x<BW+1 ; x++) 
	{
		for (y=1 ; y<BH+1 ; y++) 
		{
			gotoXY(BX+x*2, BY+y);
			puts(arTile[board[x][y]]);
		}
	}
}


/**
* @brief printBrick 벽돌을 그린다.
* @param bool Show true이면 벽돌을 그리고, false이면 벽돌을 삭제한다.
*/
void printBrick(bool Show)
{
	int i;

	for (i = 0; i<4; i++)
	{
		gotoXY(BX + (shape[brick][rot][i].x + nx) * 2, BY + shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}

}


/**
* @brief moveDown 벽돌을 한 칸 씩 아래로 내린다.
* @return bool 다른 벽돌에 닿으면 true를 반환한다.
*/
bool moveDown()
{
	if (getAround(nx, ny + 1, brick, rot) != EMPTY)
	{
		testFull();
		return true;
	}
	printBrick(false);
	ny++;
	printBrick(true);
	return false;
}


/**
* @brief processKey 키입력을 받아 그에 맞는 처리를 한다. 
* @return 이동 중인 벽돌이 다른 벽돌에 닿으면 true를 반환한다.
*/
bool processKey()
{
	int ch, trot;

	if (kbhit()) //눌러진 키가 있으면 true를 반환한다.
	{
		ch = getch(); //<conio.h> getch 값을 입력할 때까지 대기했다가 값이 들어오면 입력버퍼에서 1바이트를 가져온다.
		
		//화살표키(←키→키↑키↓키)는 2byte로 표현되는 확장키코드이다.
		//확장키코드가 입력되었을 경우에는 0 또는 0xE0을 반환한 후, 해당 키 스캔코드를 반환한다.
		if (ch==0xE0 || ch==0) 
		{
			ch = getch();

			switch (ch) 
			{
				case LEFT: //←키 입력 : 왼쪽 이동
					if (getAround(nx-1, ny, brick, rot) == EMPTY) 
					{
						printBrick(false);
						nx--;
						printBrick(true);
					}
					break;

				case RIGHT: //→키 입력 : 오른쪽 이동
					if ( getAround(nx+1, ny, brick, rot) == EMPTY ) 
					{
						printBrick(false);
						nx++;
						printBrick(true);
					}
					break;

				case UP: //↑키 입력 : 회전
					trot = (rot==3 ? 0 : rot+1);
					if ( getAround(nx, ny, brick, trot) == EMPTY ) 
					{
						printBrick(false);
						rot = trot;
						printBrick(true);
					}
					break;

				case DOWN: //↓키 입력 : 아래로 내림
					if ( moveDown() ) 
					{
						return true;
					}
					break;
			}

		}
		else {

			switch (ch) 
			{
				case ' ': //space키 입력 : 바닥까지 내림
					while ( moveDown() == false ) 
					{ 
						; 
					}
					return true;

			}

		}

	}

	return false; 

}


/**
* @brief getAround 벽돌의 주변이 공백인지, 다른 벽돌이 있는지 또는 벽인지를 판단한다.
* @param int x 현재 X좌표
* @param int y 현재 Y좌표
* @param int b 벽돌 번호
* @param int r 벽돌의 회전 번호
* @return int 공백이면 EMPTY, 다른 벽돌이 있으면 BRICK, 벽이면 WALL을 반환한다.
*/
int getAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i=0 ; i<4 ; i++) 
	{
		k = max(k, board[x + shape[b][r][i].x][y + shape[b][r][i].y]);
	}

	return k;
}


/**
* @brief testFull 게임판이 벽돌로 다 찼는지 체크한다.
* @detail 이동중인 벽돌을 board배열에 기록한다.
*         한줄 모두 벽돌이면 한줄을 지운다.
*/
void testFull()
{
	int i, x, y, ty;

	//이동중인 벽돌을 board 배열에 기록한다.
	for (i=0 ; i<4 ; i++) 
	{
		board[nx + shape[brick][rot][i].x][ny + shape[brick][rot][i].y] = BRICK;
	}

	for (y=1 ; y<BH+1 ; y++) 
	{
		//한줄이 모두 벽돌인지 판단
		for (x=1; x<BW+1; x++) 
		{
			if (board[x][y] != BRICK) 
				break;
		}
		//한줄이 모두 벽돌일 경우, 삭제할 줄 윗줄부터 한칸씩 아래로 복사한다.
		if (x == BW+1) 
		{
			for (ty=y ; ty>1 ; ty--) 
			{
				for (x=1 ; x<BW+1 ; x++) 
				{
					board[x][ty] = board[x][ty-1];
				}
			}
			
			drawBoard(); //한 줄 삭제 후 게임판 내부화면을 그린다.
			update_score(&score);
			test_score();
			delay(200); //0.2초 시간 지연
		}
	}
}

/**
* @brief 스코어를  80%의 확률로 100 올린다.
*/
void update_score(int* score){
	int test = rand() % 10;

	if(test == 0 || test == 1){
		*score = *score;
	}
	else{
		*score += 100;
	}
}

/**
* @brief 속도를 랜덤하게 변경한다.
*/
int random_nFrame(int nFrame){
	if(nFrame > 5 && nFrame < 12){ //최소 속도 5 최대 속도 12
		if (rand() % 20 == 6)
		{
			nFrame = nFrame - 1;
			return nFrame;
		}
		else if(rand() % 20 == 7){
			nFrame = nFrame + 1;
			return nFrame;
		}
	}
	return nFrame;
}

/**
* @brief 현재 속도와 점수 출력
*/
void print_score_nFrame(int score, int nFrame){

	gotoXY(46, 10);
	puts("▣ ▶==============◀ ▣");
	gotoXY(50, 8);
	printf("현재 score: %d", score);
	gotoXY(46, 7);
	puts("▣ ▶==============◀ ▣");
	gotoXY(50, 9);
	printf("현재 속도: %.2d", nFrame);
}


/**
* @brief 랜덤한 좌표의 블럭 삭제
*/
void random_clear_line(){	
	for(int i = 0; i < 17; i++)
	{
		int x = rand() % BW + 1; // 1부터 BW까지의 랜덤 x좌표
		int y = rand() % BH + 1; // 1부터 BH까지의 랜덤 y좌표

		board[x][y] = EMPTY;
		gotoXY(BX + x * 2, BY + y);
		puts(arTile[3]);
	}
}

/**
* @brief 기본 이미지 출력
*/
void print_qwerty(){
	gotoXY(40 , 12);
	puts("            :^^^^^^^^^^^^^^^:..                   ");
	gotoXY(40 , 13);
	puts("            ~???77!!!!!!!7777?7!~:                ");
	gotoXY(40 , 14);
	puts("            ~?77.^~~~~~~~~~~^^~77?!:              ");
	gotoXY(40 , 15);
	puts("            ~??7.~??7!!!!!77?7!:^7??~             ");
	gotoXY(40 , 16);
	puts("            ~??7.~??7      .~??7::???:            ");
	gotoXY(40 , 17);
	puts("            ~??7.~??7       :7??^.7??^            ");
	gotoXY(40 , 18);
	puts("            ~??7 ^!~^.::::^~7?7~.!??7.            ");
	gotoXY(40, 19);
	puts("            ~?!^.^~!77777777!~^^77?!.             ");
	gotoXY(40 , 20);
	puts("            ^^:!7?7!~^^~~~~~!!7?7~^.              ");
	gotoXY(40 , 21);
	puts("            .~???~:^!7?7777????~:~77^             ");
	gotoXY(40 , 22);
	puts("            ~???^.!??7^..  ^7??7~^7?7:            ");
	gotoXY(40 , 23);
	puts("            !??7.^?!?!      .!?!?!^!?^            ");
	gotoXY(40 , 24);
	puts("            ^???~.~?|!        ~7??7^~.            ");
	gotoXY(40 , 25);
	puts("             ^7?|!:^~~        .^7???~             ");
	gotoXY(40 , 26);
	puts("              .~7?|7!~^::::^~!77^!?|?!:           ");
	gotoXY(40 , 27);
	puts("                .:^!777777777!~^. ~77?7^          ");
	gotoXY(40 , 28);
	puts("                      ......       .....          ");
	gotoXY(40 , 29);
	puts("                                                  ");
	gotoXY(40 , 30);
	puts("                                                  ");
}

/**
* @brief 랜덤한 이미지 출력
*/
void random_picture(){
	int test;
	test = rand() % 4;

	if(test == 0){
		print_qwerty();
	}
	else if(test == 1){
		gotoXY(40 , 12);
		puts("                        ....                      ");
		gotoXY(40 , 13);
		puts("                      .^~~~~~.                    ");
		gotoXY(40 , 14);
		puts("                     .~~~~~~~^                    ");
		gotoXY(40 , 15);
		puts("                ..::^^~~~~~~~~^^:..               ");
		gotoXY(40 , 16);
		puts("             .:^~~~~~~~~~~~~~~~~~~~^.             ");
		gotoXY(40 , 17);
		puts("     .::^^^^:^^^^^^^~~~~~~~~~~~^::^~~^.           ");
		gotoXY(40 , 18);
		puts("    :~~~~~~~~~~~~^^^^:^~~~~~~~~~^^^~~~^           ");
		gotoXY(40, 19);
		puts("   .~~~~~~~~~~~~~~~~~^.^~~^:^^^~~~~~^^..          ");
		gotoXY(40 , 20);
		puts("   .^~~~~~~~~~~~~~~~~~^^^^^^^:.       ^^          ");
		gotoXY(40 , 21);
		puts("     .:^^~~~~~~~~~~~~~~~~~~~~~^.    .~~:...       ");
		gotoXY(40 , 22);
		puts("       :^^^^^^~~~~~~~~~~~~~~~~~^   .^^^~~^.       ");
		gotoXY(40 , 23);
		puts("       .~~~~^^^^^^^^~~~~^^^^~~~.      :~:         ");
		gotoXY(40 , 24);
		puts("        .~~~~~~~~^^^^^^^^^~:..        :.          ");
		gotoXY(40 , 25);
		puts("         .^~~~~~~~~~~~~~~^:.        .:            ");
		gotoXY(40 , 26);
		puts("           .:~~~~~^:....         .:^^.            ");
		gotoXY(40 , 27);
		puts("              .^^~~~^::......::^^^:.              ");
		gotoXY(40 , 28);
		puts("                 ..::^^^^^^^^^:..                 ");
		gotoXY(40 , 29);
		puts("                                                  ");
		gotoXY(40 , 30);
		puts("                                                  ");
		}
	else if(test == 2){
		gotoXY(40 , 12);
		puts("     ...................:::.................      ");
		gotoXY(40 , 13);
		puts("  .7G&&&&&&&&&&&&&&&&&&&#?P&&@@&&&&&&&&&&&&#5~    ");
		gotoXY(40 , 14);
		puts(" 7#@@@@@@@@@@@@@@@@@@@@B~ .!??YPB@@@@@@@@@@@@@G~  ");
		gotoXY(40 , 15);
		puts("!@@@@@@@@@@G#@@@@@@@@@5.         :!B@@@@@@@@@@@@5:");
		gotoXY(40 , 16);
		puts("!@@@@@@@@@@J!?YG#@@@&7           .5@@@@@@@@@@@@@@J");
		gotoXY(40 , 17);
		puts("!@@@@@@@@@B?!!!!7?Y5^           ~B@@@@@@@@@@@@@@@J");
		gotoXY(40 , 18);
		puts("!@@@@@@@BJ7!77777!^            ?@@@@@&#BBP5YJ?B@@J");
		gotoXY(40, 19);
		puts("!@@@@&GJ!!777777!:            !5J?7!~^^:::::.~&@@J");
		gotoXY(40 , 20);
		puts("!@@@&Y7!7777!!!~.           .::::::::::::::::5@@@J");
		gotoXY(40 , 21);
		puts("!@#BGPJ~~^^^^:.            .:::::::::::::::::~JB@J");
		gotoXY(40 , 22);
		puts("!&7::::::::::             .:::::::::^^^^7J5PGGB#@J");
		gotoXY(40 , 23);
		puts("!@#^:::::::.            .^^^~~~~!!!!!!777J5#@@@@@J");
		gotoXY(40 , 24);
		puts("!@@~.:::^:.            ^!7777777777777!!!!Y&@@@@@J");
		gotoXY(40 , 25);
		puts("!@@5JYPG?.           .!7!!!777777777!77?YB@@@@@@@J");
		gotoXY(40 , 26);
		puts("!@@@@#!            ?&@@@@@&G5?7!!!!Y&@@@@@@@@@@@@Y");
		gotoXY(40 , 27);
		puts("!@@@@&##G7       .P@@@@@@@@@@@#PJ7P@@@@@@@@@@@@@G^");
		gotoXY(40 , 28);
		puts(".J&@@@@@@@5~.   ~#@@@@@@@@@@@@@@@&@@@@@@@@@@@@B7  ");
		gotoXY(40 , 29);
		puts("  :J#@@@@@@@#P7J@@@@@@@@@@@@@@@@@@@@@@@@@@@@G7.   ");
		gotoXY(40 , 30);
		puts("    .!7777777?77777777777777777777777777777^      ");
	}
	else if(test == 3){
		gotoXY(40 , 12);
		puts("!@@@@@@@@@@@@@@@@@@@@@5???JB@@@@@@@@@@@@@@@@@@@@@?");
		gotoXY(40 , 13);
		puts("!@@@@@@@@@@@@@@@@@@@#J77????G@@@@@@@@@@@@@@@@@@@@?");
		gotoXY(40 , 14);
		puts("!@@@@@@@@@@@@@@@@@@P?777?7?P#G#@@@@@@@@@@@@@@@@@@?");
		gotoXY(40 , 15);
		puts("!@@@@@@@@@@@@@@@@&Y777777JB#Y?JB@@@@@@@@@@@@@@@@@?");
		gotoXY(40 , 16);
		puts("!@@@@@@@@@@@@@@@B?777777Y#G?????P&@@@@@@@@@@@@@@@?");
		gotoXY(40 , 17);
		puts("!@@@@@@@@@@@@@@P777777?P&5??????J#@@@@@@@@@@@@@@@?");
		gotoXY(40 , 18);
		puts("!@@@@@@@@@@@@#J!77777JBBJ7?????5#GYG@@@@@@@@@@@@@?");
		gotoXY(40, 19);
		puts("!@@@@@@@@@@@G7!777775#P?77?PGJG&5?JJP&@@@@@@@@@@@?");
		gotoXY(40 , 20);
		puts("!@@@@@@@@@&5!!!77!7G#Y777JB@@@BY?JJJJY#@@@@@@@@@@?");
		gotoXY(40 , 21);
		puts("!@@@@@@@@#?!!!!7!J#B?7775&@@@P??JJJJJJJG@@@@@@@@@?");
		gotoXY(40 , 22);
		puts("!@@@@@@@P7!!!!!!5&5777?G@@@&Y???JJJJJJJJP&@@@@@@@?");
		gotoXY(40 , 23);
		puts("!@@@@@&Y!!!!!!7G#J777J#@@@BJ?????JYJJJJJJYB@@@@@@?");
		gotoXY(40 , 24);
		puts("!@@@@B?!!!!!!J#G7!775&@@&P??????JG&PJJJJYYYG@@@@@?");
		gotoXY(40 , 25);
		puts("!@@@P7!!!!!!5#5!7!?G@@@#J7?????Y#BYYJJJJYYYYP&@@@?");
		gotoXY(40 , 26);
		puts("!@#J!!!!!!7BB?!!!J#@@@G?7?????P&P??JJJJJJYYYY5B@@?");
		gotoXY(40 , 27);
		puts("!#J777777J#G?7??5@@@@P??JJJJJG&5JJYYYYYYY555555G@?");
		gotoXY(40 , 28);
		puts("!&&&&&&&&@@&&&&&@@@@@&&&&&&&@@@&&&&&&&&@@@@@@@@&@?");
		gotoXY(40 , 29);
		puts("!@@@@@@@@@@@@@@@@@@&#@@@@@@@@#@@@@@@@@@@@@@@@@@@@?");
		gotoXY(40 , 30);
		puts("                                                  ");
	}
}


/**
* @brief 점수가 1000의 배수인지 확인 후 랜덤으로 방해공작
*/
void test_score(){
	if(score % 1000 == 0){
		int test;
		test = rand() % 2;
		if(test == 0){
			random_clear_line();
		}
		else if(test == 1){
			random_picture();
		}
	}
	else{
		
	}
}