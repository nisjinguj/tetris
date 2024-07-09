/**
 * �߰��� ���̴��� ����
 *�ʱ� ���̵��
 * 
 * =�˾����� ����: �������ϴ� ��Ʈ���� �������� ���� 12 ���� 22�̴�(�ܺκ� ����) =
 * 
 * 1: ���� �߰� 10���� �������ϴ°� ���� �Կ��� ������(�������� ������ �ϴ� ���̵��� ����)
 * 
 * 2: �� ������ �����ɶ����� 10%�� Ȯ���� ������ �������� �ӵ��� �����ϰ��Ѵ�()
 * 
 * 3: ������ 100�� ����� �ɶ����� �������� ���ذ����� ���´�
 * 
 * 4: ���� ���ذ����� ������ �ʴ´ٸ� �׸��� ����Ѵ�
 * 
 * cxc
 * 
 * 
**/ 




/****************************************************************************
* @mainpage Tetris : ��Ʈ���� ���� ����
* @version 1.0
* @date 1985��
* @author �˷����� �������� (���þ�)
* @brief ���� ��Ʈ���� ������ �����Ͽ� ����� ��Ʈ���� ������ �����.
* @detail ��ó : ȥ�� �����ϴ� C/C++ (http://www.soen.kr/)
*            ��.��Ʈ���� http://www.soen.kr/lecture/ccpp/cpp2/14-3-1.htm
*            ��.������ ��� ���� http://www.soen.kr/lecture/ccpp/cpp2/14-3-2.htm
*            ��.������ http://www.soen.kr/lecture/ccpp/cpp2/14-3-3.htm
*            ��.Turboc.h http://soen.kr/lecture/ccpp/cpp1/6-4-4.htm
*�̹��� ��ȯ : https://glan.tistory.com/33
*****************************************************************************/

#include "turboc.h"

/* ��� */
#define LEFT 75		//��Ű
#define RIGHT 77	//��Ű
#define UP 72		//��Ű
#define DOWN 80		//��Ű   
#define ESC 27
#define BX 5		//�������� �»�� ��ǥ X (�ܺ� ���� �»�� ��ǥ X)
#define BY 1		//�������� �»�� ��ǥ Y (�ܺ� ���� �»�� ��ǥ Y)
#define BW 10		//�������� �� (�ܺκ� ����)
#define BH 20		//�������� ���� (�ܺκ� ����)
#define max(a,b) (((a) > (b)) ? (a) : (b))

/* �Լ� */
void drawScreen();			//������ ���ȭ���� �������� �׸���.
void drawBoard();			//�������� ����ȭ���� �׸���. (�ܺκ��� ���ڿ� ����)
void printBrick(bool Show);	//������ �׸���.
bool moveDown();			//������ �¿�, �Ʒ��� �����δ�.
bool processKey();			//Ű�� ������ �� ������ �����Ѵ�.
int getAround(int x, int y, int b, int r);	//������ �ֺ��� �����Ͽ� ������� �ִ��� üũ�Ѵ�.
void testFull();			//�������� ������ �� á���� üũ�Ѵ�.
void update_score(int* score); // ���ھ �������� ������Ʈ �մϴ�.
void random_clear_line(); //�������� ���� ������ ����
int random_nFrame(int nFrame); //�������� �ӵ��� �����Ѵ�.
void print_score_nFrame(int score, int nFrame); // ���ھ ����Ѵ�.
void test_score(); //���ھ 1000�� ������� Ȯ�� �� �׸��� ������� ���ظ� ���� ���Ѵ�.
void random_picture();// ������ �׸��� ���
void print_qwerty(); //�⺻�̹����� ���
	
/* ����ü */
typedef struct point
{
	int x, y;
} point;


/* ���� ���� */
//shape[brick][rot][tile], shape[������ȣ][ȸ����ȣ][Ÿ�Ϲ�ȣ]
//������(0,0)���κ����� �����ǥ�� �����ϴ� ���
point shape[][4][4] =
{               
	{ 
		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} },
 		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} } 
	},	//0�� ��
	{ 
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} },
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} }
	},	//1�� ��
	{ 
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} },
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} }
	},	//2�� ����
	{ 
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} },
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} }
	},	//3�� ����
	{ 
		{ {0,0},{-1,0},{1,0},{-1,-1} }, { {0,0},{0,-1},{0,1},{-1,1} },
		{ {0,0},{-1,0},{1,0},{1,1} }, { {0,0},{0,-1},{0,1},{1,-1} }
	},	//4�� ����
	{ 
		{ {0,0},{1,0},{-1,0},{1,-1} }, { {0,0},{0,1},{0,-1},{-1,-1} },
		{ {0,0},{1,0},{-1,0},{-1,1} }, { {0,0},{0,-1},{0,1},{1,1} }
	},	//5�� ����
	{ 
		{ {0,0},{-1,0},{1,0},{0,1} }, { {0,0},{0,-1},{0,1},{1,0} },
		{ {0,0},{-1,0},{1,0},{0,-1} }, { {0,0},{-1,0},{0,-1},{0,1} }
	}	//6�� ��
};


