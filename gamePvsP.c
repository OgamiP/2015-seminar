#include<stdio.h>
#include<stdlib.h>

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
//プレイヤー1行動
int Player1_Turn(int Board[Size][Size]);
//プレイヤー2行動
int Player2_Turn(int Board[Size][Size]);
//勝敗判定
int Game_Judge(int Judge_Point);
//引き分け判定
int Game_Judge_Draw();
//手数管理変数
int Number_of_Move_Count = 0;
//ゲーム終了フラグ
int Game_End_Flag = 10;//初期値-1,0,1以外制限なし

int main()
{

	//盤面情報初期化
	Initialization_Board();
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
				printf("・");
			//printf("%d",Current_Board[i][j]);
		}
		printf("\n");

	}

	//盤番号表示
	printf("0 1 2 3 4 5 6 7 8 9 A B\n");

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
	//P1
	Player1_Turn(Current_Board);
	if(Game_End_Flag != 10)
	{
		if(Game_End_Flag == -1)
			return Game_End_Flag;
		if(Game_End_Flag == 0)
			return Game_End_Flag;
		if(Game_End_Flag == 1)
			return Game_End_Flag;

	}
	//P2
	Player2_Turn(Current_Board);
	if(Game_End_Flag != 10)
	{
		if(Game_End_Flag == -1)
			return Game_End_Flag;
		if(Game_End_Flag == 0)
			return Game_End_Flag;
		if(Game_End_Flag == 1)
			return Game_End_Flag;

	}

	}

}

//P1 Turn
int Player1_Turn(int Board[Size][Size])
{
	int Input_Board_Number;
	int Domination_Board_Pointer;//操作するボードポインタ記憶
	//P1がどこにおくか
	printf("Player1:");
	scanf("%d",&Input_Board_Number);

	//スタックポインターセレクター
	switch(Input_Board_Number)
	{
		case 0:
			Domination_Board_Pointer = BP0;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP0 -= 1;//積み重ねたことを意味
			break;
		case 1:
			Domination_Board_Pointer = BP1;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP1 -= 1;//積み重ねたことを意味
			break;
		case 2:
			Domination_Board_Pointer = BP2;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP2 -= 1;//積み重ねたことを意味
			break;
		case 3:
			Domination_Board_Pointer = BP3;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP3 -= 1;//積み重ねたことを意味
			break;
		case 4:
			Domination_Board_Pointer = BP4;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP4 -= 1;//積み重ねたことを意味
			break;
		case 5:
			Domination_Board_Pointer = BP5;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP5 -= 1;//積み重ねたことを意味
			break;
		case 6:
			Domination_Board_Pointer = BP6;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP6 -= 1;//積み重ねたことを意味
			break;
		case 7:
			Domination_Board_Pointer = BP7;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP7 -= 1;//積み重ねたことを意味
			break;
		case 8:
			Domination_Board_Pointer = BP8;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP8 -= 1;//積み重ねたことを意味
			break;
		case 9:
			Domination_Board_Pointer = BP9;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP9 -= 1;//積み重ねたことを意味
			break;
		case 10:
			Domination_Board_Pointer = BP10;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
			BP4 -= 1;//積み重ねたことを意味
			break;
		case 11:
			Domination_Board_Pointer = BP11;
			Board[Domination_Board_Pointer][Input_Board_Number] = 0;
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

	
	Game_Judge(0);
}

//P2 Turn
int Player2_Turn(int Board[Size][Size])
{
	int Input_Board_Number;
	int Domination_Board_Pointer;//操作するボードポインタ記憶
	//P2がどこにおくか
	printf("Player2:");
	scanf("%x",&Input_Board_Number);

	//スタックポインターセレクター
	switch(Input_Board_Number)
	{
		case 0:
			Domination_Board_Pointer = BP0;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP0 -= 1;//積み重ねたことを意味
			break;
		case 1:
			Domination_Board_Pointer = BP1;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP1 -= 1;//積み重ねたことを意味
			break;
		case 2:
			Domination_Board_Pointer = BP2;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP2 -= 1;//積み重ねたことを意味
			break;
		case 3:
			Domination_Board_Pointer = BP3;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP3 -= 1;//積み重ねたことを意味
			break;
		case 4:
			Domination_Board_Pointer = BP4;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP4 -= 1;//積み重ねたことを意味
			break;
		case 5:
			Domination_Board_Pointer = BP5;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP5  -= 1;//積み重ねたことを意味
			break;
		case 6:
			Domination_Board_Pointer = BP6;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP6 -= 1;//積み重ねたことを意味
			break;
		case 7:
			Domination_Board_Pointer = BP7;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP7 -= 1;//積み重ねたことを意味
			break;
		case 8:
			Domination_Board_Pointer = BP8;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP8 -= 1;//積み重ねたことを意味
			break;
		case 9:
			Domination_Board_Pointer = BP9;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP9 -= 1;//積み重ねたことを意味
			break;
		case 10:
			Domination_Board_Pointer = BP10;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
			BP10 -= 1;//積み重ねたことを意味
			break;
		case 11:
			Domination_Board_Pointer = BP11;
			Board[Domination_Board_Pointer][Input_Board_Number] = 1;
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

	//置かれた場所を基準に勝敗判定を行う
	Game_Judge(1);
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
				printf("・");
			}
		}
		printf("\n");
	}

	//盤番号表示
	printf("0 1 2 3 4 5 6 7 8 9 A B\n");

}

