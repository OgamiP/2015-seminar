//与えた盤面に対して4つ並ぶ手を求める

#include<stdio.h>
#include<stdlib.h>


#define Size 12

//盤面入力関数
int Input_Board();
//箱数カウント関数
void Count_Box();
//ひとつ置いて勝利する手（盤面）を全て表示する関数.引数はどちらが先行かを示す"First"変数
int Game_Start_Win(int which);

//現在の盤面を格納
int Current_Board[Size][Size];

//積み重ね記憶ポインター制御変数
int BP0=Size-1,BP1=Size-1,BP2=Size-1,BP3=Size-1,BP4=Size-1, BP5=Size-1,BP6=Size-1,
	BP7=Size-1,BP8=Size-1,BP9=Size-1,BP10=Size-1,BP11=Size-1;

//読み込み時次に打つのはどちらかを判断するために初めに各Playerの箱数をカウントする
int P1_Number = 0;
int P2_Number = 0;
//どちらが先手になるかを判定(0:白1:黒)
int First = -1;



int main()
{
	//入力盤面について初期化処理
	Input_Board();
	//盤面情報より先行、後攻を判定
	Count_Box();
	return 0;

}

int Input_Board()
{
	//ループ制御変数
	int i=0,j=0;
	FILE *fp;
	char file_name[128];
	int Number;

	printf("File Name:");
	scanf("%s",&file_name);

	//盤面情報読み込み(ファイル読み込み)
	fp = fopen(file_name,"r");
	if(fp == NULL)
	{
		printf("盤面情報読み込み失敗");
		return 0;

	}

//ボード情報を表示
	for(i=0;i<Size;i++)
	{
		for(j=0;j<Size;j++)
		{
			fscanf(fp,"%d",&Current_Board[i][j]);
			switch (Current_Board[i][j])
			{
				
			case -1:
				printf("・");
				break;

			case 0:
				printf("0");
				break;

			case 1:
				printf("X");
				break;


			}
		//	printf("%d",Current_Board[i][j]);

		}

		printf("\n");

	}

	printf("0 1 2 3 4 5 6 7 8 9 A B\n");

	//積み重ね制御変数を入力盤面に合わせる

	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][0] == -1)
		{
			BP0 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][1] == -1)
		{
			BP1 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][2] == -1)
		{
			BP2 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][3] == -1)
		{
			BP3 = i;
			break;
		}
	}

	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][4] == -1)
		{
			BP4 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][5] == -1)
		{
			BP5 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][6] == -1)
		{
			BP6 = i;
			break;
		}
	}

	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][7] == -1)
		{
			BP7 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][8] == -1)
		{
			BP8 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][9] == -1)
		{
			BP9 = i;
			break;
		}
	}

	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][10] == -1)
		{
			BP10 = i;
			break;
		}
	}

	
	for(i=Size-1;i>=0;i-=1)
	{
		if(Current_Board[i][11] == -1)
		{
			BP11 = i;
			break;
		}
	}


	return;



}

void Count_Box()
{
	//ループ制御関数
	int i=0,j=0;

	//入力盤面を検査し、どちらが先手になるかを判定する
	for(i=0;i<Size;i++)
	{
		for(j=0;j<Size;j++)
		{
			if(Current_Board[i][j] == 0)
			{
				P1_Number+=1;

			}else if(Current_Board[i][j] == 1)
			{
				P2_Number+=1;

			}

		}

	}
	//黒が少なければ黒からスタート
	if(P1_Number > P2_Number)
	{
		First = 1;
	}else if(P1_Number <= P2_Number)
	{
		//白が少ない、もしくは同じ数の場合白からスタート
		First = 0;

	}


}

//ひとつ置いて勝利する手（盤面）を全て表示する関数.引数はどちらが先行かを示す"First"変数
int Game_Start_Win(int which)
{


}