enum { EMPTY, BRICK, WALL }; //0-EMPTY-����, 1-BRICK-�ٸ� ����, 2-WALL-��

// ��
const char *arTile[] = { ".","��","��","z" }; //����, ����, �� ����� �����ϴ� ���ڿ� �迭, Ÿ�� 1���� 2���ڷ� ��� ǥ�� 

int board[BW+2][BH+2]; //��ü �������� ����

int nx, ny; //�̵����� ������ �迭���� ���� ��ǥ (ȭ����� ��ǥ BX+nx*2, BY+ny)
int brick; //�̵����� ������ ��ȣ
int rot; //�̵����� ������ ȸ�� ��� ��ȣ
int score = 0;

/* main�Լ� */
int main()

{
	int nFrame, nStay; //������ �������� �ӵ�
	int x, y;

	/*���� �ʱ�ȭ*/
	setCursorType(NOCURSOR); //Ŀ�� ����
	srand((unsigned)time(NULL)); //���������� �ʱ�ȭ
	clearScreen(); //ȭ�� ����

	//���� �迭�� �ٱ����� ������ �ʱ�ȭ
	for (x=0 ; x<BW+2 ; x++) 
	{
		for (y=0 ; y<BH+2 ; y++) 
		{
			board[x][y] = ( y==0 || y==BH+1 || x==0 || x==BW+1 ) ? WALL : EMPTY;
		}
	}

	nFrame = 10; //������ �������� �ӵ�����
	drawScreen(); //ȭ�� ��ü �׸���
	print_qwerty(); //�⺻ �̹��� ���
	while(1)

	{
		/*�� ���� ����*/
		nFrame = random_nFrame(nFrame); // ���� �ӵ� ��ȯ
		print_score_nFrame(score, nFrame); //������ �ӵ� ���
		brick = rand() % ( sizeof(shape) / sizeof(shape[0]) ); // 0~6�� ���� �����ϱ�
		nx = BW/2; //������ �ʱ� X��ǥ
		ny = 2; //������ �ʱ� Y��ǥ
		rot = 0; //������ �ʱ� ȸ����� ��ȣ
		printBrick(true); //���� �׸��� �Լ� ȣ��
		if (getAround(nx, ny, brick, rot) != EMPTY) //���� �ֺ��� ������� üũ
			break;
		nStay = nFrame;

		while(1)
		{
			/*��������*/
			if (--nStay == 0)
			{
				nStay = nFrame;
				if (moveDown()) //�ٴڿ� ������ true
					break;
			}
			/*Ű�Է� ó��*/
			if (processKey()) //�ٸ� ������ ������ true
				break;
			/*�ð� ����*/
			delay(1000/20);  //0.05�ʾ� ���� (1000=1��)
		}
	}
	/*���� �� ó��*/
	clearScreen();
	gotoXY(30, 12); 
	puts("G A M E  O V E R");
	gotoXY(32, 15);
	printf("���� ���� %d��", score);
	score = 0;
	setCursorType(NORMALCURSOR);
	
}


