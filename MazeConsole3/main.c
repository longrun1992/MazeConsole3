/*******************************
�����@�A���S���Y���V�~�����[�V����(MazeConsole3)
�쐬�FTakashi

�y�R�[�f�B���O�̍ۂ̒��ӎ����z
�E�}�C�N���}�E�X�ɑg�ݍ��ނ̂ŋɗ́A�g�p�̈悪���Ȃ��čςނ悤�ɂ���B
�E�g��Ȃ��Ȃ����ϐ�����������肷��̂�Y�ꂸ�ɁB(���[�J���ϐ���p����悤�ɂ���)
�E�I�u�W�F�N�g�w�����ۂ������B(�e�@�\�����W���[��������)

[�ύX�L�^]
2014.2.28
�E�����v���W�F�N�g����C���^�t�F�[�X���ڐA(�R�s�[)
�E�}�N���̒�`�Amain�֐��̗�����L�q

2014.3.1
�Eint imgRout_reInit��maze_step��2�̊֐��̏������L�q
�s�ۑ�t
�@�R�X�g�̓��o�Ńo�O���������₷���̂ŁA�R�X�g���o�A���S���Y����T�d�ɍl����B
�A��ŃA���S���Y�����l����ہA�ȂȂߍl���Ȃǂ����₷���悤�Ȃ��̂ɂł����Good�I

2014.3.4
�E�S�[���n�_�̏����l��1�ɕύX
�E�R�X�g���o�֐��̃o�O���C��
	�E�ϐ��̏������̃^�C�~���O���s�K�؂������̂ł�������P
->�Ƃ肠�����A�S�[���܂œ��B���邱�Ƃ��\�ɂȂ����B

2014.3.12
�Emaze_step�֐��𐳏퓮�삷��悤�ɉ��P
->���H�f�[�^���ǂ�����ė^���邩�A���@���l����K�v������B

2014.4.27
�Emaze_set�֐��̃o�O���C���A�S�[���ł��邱�Ƃ��m�F�B
->�v���O�����������Ă��炵�΂炭�o���Ă��܂����B
�܂��͂��܂܂ŏ����Ă����v���O�����̓ǂݍ��ݒ�������n�߂����B
�y�ۑ�z
�@����͍ŏI���s�ɂ�������ł͎g���Ȃ�
�A�T�����s�A���S���Y��������������K�v������

2014.4.28
�E�v���O�������̖��ʂȕ������폜

2014.9.11
�E�ŏI���s�������A�}�C�R���p�ɕ\�������̍팸�J�n

********************************/

#include<stdio.h>

#define SIZE_X 4
#define SIZE_Y 4
#define MAX_X SIZE_X * 2
#define MAX_Y SIZE_Y * 2
#define MAX_STEP SIZE_X*SIZE_Y
#define WALL 999
#define DEF 100	//������Ԃ̃R�X�g
#define START_X 0
#define START_Y 0
#define GOAL_X 3
#define GOAL_Y 3

/*�O���[�o���ϐ��̐錾*/
int maze_data[SIZE_X * 2 + 1][SIZE_Y * 2 + 1] = { DEF };
int maze_predata[SIZE_X][SIZE_Y];
int maze_routData[MAX_STEP];
int x, y;

/*�v���g�^�C�v�錾*/
void maze_disp(void);
void maze_init(void);
void maze_step(int);
void maze_costInit(void);
int imgRout_judge(int, int, int);
int imgRout_reInit(int,int);
//void maze_set(void);
void maze_set2(int,int);


