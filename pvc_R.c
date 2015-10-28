//2P側が乱数で
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>

//盤面サイズ
#define Size 12
//手数
#define Number_of_Moves 72
//現在の盤面
int Current_Board[Size][Size];
//積み重ね記憶ポインター配列
int *Board_Pointer;
//積み重ね記憶ポインター制御変数
int BP0=Size-1,BP1=Size-1,BP2=Size-1,BP3=Size-1,BP4=Size-1, BP5=Size-1,BP6=Size-1,
	BP7=Size-1,BP8=Size-1,BP9=Size-1,BP10=Size-1,BP11=Size-1;

//盤面情報初期化
void Initialization_Board();
//ゲームスタート
int Game_Start();
//行動後の盤面を表示
void Show_Board();
//各プレイヤー行動which:何色が行動するか
int Player_Turn(int Board[Size][Size],int which);
//コンピュータ側の手を決定する(引数プレイヤーの白黒　乱数生成の最小値、最大値)
int CPU_Turn(int which , int Min,int Max);
//現在どちらが行動しているか
int Game_Side = 0;//0:白1:黒

//勝敗判定which:どちら側の判定をするか
int Game_Judge(int which);
//引き分け判定
int Game_Judge_Draw();
//手数管理変数
int Number_of_Move_Count = 0;
//ゲーム終了フラグ
int Game_End_Flag = 10;//初期値-1,0,1以外制限なし

//引数 human 人間側スタート　cpu側スタート
int main(int argc, char *argv[])
{

	//先手、後手設定
	if(strcmp(argv[1],"human") == 0)
	{
		Game_Side = 0;


	}else if(strcmp(argv[1],"cpu") == 0)
	{

		Game_Side = 1;

	}

	//盤面情報初期化
	Initialization_Board();

	//CPU側の手を乱数で決定する為、乱数のシードを時刻とする
	srand((unsigned int)time(NULL));

	//ゲーム開始
	Game_Start();

	if(Game_End_Flag == -1)
		printf("Draw\n");
	

	if(Game_End_Flag == 0)
		printf("Player1 Win\n");
	
	if(Game_End_Flag == 1)
		printf("Player2 Win\n");

	return 0;

}

/*****盤面初期化*****

配列要素...
-1  → 空き
1	→	X(黒)
0	→  0(白)

********************/
void Initialization_Board()
{
	//ループ制御変数
	int i = 0,j = 0;

	//ボードポインター初期化
	Board_Pointer = (int*)malloc(sizeof(int) * Size);
	for(i=0;i<Size;i++)
		Board_Pointer[i] = 0;


	for(i=0;i<Size;i++)
	{
		/*
		Current_Board[i][j] = -1;
		printf("%d",Current_Board[i][j]);
		*/
		for(j=0;j<Size;j++)
		{
			Current_Board[i][j] = -1;
			if(Current_Board[i][j] == -1)
				printf(".");
			//printf("%d",Current_Board[i][j]);
		}
		printf("\n");

	}

	//盤番号表示
	printf("0123456789AB\n");

}

int Game_Start()
{
	//ゲーム終了フラグ
	/**********

	-1 = Draw
	0  = P1Win
	1  = P21Win

	**********/
	
	//各プレイヤー行動
	while(1)
	{
	//プレイヤーの行動は次をループ勝敗判定後交代処理する
		Player_Turn(Current_Board,Game_Side);
		if(Game_End_Flag != 10)
		{
			if(Game_End_Flag == -1)
				return Game_End_Flag;
			if(Game_End_Flag == 0)
				return Game_End_Flag;
			if(Game_End_Flag == 1)
				return Game_End_Flag;
		}

		//プレイヤー交代処理
		if(Game_Side == 0)
		{
			Game_Side = 1;

		}else if(Game_Side == 1){

			Game_Side = 0;
		}

	}

}

