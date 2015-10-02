//与えた盤面に対して4つ並ぶことを防ぐ手を探す

#include<stdio.h>
#include<stdlib.h>

#define Size 12

//盤面入力関数
int Input_Board();
//箱数カウント関数
void Count_Box();
//ひとつ置いて勝利する手（盤面）を全て表示する関数.引数はどちらが先行かを示す"First"変数
int Game_Start_Win(int which);
//ひとつ置いたと勝利が確定する場所を探す.引数は現在白黒どちらが置くのか
int Find_Win_Point(int which);

//ゲーム終了フラグ
int Game_End_Flag = 10;

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
	//ゲームスタート先手情報を引数に
	Game_Start_Win(First);
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
		exit(0);
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
				printf(".");
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

	printf("0123456789AB\n");

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

//ひとつ置いて勝利する手を全て表示する関数.引数はどちらが先行かを示す"First"変数
//置いて勝つなら、その場所に逆の色を置けば防ぐことが可能
//白スタートならば、黒を置いて勝つところに白を置く考え
int Game_Start_Win(int which)
{
	//置く色を逆転
    if(which == 0)
	{
		which = 1;
	}else if(which == 1){
		which = 0;
	}

    printf("防ぎ手\n");

    /*************************set***************************/
    Current_Board[BP0][0] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("0\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("0は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP0][0] = -1;//初期化
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP1][1] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("1\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("1は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP1][1] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP2][2] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("2\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("2は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP2][2] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP3][3] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("3\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("3は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP3][3] = -1;
	/********************************************************/
	
	/*************************set***************************/
    Current_Board[BP4][4] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("4\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("4は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP4][4] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP5][5] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("5\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("5は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP5][5] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP6][6] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("6\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("6は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP6][6] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP7][7] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("7\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("7は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP7][7] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP8][8] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("8\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("8は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP8][8] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP9][9] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("9\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("9は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP9][9] = -1;
	/********************************************************/

	/*************************set***************************/
    Current_Board[BP10][10] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("A\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("Aは勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP10][10] = -1;
	/********************************************************/
    
    /*************************set***************************/
    Current_Board[BP11][11] = which;//8番目の正しい位置置く(暫定)縦側を変動させる？
    Find_Win_Point(which);//置いたとき勝利したかどうか
    if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
	{
	    //勝ったときその場所の番号を表示する
	    printf("B\n");
	    Game_End_Flag = 10;//他にも勝つ場所がある可能性があるため初期化しておく
	}else{//置いても勝ってない
		//printf("Bは勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
    }
	Current_Board[BP11][11] = -1;
	/********************************************************/


}

//一つ置いて勝つ場所を探す
int Find_Win_Point(int which)
{
    //ループ制御変数
    int i=0,j=0;



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


}