int main(void){
	int i = 0;
	int step = 0;
	int step_total = 0;
	int tmp = 0;

	maze_init();

	maze_costInit();

	/*���z���[�g�̏�����*/
	step_total = imgRout_reInit(x,y);
	step = 0;

	printf("���z���[�g\n");
	printf("��������������������������\n");

	for (i = 0; i < step_total; i++){
		printf("%d����=%d\n", (i+1), maze_routData[i]);
	}

	maze_disp();

	while ( !(x == GOAL_X && y == GOAL_Y)){
		maze_set2(x, y);
		tmp=maze_routData[step];
		tmp = imgRout_judge(tmp, x, y);	//���z���[�g��i�s�ł��邩�𔻒f
		
		if (tmp != 0){	//���z���[�g�i�s�s�\
			//���z���[�g�̍Đݒ�
			maze_costInit();
			step_total=imgRout_reInit(x,y);
			//������0�Ƀ��Z�b�g
			step = 0;
		}
		else{
			//���z���[�g��i�s�\
			//(�����ɂ͉����������L�q���Ȃ�)
		}
		maze_step(maze_routData[step]);
		step += 1;
		printf("%d����\tx = %d\ty = %d\n", step,x,y);
		maze_disp();
	}

	printf("\n�S�[���ł��B\n", x, y);

	getchar();
	
	//�X�^�[�g�n�_�ɖ߂�
	x = START_X;
	y = START_Y;

	printf("\n\n�ŏI�I�ɂ����Ȃ�\n");

	maze_costInit();
	step_total = imgRout_reInit(x, y);
	step = 0;

	while (!(x == GOAL_X && y == GOAL_Y)){
		maze_step(maze_routData[step]);
		step += 1;
		printf("%d����\tx = %d\ty = %d\n", step, x, y);
		maze_disp();
	}

	printf("\n�S�[���ł��B\n", x, y);

	return 0;
}

/*���H�̃R�X�g�ݒ�*/
void maze_costInit(){
	int i, j, k;
	int count = 0;
	int tmp = WALL;
	int base[2] = { 0 };
	int data[4] = { 0 };

	do{
		for (i = SIZE_X-1; i > (-1); i = i - 1){
			for (j = SIZE_Y-1; j > (-1); j = j - 1){
				//�ϐ��̏�����
				tmp = 100;

				//�x�[�X���W�̎擾
				base[0] = 2 * i + 1;
				base[1] = 2 * j + 1;

				//�Ǐ��̎擾
				data[0] = maze_data[base[0] - 1][base[1]];
				data[1] = maze_data[base[0]][base[1] + 1];
				data[2] = maze_data[base[0] + 1][base[1]];
				data[3] = maze_data[base[0]][base[1] - 1];
				
				//�ŏ��l�����߂�
				for (k = 0; k < 4; k++){
					if (tmp>data[k]){
						tmp = data[k];
					}
				}

				//�S�[�����W�ȊO�̃R�X�g���X�V
				if (!(i == GOAL_X && j == GOAL_Y)){
					//����
					if (!(tmp == data[0] || data[0] == WALL)){
						maze_data[base[0] - 1][base[1]] = tmp+1;
					}
					//���
					if (!(tmp == data[1] || data[1] == WALL)){
						maze_data[base[0]][base[1] + 1] = tmp+1;
					}
					//�E��
					if (!(tmp == data[2] || data[2] == WALL)){
						maze_data[base[0] + 1][base[1]] = tmp+1;
					}
					//����
					if (!(tmp == data[3] || data[3] == WALL)){
						maze_data[base[0]][base[1] - 1] = tmp+1;
					}
				}
			}
		}
		count++;

	} while (count != (MAX_X * MAX_Y));
}

/*���H�̏������֐�*/
void maze_init(void){
	/*���[�J���ϐ��̐錾*/
	int i, j = 0;
	int base_plot[2] = { 0 };

	/*�X�^�[�g�n�_�̍��W����*/
	x = START_X;
	y = START_Y;

	/*������������*/
	for (i = 0; i < MAX_X + 1; i++){
		for (j = 0; j < MAX_Y + 1; j++){
			maze_data[i][j] = DEF;
		}
	}

	/*���̏�����*/
	for (i = 0; i < MAX_X+1; i += 2){
		for (j = 0; j < MAX_Y+1; j += 2){
			maze_data[i][j] = WALL;
		}
	}

	/*�O���̕ǂ̏�����(�c����)*/
	for (i = 1; i < MAX_X+1; i += 2){
		maze_data[i][0] = WALL;
		maze_data[i][SIZE_X * 2] = WALL;
	}

	/*�O���̕ǂ̏�����(������)*/
	for (j = 1; j < MAX_Y+1; j += 2){
		maze_data[0][j] = WALL;
		maze_data[SIZE_Y * 2][j] = WALL;
	}

	/*�X�^�[�g�n�_�E���̕ǂ̏�����*/
	maze_data[2][1] = WALL;

	/*�S�[�����W�̃R�X�g�����B*/
	base_plot[0] = GOAL_X * 2 + 1;	//�S�[�����W��X���f�[�^���W�ɕϊ�
	base_plot[1] = GOAL_Y * 2 + 1;	//�S�[�����W��Y���f�[�^���W�ɕϊ�

	if (maze_data[(base_plot[0] - 1)][base_plot[1]] != WALL){	//�S�[���n�_����
		maze_data[(base_plot[0] - 1)][base_plot[1]] = 1;
	}
	if (maze_data[base_plot[0]][(base_plot[1] + 1)] != WALL){	//�S�[���n�_���
		maze_data[base_plot[0]][(base_plot[1] + 1)] = 1;
	}
	if (maze_data[(base_plot[0] + 1)][base_plot[1]] != WALL){	//�S�[���n�_�E��
		maze_data[(base_plot[0] + 1)][base_plot[1]] = 1;
	}
	if (maze_data[base_plot[0]][(base_plot[1] - 1)] != WALL){	//�S�[���n�_����
		maze_data[base_plot[0]][(base_plot[1] - 1)] = 1;
	}	
	
	printf("�������\n");
	maze_disp();

	return;
}