//PTurn
int Player_Turn(int Board[Size][Size],int which)
{
	char Input_Board_Number_C[256] , *error;//入力は初めここに格納後に判定eは変換不可時の返還先
	int Input_Board_Number;
	int Domination_Board_Pointer;//操作するボードポインタ記憶
	int Player_Number = 1;//表示するプレイヤー番号
	//P1がどこにおくか
	//入力を確認する無効であれば再入力
	while(1)
	{
		if(which == 0)
		{
			Player_Number = 1;

		}else if(which == 1){

			Player_Number = 2;
		}

		printf("Player%d:",Player_Number);
		if(which == 0)
		{
		//文字列として格納
		scanf("%s",&Input_Board_Number_C);
		//文字列を16進変換
		Input_Board_Number = strtol(Input_Board_Number_C,&error,16);
		}else if(which == 1)
		{
			Input_Board_Number = CPU_Turn(which,0,11);

		}
		


		

		//ここでは単純に入力を評価する人間とCPUでは別で判定する
		if(which == 0 && Input_Board_Number < 12  && strcmp("\0",error) == 0)
		{
			//条件に合えば抜ける
			break;
		}else if(which == 1 &&  Input_Board_Number < 12){
			break;
			
		}else{
			
			printf("error\n");

		}
	}

	//スタックポインターセレクター
	switch(Input_Board_Number)
	{
		case 0:
			Domination_Board_Pointer = BP0;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP0 -= 1;//積み重ねたことを意味
			break;
		case 1:
			Domination_Board_Pointer = BP1;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP1 -= 1;//積み重ねたことを意味
			break;
		case 2:
			Domination_Board_Pointer = BP2;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP2 -= 1;//積み重ねたことを意味
			break;
		case 3:
			Domination_Board_Pointer = BP3;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP3 -= 1;//積み重ねたことを意味
			break;
		case 4:
			Domination_Board_Pointer = BP4;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP4 -= 1;//積み重ねたことを意味
			break;
		case 5:
			Domination_Board_Pointer = BP5;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP5 -= 1;//積み重ねたことを意味
			break;
		case 6:
			Domination_Board_Pointer = BP6;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP6 -= 1;//積み重ねたことを意味
			break;
		case 7:
			Domination_Board_Pointer = BP7;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP7 -= 1;//積み重ねたことを意味
			break;
		case 8:
			Domination_Board_Pointer = BP8;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP8 -= 1;//積み重ねたことを意味
			break;
		case 9:
			Domination_Board_Pointer = BP9;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP9 -= 1;//積み重ねたことを意味
			break;
		case 0x0A:
			Domination_Board_Pointer = BP10;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP10 -= 1;//積み重ねたことを意味
			break;
		case 0x0B:
			Domination_Board_Pointer = BP11;
			Board[Domination_Board_Pointer][Input_Board_Number] = which;
			BP11 -= 1;//積み重ねたことを意味
			break;
	}
	
	//盤面情報表示
	Show_Board();
	//手数カウント
	Number_of_Move_Count += 1;
	//手数カウント上限で引き分け判定
	if(Number_of_Move_Count == Number_of_Moves)
	{
		//引き分け判定とする
		Game_End_Flag = -1;
		return;
	}

	//勝敗判定
	Game_Judge(which);
}

//盤面表示
void Show_Board()
{
	//ループ制御変数
	int i,j;
	
	//数字に対応した盤情報を表示する
	for(i=0;i<Size;i++)
	{
		for(j=0;j<Size;j++)
		{
			if(Current_Board[i][j] == 0)
			{
				printf("0");
			}else if(Current_Board[i][j] == 1)
			{
				printf("X");
			}else{
				printf(".");
			}
		}
		printf("\n");
	}

	//盤番号表示
	printf("0123456789AB\n");

}

//勝敗判定引数はどちらのターンでjudgeしているか(0,1で区別しているもの)引数Pは置いた場所の番号
int Game_Judge(int which)
{
	int i=0,j=0;//ループ制御変数

	//勝敗判定 縦横斜めに4つそろったら終了
	//右横方向

			for(i=0;i<Size;i++)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == which &&Current_Board[i][j+1] == which &&Current_Board[i][j+2] == which &&Current_Board[i][j+3] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//左横方向
			for(i=0;i<Size;i++)
			{
				for(j=Size-1;j>4;j-=1)
				if(Current_Board[i][j] == which &&Current_Board[i][j-1] == which &&Current_Board[i][j-2] == which &&Current_Board[i][j-3] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=0;j<Size;j++)
				if(Current_Board[i][j] == which &&Current_Board[i-1][j] == which &&Current_Board[i-2][j] == which &&Current_Board[i-3][j] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=0;j<Size;j++)
				if(Current_Board[i][j] == which &&Current_Board[i+1][j] == which &&Current_Board[i+2][j] == which &&Current_Board[i+3][j] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//右上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == which &&Current_Board[i-1][j+1] == which &&Current_Board[i-2][j+2] == which &&Current_Board[i-3][j+3] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//左上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=Size-1;j>3;j-=1)
				if(Current_Board[i][j] == which && Current_Board[i-1][j-1] == which && Current_Board[i-2][j-2] == which && Current_Board[i-3][j-3] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//右下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == which &&Current_Board[i+1][j+1] == which &&Current_Board[i+2][j+2] == which &&Current_Board[i+3][j+3] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
			//左下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=Size-1;j>Size-3;j-=1)
				if(Current_Board[i][j] == which &&Current_Board[i+1][j-1] == which &&Current_Board[i+2][j-2] == which &&Current_Board[i+3][j-3] == which)
				{
					Game_End_Flag = which;
					return;
				}
			}
		


	

	//引き分け判定関数に飛ばす
			if(Game_End_Flag != 0 && Game_End_Flag != 1)
				Game_End_Flag = Game_Judge_Draw();

}

int Game_Judge_Draw()
{
	//ループ制御変数
	int i,j;

	//盤面全てを見る
	for(i=0;i<Size;i++)
	{
		for(j=0;j<Size;j++)
		{
			if(Current_Board[i][j] == -1)//ひとつでも空きがあればゲーム続行
				return 10;//Game_End_Flagの初期値を返しておく

		}


	}

	//全て走査した結果空がない場合(-1がひとつもない)引き分けを示す値を返す
	return -1;

}
//コンピュータ側の手を乱により決定する
int CPU_Turn(int which , int Min ,int Max)
{
	int Number;//CPU側の手

	Number = Min + (int)(rand() * (Max - Min + 1.0)/(1.0 + RAND_MAX));

	printf("CPUの手:%d\n",Number);


	//範囲乱数公式使用
	return Number;



}