/*******************************
足立法アルゴリズムシミュレーション(MazeConsole3)
作成：Takashi

【コーディングの際の注意事項】
・マイクロマウスに組み込むので極力、使用領域が少なくて済むようにする。
・使わなくなった変数を解放したりするのを忘れずに。(ローカル変数を用いるようにする)
・オブジェクト指向っぽく書く。(各機能をモジュール化する)

[変更記録]
2014.2.28
・既存プロジェクトからインタフェースを移植(コピー)
・マクロの定義、main関数の流れを記述

2014.3.1
・int imgRout_reInitとmaze_stepの2つの関数の処理を記述
《課題》
①コストの導出でバグが発生しやすいので、コスト導出アルゴリズムを慎重に考える。
②上でアルゴリズムを考える際、ななめ考慮などを入れやすいようなものにできればGood！

2014.3.4
・ゴール地点の初期値を1に変更
・コスト導出関数のバグを修正
	・変数の初期化のタイミングが不適切だったのでそれを改善
->とりあえず、ゴールまで到達することが可能になった。

2014.3.12
・maze_step関数を正常動作するように改善
->迷路データをどうやって与えるか、方法を考える必要がある。

2014.4.27
・maze_set関数のバグを修正、ゴールできることを確認。
->プログラムを書いてからしばらく経ってしまった。
まずはいままで書いてきたプログラムの読み込み直しから始めたい。
【課題】
①これは最終走行にしか現状では使えない
②探索走行アルゴリズムを完成させる必要がある

2014.4.28
・プログラム中の無駄な部分を削除

2014.9.11
・最終走行を実装、マイコン用に表示部分の削減開始

********************************/

#include<stdio.h>

#define SIZE_X 4
#define SIZE_Y 4
#define MAX_X SIZE_X * 2
#define MAX_Y SIZE_Y * 2
#define MAX_STEP SIZE_X*SIZE_Y
#define WALL 999
#define DEF 100	//初期状態のコスト
#define START_X 0
#define START_Y 0
#define GOAL_X 3
#define GOAL_Y 3

/*グローバル変数の宣言*/
int maze_data[SIZE_X * 2 + 1][SIZE_Y * 2 + 1] = { DEF };
int maze_predata[SIZE_X][SIZE_Y];
int maze_routData[MAX_STEP];
int x, y;

/*プロトタイプ宣言*/
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

	/*仮想ルートの初期化*/
	step_total = imgRout_reInit(x,y);
	step = 0;

	printf("仮想ルート\n");
	printf("■■■■■■■■■■■■■\n");

	for (i = 0; i < step_total; i++){
		printf("%d歩目=%d\n", (i+1), maze_routData[i]);
	}

	maze_disp();

	while ( !(x == GOAL_X && y == GOAL_Y)){
		maze_set2(x, y);
		tmp=maze_routData[step];
		tmp = imgRout_judge(tmp, x, y);	//仮想ルートを進行できるかを判断
		
		if (tmp != 0){	//仮想ルート進行不能
			//仮想ルートの再設定
			maze_costInit();
			step_total=imgRout_reInit(x,y);
			//歩数を0にリセット
			step = 0;
		}
		else{
			//仮想ルートを進行可能
			//(ここには何も処理を記述しない)
		}
		maze_step(maze_routData[step]);
		step += 1;
		printf("%d歩目\tx = %d\ty = %d\n", step,x,y);
		maze_disp();
	}

	printf("\nゴールです。\n", x, y);

	getchar();
	
	//スタート地点に戻す
	x = START_X;
	y = START_Y;

	printf("\n\n最終的にこうなる\n");

	maze_costInit();
	step_total = imgRout_reInit(x, y);
	step = 0;

	while (!(x == GOAL_X && y == GOAL_Y)){
		maze_step(maze_routData[step]);
		step += 1;
		printf("%d歩目\tx = %d\ty = %d\n", step, x, y);
		maze_disp();
	}

	printf("\nゴールです。\n", x, y);

	return 0;
}

/*迷路のコスト設定*/
void maze_costInit(){
	int i, j, k;
	int count = 0;
	int tmp = WALL;
	int base[2] = { 0 };
	int data[4] = { 0 };

	do{
		for (i = SIZE_X-1; i > (-1); i = i - 1){
			for (j = SIZE_Y-1; j > (-1); j = j - 1){
				//変数の初期化
				tmp = 100;

				//ベース座標の取得
				base[0] = 2 * i + 1;
				base[1] = 2 * j + 1;

				//壁情報の取得
				data[0] = maze_data[base[0] - 1][base[1]];
				data[1] = maze_data[base[0]][base[1] + 1];
				data[2] = maze_data[base[0] + 1][base[1]];
				data[3] = maze_data[base[0]][base[1] - 1];
				
				//最小値を求める
				for (k = 0; k < 4; k++){
					if (tmp>data[k]){
						tmp = data[k];
					}
				}

				//ゴール座標以外のコストを更新
				if (!(i == GOAL_X && j == GOAL_Y)){
					//左壁
					if (!(tmp == data[0] || data[0] == WALL)){
						maze_data[base[0] - 1][base[1]] = tmp+1;
					}
					//上壁
					if (!(tmp == data[1] || data[1] == WALL)){
						maze_data[base[0]][base[1] + 1] = tmp+1;
					}
					//右壁
					if (!(tmp == data[2] || data[2] == WALL)){
						maze_data[base[0] + 1][base[1]] = tmp+1;
					}
					//下壁
					if (!(tmp == data[3] || data[3] == WALL)){
						maze_data[base[0]][base[1] - 1] = tmp+1;
					}
				}
			}
		}
		count++;

	} while (count != (MAX_X * MAX_Y));
}