/*���H�̏�Ԃ��R���\�[���ŕ\������֐�*/
void maze_disp(void){
	/*���[�J���ϐ��̐錾*/
	int i, j = 0;

	printf("\n");

	for (j = SIZE_Y * 2; j >= 0; j -= 1){
		for (i = 0; i < SIZE_X * 2 + 1; i++){
			if (maze_data[i][j] == WALL){
				printf("��");
			}
			else if (i == x * 2 + 1 && j == y * 2 + 1){
				printf("��");
			}
			else{
				printf("�@");
			}
		}
		printf("\n");
	}
}

/*���z���[�g�𔻒�*/
int imgRout_judge(int step,int a,int b){
	int i = 0;
	int data[4] = { 0 };
	int base_plot[2] = { 0 };
	int tmp[2] = { 999, 5 };

	/*�ǂ̏�Ԃ��擾*/
	base_plot[0] = a * 2 + 1;	//�S�[�����W��X���f�[�^���W�ɕϊ�
	base_plot[1] = b * 2 + 1;	//�S�[�����W��Y���f�[�^���W�ɕϊ�
	data[0] = maze_data[base_plot[0]-1][base_plot[1]];
	data[1] = maze_data[base_plot[0]][base_plot[1]+1];
	data[2] = maze_data[base_plot[0]+1][base_plot[1]];
	data[3] = maze_data[base_plot[0]][base_plot[1]-1];

	/*�ŏ��l�����߂�*/
	for (i = 0; i < 4; i++){
		if (data[i]<tmp[0]){
			tmp[0] = data[i];
			tmp[1] = i;
		}
	}

	if (data[step] != WALL){
		return 0;	//���z���[�g��i�ނ̂��K���Ă���
	}
	else{
		return 1;	//���z���[�g��i�߂Ȃ�
	}

}

/*����i�߂�֐�*/
void maze_step(int direction){
	switch (direction){
	case 0:
		x = x - 1;
		break;
	case 1:
		y = y + 1;
		break;
	case 2:
		x = x + 1;
		break;
	case 3:
		y = y - 1;
		break;
	default:
		break;
	}
}

/*���z���[�g�̍Đݒ�֐�*/
//4�̕ǂ̂����A�R�X�g���ŏ��ƂȂ�������L�^��������B
int imgRout_reInit(int a,int b){
	int data[4] = { 0 };
	int base[2] = { 0 };
	int step = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int minimum[2] = { 0 };

	i = a;
	j = b;

	while (!(i == GOAL_X && j == GOAL_Y)){

		base[0] = 2 * i + 1;
		base[1] = 2 * j + 1;

		minimum[0] = 100;
		minimum[1] = 5;

		data[0] = maze_data[(base[0] - 1)][base[1]];
		data[1] = maze_data[base[0]][(base[1] + 1)];
		data[2] = maze_data[(base[0] + 1)][base[1]];
		data[3] = maze_data[base[0]][(base[1] - 1)];

		printf("( %d , %d )\t", i, j);

		for (k = 0; k < 4; k++){
			printf("data[%d]=%d\t", k, data[k]);
		}
		printf("\n");

		//�R�X�g���ŏ��ƂȂ���������
		for (k = 0; k < 4; k++){
			if (data[k] < minimum[0]){
				minimum[0] = data[k];
				minimum[1] = k;
			}
		}

		switch (minimum[1]){
		case 0:
			i = i - 1;
			break;
		case 1:
			j = j + 1;
			break;
		case 2:
			i = i + 1;
			break;
		case 3:
			j = j - 1;
			break;
		default:
			printf("Error!\n");
			break;
		}

		//���z���[�g�Ƃ��ċL��
		maze_routData[step] = minimum[1];
		step++;
	}

	//�ŏI�I�ȕ������Ăяo�����ɕԂ�
	return step;
}