//勝敗判定引数はどちらのターンでjudgeしているか(0,1で区別しているもの)引数Pは置いた場所の番号
int Game_Judge(int Judge_Side)
{
	int i=0,j=0,k=0;//ループ制御変数

	//勝敗判定 縦横斜めに4つそろったら終了
		
	switch (Judge_Side)
	{
		case 0:
			//右横方向
			for(i=0;i<Size;i++)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == 0 &&Current_Board[i][j+1] == 0 &&Current_Board[i][j+2] == 0 &&Current_Board[i][j+3] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//左横方向
			for(i=0;i<Size;i++)
			{
				for(j=Size-1;j>4;j-=1)
				if(Current_Board[i][j] == 0 &&Current_Board[i][j-1] == 0 &&Current_Board[i][j-2] == 0 &&Current_Board[i][j-3] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=0;j<Size;j++)
				if(Current_Board[i][j] == 0 &&Current_Board[i-1][j] == 0 &&Current_Board[i-2][j] == 0 &&Current_Board[i-3][j] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=0;j<Size;j++)
				if(Current_Board[i][j] == 0 &&Current_Board[i+1][j] == 0 &&Current_Board[i+2][j] == 0 &&Current_Board[i+3][j] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//右上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == 0 &&Current_Board[i-1][j+1] == 0 &&Current_Board[i-2][j+2] == 0 &&Current_Board[i-3][j+3] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//左上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=Size-1;j>3;j-=1)
				if(Current_Board[i][j] == 0 && Current_Board[i-1][j-1] == 0 && Current_Board[i-2][j-2] == 0 && Current_Board[i-3][j-3] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//右下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == 0 &&Current_Board[i+1][j+1] == 0 &&Current_Board[i+2][j+2] == 0 &&Current_Board[i+3][j+3] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}
			//左下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=Size-1;j>Size-3;j-=1)
				if(Current_Board[i][j] == 0 &&Current_Board[i+1][j-1] == 0 &&Current_Board[i+2][j-2] == 0 &&Current_Board[i+3][j-3] == 0)
				{
					Game_End_Flag = 0;
					return;
				}
			}

	
			break;

			
		case 1:
			//右横方向
			for(i=0;i<Size;i++)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == 1 &&Current_Board[i][j+1] == 1 &&Current_Board[i][j+2] == 1 &&Current_Board[i][j+3] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//左横方向
			for(i=0;i<Size;i++)
			{
				for(j=Size-1;j>4;j-=1)
				if(Current_Board[i][j] == 1 &&Current_Board[i][j-1] == 1 &&Current_Board[i][j-2] == 1 &&Current_Board[i][j-3] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=0;j<Size;j++)
				if(Current_Board[i][j] == 1 &&Current_Board[i-1][j] == 1 &&Current_Board[i-2][j] == 1 &&Current_Board[i-3][j] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=0;j<Size;j++)
				if(Current_Board[i][j] == 1 &&Current_Board[i+1][j] == 1 &&Current_Board[i+2][j] == 1 &&Current_Board[i+3][j] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//右上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == 1 &&Current_Board[i-1][j+1] == 1 &&Current_Board[i-2][j+2] == 1 &&Current_Board[i-3][j+3] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//左上方向
			for(i=Size-1;i>Size-3;i-=1)
			{
				for(j=Size-1;j>3;j-=1)
				if(Current_Board[i][j] == 1 && Current_Board[i-1][j-1] == 1 && Current_Board[i-2][j-2] == 1 && Current_Board[i-3][j-3] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//右下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=0;j<Size-3;j++)
				if(Current_Board[i][j] == 1 &&Current_Board[i+1][j+1] == 1 &&Current_Board[i+2][j+2] == 1 &&Current_Board[i+3][j+3] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}
			//左下方向
			for(i=0;i<Size-3;i++)
			{
				for(j=Size-1;j>Size-3;j-=1)
				if(Current_Board[i][j] == 1 &&Current_Board[i+1][j-1] == 1 &&Current_Board[i+2][j-2] == 1 &&Current_Board[i+3][j-3] == 1)
				{
					Game_End_Flag = 1;
					return;
				}
			}

	
			break;



	}

	//引き分け判定関数に飛ばす
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