/*迷路の初期化関数*/
void maze_init(void){
	/*ローカル変数の宣言*/
	int i, j = 0;
	int base_plot[2] = { 0 };

	/*スタート地点の座標を代入*/
	x = START_X;
	y = START_Y;

	/*初期条件を代入*/
	for (i = 0; i < MAX_X + 1; i++){
		for (j = 0; j < MAX_Y + 1; j++){
			maze_data[i][j] = DEF;
		}
	}

	/*柱の情報を代入*/
	for (i = 0; i < MAX_X+1; i += 2){
		for (j = 0; j < MAX_Y+1; j += 2){
			maze_data[i][j] = WALL;
		}
	}

	/*外周の壁の情報を代入(縦方向)*/
	for (i = 1; i < MAX_X+1; i += 2){
		maze_data[i][0] = WALL;
		maze_data[i][SIZE_X * 2] = WALL;
	}

	/*外周の壁の情報を代入(横方向)*/
	for (j = 1; j < MAX_Y+1; j += 2){
		maze_data[0][j] = WALL;
		maze_data[SIZE_Y * 2][j] = WALL;
	}

	/*スタート地点右側の壁の情報を代入*/
	maze_data[2][1] = WALL;

	/*ゴール座標のコストを代入。*/
	base_plot[0] = GOAL_X * 2 + 1;	//ゴール座標のXをデータ座標に変換
	base_plot[1] = GOAL_Y * 2 + 1;	//ゴール座標のYをデータ座標に変換

	if (maze_data[(base_plot[0] - 1)][base_plot[1]] != WALL){	//ゴール地点左壁
		maze_data[(base_plot[0] - 1)][base_plot[1]] = 1;
	}
	if (maze_data[base_plot[0]][(base_plot[1] + 1)] != WALL){	//ゴール地点上壁
		maze_data[base_plot[0]][(base_plot[1] + 1)] = 1;
	}
	if (maze_data[(base_plot[0] + 1)][base_plot[1]] != WALL){	//ゴール地点右壁
		maze_data[(base_plot[0] + 1)][base_plot[1]] = 1;
	}
	if (maze_data[base_plot[0]][(base_plot[1] - 1)] != WALL){	//ゴール地点下壁
		maze_data[base_plot[0]][(base_plot[1] - 1)] = 1;
	}	
	
	printf("初期状態\n");
	maze_disp();

	return;
}

/*迷路の状態をコンソールで表示する関数*/
void maze_disp(void){
	/*ローカル変数の宣言*/
	int i, j = 0;

	printf("\n");

	for (j = SIZE_Y * 2; j >= 0; j -= 1){
		for (i = 0; i < SIZE_X * 2 + 1; i++){
			if (maze_data[i][j] == WALL){
				printf("■");
			}
			else if (i == x * 2 + 1 && j == y * 2 + 1){
				printf("☆");
			}
			else{
				printf("　");
			}
		}
		printf("\n");
	}
}

/*仮想ルートを判定*/
int imgRout_judge(int step,int a,int b){
	int i = 0;
	int data[4] = { 0 };
	int base_plot[2] = { 0 };
	int tmp[2] = { 999, 5 };

	/*壁の状態を取得*/
	base_plot[0] = a * 2 + 1;	//ゴール座標のXをデータ座標に変換
	base_plot[1] = b * 2 + 1;	//ゴール座標のYをデータ座標に変換
	data[0] = maze_data[base_plot[0]-1][base_plot[1]];
	data[1] = maze_data[base_plot[0]][base_plot[1]+1];
	data[2] = maze_data[base_plot[0]+1][base_plot[1]];
	data[3] = maze_data[base_plot[0]][base_plot[1]-1];

	/*最小値を求める*/
	for (i = 0; i < 4; i++){
		if (data[i]<tmp[0]){
			tmp[0] = data[i];
			tmp[1] = i;
		}
	}

	if (data[step] != WALL){
		return 0;	//仮想ルートを進むのが適している
	}
	else{
		return 1;	//仮想ルートを進めない
	}

}

/*一歩進める関数*/
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

/*仮想ルートの再設定関数*/
//4つの壁のうち、コストが最小となる方向を記録し続ける。
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

		//コストが最小となる方向を特定
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

		//仮想ルートとして記憶
		maze_routData[step] = minimum[1];
		step++;
	}

	//最終的な歩数を呼び出し元に返す
	return step;
}

/*
void maze_set(){
	int i, j, k;
	int base[2] = { 0x00 };
	int tmp[4] = { 0x00 };

	//迷路のデータを与える ⇒　データの点検をしておくこと
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
					else{	//ゴール座標だったら…
						tmp[k] = 0;
					}
				}
			}

			//ベース座標の計算
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

//探索シミュレーション用
void maze_set2(int a,int b){
	int k = 0;
	int base[2] = { 0x00 };
	int tmp[4] = { 0x00 };

	//迷路のデータを与える ⇒　データの点検をしておくこと
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
			else{	//ゴール座標だったら…
				tmp[k] = 0;
			}
		}
	}

	//ベース座標の計算
	base[0] = 2 * a + 1;
	base[1] = 2 * b + 1;

	maze_data[(base[0] - 1)][base[1]] = tmp[0];
	maze_data[base[0]][(base[1] + 1)] = tmp[1];
	maze_data[(base[0] + 1)][base[1]] = tmp[2];
	maze_data[base[0]][(base[1] - 1)] = tmp[3];

}