/**
* @brief drawScreen ������ ����� �������� �׸���.
* @detail board[x][y]�� EMPTY(0), BRICK(1), WALL(2)�� ������ �Ϳ� �°�
*         arTile[]�� ���� ". "(0), "��"(1), "��"(2)�� ȭ�鿡 �׸���.
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
	puts("�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	
	gotoXY(50, 6); 
	puts("����:���� ����");

}


/**
* @brief drawBoard �������� ���θ� �׸���.
* @detail �ܺκ��� ���ڿ��� ������ �׿� �ִ� ������ �׸���.
*         board[x][y]�� EMPTY(0), BRICK(1), WALL(2)�� ������ �Ϳ� �°�
*         arTile[]�� ���� ". "(0), "��"(1), "��"(2)�� ȭ�鿡 �׸���.
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
* @brief printBrick ������ �׸���.
* @param bool Show true�̸� ������ �׸���, false�̸� ������ �����Ѵ�.
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
* @brief moveDown ������ �� ĭ �� �Ʒ��� ������.
* @return bool �ٸ� ������ ������ true�� ��ȯ�Ѵ�.
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
* @brief processKey Ű�Է��� �޾� �׿� �´� ó���� �Ѵ�. 
* @return �̵� ���� ������ �ٸ� ������ ������ true�� ��ȯ�Ѵ�.
*/
bool processKey()
{
	int ch, trot;

	if (kbhit()) //������ Ű�� ������ true�� ��ȯ�Ѵ�.
	{
		ch = getch(); //<conio.h> getch ���� �Է��� ������ ����ߴٰ� ���� ������ �Է¹��ۿ��� 1����Ʈ�� �����´�.
		
		//ȭ��ǥŰ(��Ű��Ű��Ű��Ű)�� 2byte�� ǥ���Ǵ� Ȯ��Ű�ڵ��̴�.
		//Ȯ��Ű�ڵ尡 �ԷµǾ��� ��쿡�� 0 �Ǵ� 0xE0�� ��ȯ�� ��, �ش� Ű ��ĵ�ڵ带 ��ȯ�Ѵ�.
		if (ch==0xE0 || ch==0) 
		{
			ch = getch();

			switch (ch) 
			{
				case LEFT: //��Ű �Է� : ���� �̵�
					if (getAround(nx-1, ny, brick, rot) == EMPTY) 
					{
						printBrick(false);
						nx--;
						printBrick(true);
					}
					break;

				case RIGHT: //��Ű �Է� : ������ �̵�
					if ( getAround(nx+1, ny, brick, rot) == EMPTY ) 
					{
						printBrick(false);
						nx++;
						printBrick(true);
					}
					break;

				case UP: //��Ű �Է� : ȸ��
					trot = (rot==3 ? 0 : rot+1);
					if ( getAround(nx, ny, brick, trot) == EMPTY ) 
					{
						printBrick(false);
						rot = trot;
						printBrick(true);
					}
					break;

				case DOWN: //��Ű �Է� : �Ʒ��� ����
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
				case ' ': //spaceŰ �Է� : �ٴڱ��� ����
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
* @brief getAround ������ �ֺ��� ��������, �ٸ� ������ �ִ��� �Ǵ� �������� �Ǵ��Ѵ�.
* @param int x ���� X��ǥ
* @param int y ���� Y��ǥ
* @param int b ���� ��ȣ
* @param int r ������ ȸ�� ��ȣ
* @return int �����̸� EMPTY, �ٸ� ������ ������ BRICK, ���̸� WALL�� ��ȯ�Ѵ�.
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
* @brief testFull �������� ������ �� á���� üũ�Ѵ�.
* @detail �̵����� ������ board�迭�� ����Ѵ�.
*         ���� ��� �����̸� ������ �����.
*/
void testFull()
{
	int i, x, y, ty;

	//�̵����� ������ board �迭�� ����Ѵ�.
	for (i=0 ; i<4 ; i++) 
	{
		board[nx + shape[brick][rot][i].x][ny + shape[brick][rot][i].y] = BRICK;
	}

	for (y=1 ; y<BH+1 ; y++) 
	{
		//������ ��� �������� �Ǵ�
		for (x=1; x<BW+1; x++) 
		{
			if (board[x][y] != BRICK) 
				break;
		}
		//������ ��� ������ ���, ������ �� ���ٺ��� ��ĭ�� �Ʒ��� �����Ѵ�.
		if (x == BW+1) 
		{
			for (ty=y ; ty>1 ; ty--) 
			{
				for (x=1 ; x<BW+1 ; x++) 
				{
					board[x][ty] = board[x][ty-1];
				}
			}
			
			drawBoard(); //�� �� ���� �� ������ ����ȭ���� �׸���.
			update_score(&score);
			test_score();
			delay(200); //0.2�� �ð� ����
		}
	}
}

/**
* @brief ���ھ  80%�� Ȯ���� 100 �ø���.
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
* @brief �ӵ��� �����ϰ� �����Ѵ�.
*/
int random_nFrame(int nFrame){
	if(nFrame > 5 && nFrame < 12){ //�ּ� �ӵ� 5 �ִ� �ӵ� 12
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
* @brief ���� �ӵ��� ���� ���
*/
void print_score_nFrame(int score, int nFrame){

	gotoXY(46, 10);
	puts("�� ��==============�� ��");
	gotoXY(50, 8);
	printf("���� score: %d", score);
	gotoXY(46, 7);
	puts("�� ��==============�� ��");
	gotoXY(50, 9);
	printf("���� �ӵ�: %.2d", nFrame);
}


/**
* @brief ������ ��ǥ�� �� ����
*/
void random_clear_line(){	
	for(int i = 0; i < 17; i++)
	{
		int x = rand() % BW + 1; // 1���� BW������ ���� x��ǥ
		int y = rand() % BH + 1; // 1���� BH������ ���� y��ǥ

		board[x][y] = EMPTY;
		gotoXY(BX + x * 2, BY + y);
		puts(arTile[3]);
	}
}

/**
* @brief �⺻ �̹��� ���
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
* @brief ������ �̹��� ���
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
* @brief ������ 1000�� ������� Ȯ�� �� �������� ���ذ���
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