/*
void maze_set(){
	int i, j, k;
	int base[2] = { 0x00 };
	int tmp[4] = { 0x00 };

	//���H�̃f�[�^��^���� �ˁ@�f�[�^�̓_�������Ă�������
	maze_predata[0][0] = 0x0B;	//0x0B
	maze_predata[1][0] = 0x09;
	maze_predata[2][0] = 0x01;
	maze_predata[3][0] = 0x03;
	maze_predata[0][1] = 0x08;
	maze_predata[1][1] = 0x06;
	maze_predata[2][1] = 0x0A;
	maze_predata[3][1] = 0x0A;
	maze_predata[0][2] = 0x08;
	maze_predata[1][2] = 0x03;
	maze_predata[2][2] = 0x0A;
	maze_predata[3][2] = 0x0A;
	maze_predata[0][3] = 0x0E;
	maze_predata[1][3] = 0x0E;
	maze_predata[2][3] = 0x0C;
	maze_predata[3][3] = 0x06;
	

	for (i = 0; i < SIZE_X; i++){
		for (j = 0; j < SIZE_Y; j++){
			tmp[0] = 0x08 & maze_predata[i][j];// / 8;
			tmp[1] = 0x04 & maze_predata[i][j];// / 4;
			tmp[2] = 0x02 & maze_predata[i][j];// / 2;
			tmp[3] = 0x01 & maze_predata[i][j];// / 1;
			
			for (k = 0; k < 4; k++){
				if (tmp[k] != 0){
					tmp[k] = WALL;
				}
				else{
					if (!(i == GOAL_X && j == GOAL_Y)){
						tmp[k] = DEF;
					}
					else{	//�S�[�����W��������c
						tmp[k] = 0;
					}
				}
			}

			//�x�[�X���W�̌v�Z
			base[0] = 2 * i + 1;
			base[1] = 2 * j + 1;

			maze_data[(base[0] - 1)][base[1]] = tmp[0];
			maze_data[base[0]][(base[1] + 1)] = tmp[1];
			maze_data[(base[0] + 1)][base[1]] = tmp[2];
			maze_data[base[0]][(base[1] - 1)] = tmp[3];
			
		}
	}
	maze_disp();
	
}
*/

//�T���V�~�����[�V�����p
void maze_set2(int a,int b){
	int k = 0;
	int base[2] = { 0x00 };
	int tmp[4] = { 0x00 };

	//���H�̃f�[�^��^���� �ˁ@�f�[�^�̓_�������Ă�������
	maze_predata[0][0] = 0x0B;
	maze_predata[1][0] = 0x09;
	maze_predata[2][0] = 0x01;
	maze_predata[3][0] = 0x03;
	maze_predata[0][1] = 0x08;
	maze_predata[1][1] = 0x06;
	maze_predata[2][1] = 0x0A;
	maze_predata[3][1] = 0x0A;
	maze_predata[0][2] = 0x08;
	maze_predata[1][2] = 0x03;
	maze_predata[2][2] = 0x0A;
	maze_predata[3][2] = 0x0A;
	maze_predata[0][3] = 0x0E;
	maze_predata[1][3] = 0x0E;
	maze_predata[2][3] = 0x0C;
	maze_predata[3][3] = 0x06;

	tmp[0] = 0x08 & maze_predata[a][b];	//8
	tmp[1] = 0x04 & maze_predata[a][b];	//4;
	tmp[2] = 0x02 & maze_predata[a][b];	//2;
	tmp[3] = 0x01 & maze_predata[a][b];	//1;

	for (k = 0; k < 4; k++){
		if (tmp[k] != 0){
			tmp[k] = WALL;
		}
		else{
			if (!(a == GOAL_X && b == GOAL_Y)){
				tmp[k] = DEF;
			}
			else{	//�S�[�����W��������c
				tmp[k] = 0;
			}
		}
	}

	//�x�[�X���W�̌v�Z
	base[0] = 2 * a + 1;
	base[1] = 2 * b + 1;

	maze_data[(base[0] - 1)][base[1]] = tmp[0];
	maze_data[base[0]][(base[1] + 1)] = tmp[1];
	maze_data[(base[0] + 1)][base[1]] = tmp[2];
	maze_data[base[0]][(base[1] - 1)] = tmp[3];

}
