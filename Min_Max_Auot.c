//箱が4つ並ぶ、防ぐ、ミニマックス法の順でCPUが動く

/*----コマンドライン引数-------------------------------

1つ目:どちらが先手か(cpu or human)
2つ目:人間側の手の選択方法は何か
      (Auot:乱数 Min_Max:ミニマックス法 それ以外:手動)

-------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include<time.h>

/*****各種定数*****/

//盤面サイズ
#define Size 12
//ミニマックス法の深さ
//#define Depth 3
//人間側ミニマックス法の深さ
//#define H_Depth 1
//手数の上限
#define Number_of_Moves 144
//ゲーム木ノード総数
#define Game_Tree_Size 2000
//ミニマックスvs乱数の試行回数
#define Auot_Number 100

/******************/

/*****関数プロトタイプ宣言*****/

//盤面情報初期化
void Initialization_Board();
//ゲームスタート
int Game_Start();

//行動後の盤面を表示
void Show_Board();

//各プレイヤー行動which:何色が行動するか
int Player_Turn(int Board[Size][Size],int which);

//コンピュータ側の手を決定する(引数プレイヤーの白黒
int CPU_Turn(int which);

//勝敗判定which:どちら側の判定をするか
int Game_Judge(int which);

//引き分け判定
int Game_Judge_Draw();

//ゲーム木構築関数
void Create_Game_Tree();

//評価値算出関数
int Evaluation_Value_Calc();

//親ノードから子ノードを手数の数だけ展開する関数
int Create_Child_Node(int which,int Parent_Node_Number);

//ミニマックス法(引数:どちら側のミニマックス法か 0:CPU 1:Human)
int Min_Max(int which);

//評価関数引数:評価値を求めるノード番号
int Evaluation_Fanction(int which,int Node_Number,int Board_Label_Wide,int Board_Label_Height);

//各ノードの盤面構築 置く場所と何色を置くか
void Create_Board(int Position,int which,int node_number);

//ノード盤面情報の盤面コピー
int Board_Copy();

//ひとつ置いたとき、勝つ、もしくは相手が3つそろっているところに置く、行う(引数:置く色 黒:1 白:0)
int Find_Vic_Def_Point(int which);

//コンピュータ側の手を乱により決定する
int Player_Random(int Min ,int Max);

//縦のラインで置けるか確認する
int Check_Height(int Point);

//ゲーム木情報初期化
void Clear_Tree();

//ゲーム木構築時盤面に置くことができるか
int Check_Put(int CK_Node,int CK_Position);

//棋譜表示
int Show_Record();

//メモリ確保
//void Malloc(int n);



/******************************/

/*****グローバル変数*****/

//ミニマックス法の深さ
int Depth = 3;
//人間側ミニマックス法の深さ
int H_Depth = 3;
//cpu側ミニマックス法の深さ
int C_Depth = 3;

//現在の盤面
int Current_Board[Size][Size];

//積み重ね記憶ポインター配列
int *Board_Pointer;

//積み重ね記憶ポインター制御変数
int BP0=Size-1,BP1=Size-1,BP2=Size-1,BP3=Size-1,BP4=Size-1, BP5=Size-1,BP6=Size-1,
	BP7=Size-1,BP8=Size-1,BP9=Size-1,BP10=Size-1,BP11=Size-1;

//現在どちらが行動しているか
int Game_Side = 0;//0:白1:黒

//手数管理変数
int Number_of_Move_Count = 0;

//ゲーム終了フラグ
int Game_End_Flag = 10;//初期値-1,0,1以外制限なし

//現在構築中の木の親ノードのノードの深さ位置
int Current_Depth = 0;

//ゲーム木構築:次の深さの展開開始ノード算出フラグ:0:算出1:算出しない
int Next_Node_Flag = 0;

//次に展開する親ノード
int Next_Parent_Node;

//int *Node;//ノードの最大数分の配列を容易(念のため動的確保)ミニマックスで使用
int Node[Game_Tree_Size];
//評価値格納配列（配列要素指定でそのノードの評価値を格納）
//int *Evaluation_Value;
int Evaluation_Value[Game_Tree_Size];

//ゲーム木のノード番号設定に使用
int Node_Number = 0;

//CPUの手が決定したかどうかのフラグ(決定:1 未決定:0)
int CPU_Decision = 0;

//CPUの手を置く場合、そこに置けるかどうかのフラグ配列
//置ける:0 置けない:1
int Put_Enable[Size] = {0};

//プレイヤー側は乱数か手動か
int Play_Style = 0;//0手動　1乱数

//棋譜
int Game_Record[Number_of_Moves];
int Game_Record_Pointer = 0;

//対局自動化時で使用する変数
int Battle_Count = 0;
int P1_Win_Count = 0;//p1の勝利回数
int P2_Win_Count = 0;//p2の勝利回数
float Min_Max_Win_Rate = 0.0;//ミニマックス法の勝率
clock_t Time_Start,Time_End;

//ノードと盤面丸々を構造体で管理node数分確保 Game_Tree_Node[ノード番号]のようなアクセス方法
struct Game_Tree_Node
{
	//このノードのParentノード
	int This_Node_Parent;

	//このノードの深さ情報
	int This_Node_Depth;

	//遷移先ノード配列
	int State_Node[12];
	//遷移先ノード配列の制御変数(遷移するときの手にも利用)
	int State_Number;
	//遷移手(親から何の手でこのノードに遷移してくるか)
	int State_Hand;

	//親の評価値を求める際、maxとminどちらを求める側に入っているか
	//max:1 min:0
	int Max_or_Min;

	//(必要かは検討の必要アリ)盤面構築の際白黒どっちを置くのか
	//white:0 black:1
	int which;

	//このノードの盤面情報
	int This_Node_Board_Info[Size][Size];

	//このノードの評価値
	int This_Node_Evaluation_Value;

};

//動的に確保される構造体(実際に操作に使うもの)
///struct Game_Tree_Node *Use_Game_Tree;
struct Game_Tree_Node Use_Game_Tree[Game_Tree_Size];

/************************/

//main
//引数 human 人間側スタート　cpu側スタート
int main(int argc, char *argv[])
{
	int i;//対局自動化ループ変数

	//手を乱数で決定する為、乱数のシードを時刻とする
	srand((unsigned int)time(NULL));
	//メモリ確保
//	Malloc(1);


	Time_Start = clock();


	for(i=0;i<Auot_Number;i++)
	{
		//先手、後手設定
		if(strcmp(argv[1],"human") == 0)
		{
			Game_Side = 0;
		}else if(strcmp(argv[1],"cpu") == 0)
		{
			Game_Side = 1;
		}

	//プレイヤー側はミニマックス法か乱数か手動か
	if(strcmp(argv[2],"Auot") == 0)
	{
		Play_Style = 1;

	}else if(strcmp(argv[2],"Min_Max") == 0){

		Play_Style = 2;

	}else{

		Play_Style = 0;

	}

	//盤面情報初期化
	Initialization_Board();

	//ゲーム開始
	Game_Start();

	if(Game_End_Flag == -1)
		printf("Draw\n");
	

	if(Game_End_Flag == 0)
	{
		P1_Win_Count++;
//		printf("Player1 Win\n");
	}
	
	if(Game_End_Flag == 1)
	{
		P2_Win_Count++;
	//	printf("Player2 Win\n");
	}

	//盤面情報表示
	//Show_Board();
	//棋譜表示
//	printf("合計手数%d\n",Number_of_Move_Count);
//	Show_Record();

	//次のために初期化しておく
	//盤面情報初期化
	Initialization_Board();
	//ゲーム木初期化
	//Clear_Tree();

	Number_of_Move_Count = 0;
	Game_End_Flag = 10;
	BP0=Size-1,BP1=Size-1,BP2=Size-1,BP3=Size-1,BP4=Size-1, BP5=Size-1,BP6=Size-1,
	BP7=Size-1,BP8=Size-1,BP9=Size-1,BP10=Size-1,BP11=Size-1;

	//初期化
	Next_Parent_Node = 0;
	Node_Number = 0;
	Next_Node_Flag = 0;
	Current_Depth = 0;
	Game_Record_Pointer = 0;

	Clear_Tree();


	}

	Time_End = clock();

	printf("Depth%d %d回試行 %s先手\n",Depth,Auot_Number,argv[1]);
	printf("人間側勝利回数 %d \n",P1_Win_Count);
	printf("CPU側ミニマックス法勝利回数 %d \n",P2_Win_Count);

	printf("人間側の勝率:%3.2f ％\n",((float)P1_Win_Count / Auot_Number) * 100);
	printf("CPU側のミニマックス法勝率:%3.2f ％\n",((float)P2_Win_Count / Auot_Number) * 100);
	printf("処理時間%.5fsec\n",(double)(Time_End - Time_Start) / CLOCKS_PER_SEC);



	
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
			//if(Current_Board[i][j] == -1)
				//printf(".");
		//	printf("%d",Current_Board[i][j]);
		}
	//	printf("\n");

	}

	//盤番号表示
//	printf("0123456789AB\n");

	//棋譜配列初期化
	for(i=0;i<144;i++)
	{
		Game_Record[i] = -1;

	}

}

//ゲームの軸
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

//各プレイヤー行動
int Player_Turn(int Board[Size][Size],int which)
{
	char Input_Board_Number_C[256] , *error;//入力は初めここに格納後に判定eは変換不可時の返還先
	int Input_Board_Number;
	int Domination_Board_Pointer;//操作するボードポインタ記憶
	int Player_Number = 1;//表示するプレイヤー番号
	int i,j;

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

	//	printf("Player%d:",Player_Number);
		//コマンドライン引数でつ目がAoutで乱数　Man で手動
		if(Play_Style == 0 && which == 0)
		{
		//文字列として格納
		scanf("%s",&Input_Board_Number_C);
		//文字列を16進変換
		Input_Board_Number = strtol(Input_Board_Number_C,&error,16);

		}else if(Play_Style == 1 && which == 0)//コマンドライン引数よりプレイヤー側が乱数の場合
		{
			//4つ並ぶ場所を探す
			Input_Board_Number = Find_Vic_Def_Point(0);
			//4つ並ぶことを防ぐ
			if(Input_Board_Number == -1)
			{
				Input_Board_Number = Find_Vic_Def_Point(1);
				Game_End_Flag = 10;
			}

			//ランダム
			//置けるまで乱数発生
			while(1)
			{

				if(Input_Board_Number == -1)
					Input_Board_Number = Player_Random(0,11);

				if(Current_Board[0][Input_Board_Number] == -1)
				{
				//	printf("%d\n",Input_Board_Number);
					break;
				}
				
			//	printf("%d\nに置けません",Input_Board_Number);
				Input_Board_Number = -1;
			}
		
		
		}else if(Play_Style == 2 && which == 0){//コマンドライン引数よりプレイヤーがミニマックス法の場合
			//4つ並ぶ場所を探す
			Input_Board_Number = Find_Vic_Def_Point(0);
			//4つ並ぶことを防ぐ
			if(Input_Board_Number == -1)
			{
				Input_Board_Number = Find_Vic_Def_Point(1);
				Game_End_Flag = 10;
			}
			//ミニマックス法
			if(Input_Board_Number == -1)
			{

			/*------------------------------------------------------------------------*/
			//ノード0の初期化
			Use_Game_Tree[0].State_Number = 0;//遷移先ノード配列制御変数初期化
			Use_Game_Tree[0].This_Node_Depth = 0;//深さ情報を書き込
			Use_Game_Tree[0].This_Node_Parent = -1;//開始ノードの親は無しとし-1で表す
			//ノード0にCurrent_Board情報をコピー
			for(i=0;i<Size;i++)
			{
				for(j=0;j<Size;j++)
				{
					Use_Game_Tree[0].This_Node_Board_Info[i][j] = Current_Board[i][j];
				}
			}
			/*------------------------------------------------------------------------*/

			//Node配列初期化
			for(i=0;i<Game_Tree_Size;i++)
				Node[i] = -1;


			//ゲーム木作成
			Create_Child_Node(1,0);



			//ミニマックス法実行
			Input_Board_Number = Min_Max(1);


			//初期化
			Next_Parent_Node = 0;
			Node_Number = 0;
			Next_Node_Flag = 0;
			Current_Depth = 0;

			}

		
		}else if(which == 1)
		{
		
			//CPUの手決定フラグをリセット
			CPU_Decision = 0;
			Input_Board_Number = CPU_Turn(which);//which=1のときcpuのターン(黒置きがcpuで固定)
		//	printf("%d\n",Input_Board_Number);

		}

		//ここでは単純に入力を評価する人間とCPUでは別で判定する
	if(which == 0 && Play_Style == 0 && Input_Board_Number < 12  && strcmp("\0",error) == 0 && Current_Board[0][Input_Board_Number] == -1)
		{
			//条件に合えば抜ける
			break;
		}
		if(which == 1 &&  Input_Board_Number < 12){
			break;
			
		}
		if(which == 0 && Play_Style == 1 && Current_Board[0][Input_Board_Number] == -1){
			break;

		}
		if(which == 0 && Play_Style == 2)
		{			
		//	printf("%d\n",Input_Board_Number);
			break;
		}


		//break;
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

	Game_Record[Game_Record_Pointer] = Input_Board_Number;
	Game_Record_Pointer++;
	//盤面情報表示
//	Show_Board();

	//勝敗判定
	Game_Judge(which);
	//手数カウント
	Number_of_Move_Count += 1;
	//手数カウント上限で引き分け判定
	if(Number_of_Move_Count == Number_of_Moves)
	{
		//引き分け判定とする
		Game_End_Flag = -1;
		return;
	}
}

//盤面表示
void Show_Board()
{
	//ループ制御変数
	int i,j;


	printf("\n");

	
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
			}else if(Current_Board[i][j] == -1){

				printf(".");
			}
		}
		printf("\n");
	}

	//盤番号表示
	printf("0123456789AB\n");
	if(Current_Board[11][0] == -1)
	{
		printf("%d %d\n",Current_Board[0][11],Current_Board[11][0]);
	}


}

//勝敗判定引数はどちらのターンでjudgeしているか(0,1で区別しているもの)
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
				//	printf("右\n");
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
					//printf("左\n");
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
				//	printf("上\n");
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
				//	printf("下\n");
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
			//		printf("右上\n");
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
			//		printf("左上\n");
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
				//	printf("右下\n");
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
				//	printf("左下\n");
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

//CPU`メイン
//4つ並ぶ手、防ぐ手、ミニマックス法の順で手を求める
int CPU_Turn(int which)
{
	int Number =  -1;//CPU側の手(初期値で手としてふさわしくない値を入れておく)
	int i,j;//ループ制御変数

	//手が決定されていない場合
	if(CPU_Decision == 0)
	{
		//4つ並ぶ手CPUが単純に勝つ場所を探す
		Number = Find_Vic_Def_Point(1);
		
		if(Number != -1)
		{
			//手が決定したらフラグセットしておく
			CPU_Decision = 1;
		}

	}
	
	if(CPU_Decision == 0 )
	{
		
		//並ぶことを防ぐ手(相手(白)が置いて勝つ場所を探し、そこに自分の元の色を置いて防ぐこととする)
		Number = Find_Vic_Def_Point(0);
				
		if(Number != -1)
		{
			//手が決定したらフラグセットしておく
			CPU_Decision = 1;
			//防ぐ場合、勝利フラグは10に戻しておく
			Game_End_Flag = 10;
		}

	}

	if(CPU_Decision == 0)
	{

		

	
		//ミニマックス
		
		/*------------------------------------------------------------------------*/
		//ノード0の初期化
		Use_Game_Tree[0].State_Number = 0;//遷移先ノード配列制御変数初期化
		Use_Game_Tree[0].This_Node_Depth = 0;//深さ情報を書き込
		Use_Game_Tree[0].This_Node_Parent = -1;//開始ノードの親は無しとし-1で表す
		//ノード0にCurrent_Board情報をコピー
		for(i=0;i<Size;i++)
		{
			for(j=0;j<Size;j++)
			{
				Use_Game_Tree[0].This_Node_Board_Info[i][j] = Current_Board[i][j];
			}
		}
		/*------------------------------------------------------------------------*/

		//Node配列初期化
		for(i=0;i<Game_Tree_Size;i++)
			Node[i] = -1;


		//ゲーム木作成
		Create_Child_Node(0,0);



		//ミニマックス法実行
		Number = Min_Max(0);
	
		//手が決定したらフラグセットしておく
		CPU_Decision = 1;
	
	}


	//初期化
	Next_Parent_Node = 0;
	Node_Number = 0;
	Next_Node_Flag = 0;
	Current_Depth = 0;
		//手を返す
		return Number;



}

//親ノードから子ノードを手数の数だけ展開する関数
int Create_Child_Node(int which ,int Parent_Node_Number)
{
	int i,j;

	//引数からゲーム木の構築サイズを可変
	if(which == 1)
	{
		Depth = H_Depth;
	}else{

		Depth = C_Depth;

	}


	//次の深さの展開開始ノード番号を算出する(新しい深さに突入した初めの一回目だけ
	if(Next_Node_Flag == 0)
	{
		Next_Parent_Node = Parent_Node_Number + pow((double)Size,(double)Current_Depth);
	//	printf("Next Parent:%d\n",Next_Parent_Node);
		Next_Node_Flag = 1;//ロック
	}

	//隣同士のノード番号が同じ親を持つかどうかを判定
	//異なれば深さの区切りとなり、終了判定が入る
	if(Parent_Node_Number > 0
		&& Use_Game_Tree[Parent_Node_Number - 1].This_Node_Parent != Use_Game_Tree[Parent_Node_Number].This_Node_Parent
		 && Current_Depth == Depth)
	{

	//		printf("終了\n");

			return 0;



	}else{

		//隣同士のノードが同じ親を持ち、まだ深さが達していない場合、子ノードからさらに深く展開する
		
			//親ノードから幅優先的に手数だけノードを展開する
		for(i=0;i<Size;i++)
			{
				//ノード展開親情報付加も必須
				Node_Number++;
				//親ノードの遷移先を更新
				Use_Game_Tree[Parent_Node_Number].State_Node[Use_Game_Tree[Parent_Node_Number].State_Number] = Node_Number;
				Use_Game_Tree[Parent_Node_Number].State_Number++;
				//展開された子に情報追加と初期化
				Use_Game_Tree[Node_Number].This_Node_Parent = Parent_Node_Number;
				Use_Game_Tree[Node_Number].State_Number = 0;
				Use_Game_Tree[Node_Number].This_Node_Depth = Current_Depth + 1;
				Use_Game_Tree[Node_Number].State_Hand = i;
				//printf("%d\n",Use_Game_Tree[Node_Number].This_Node_Depth);
				for(j=0;j<Size;j++)
				{
					//遷移先ノード番号を-1で初期化しておく
					Use_Game_Tree[Node_Number].State_Node[j] = -1;
				}
				
				//作成したノード情報の視覚化
			//	printf("作成ノード%d\n親ノード%d\n遷移手%d\n",Node_Number,Parent_Node_Number,i);
				//現在の盤面のコピー
				Board_Copy(Node_Number);
				//このノードの盤面情報構築と表示
				if((Current_Depth + 1) % 2 != 0)//深さ奇数で白置き
				{
					Create_Board(i,1,Node_Number);

				}else if((Current_Depth + 1) % 2 == 0){

					Create_Board(i,0,Node_Number);

				}
			}


			//親の切り替わりポイント
			if(Parent_Node_Number != Next_Parent_Node - 1)
			{
			//次の親ノード情報を初期化しておく
				Use_Game_Tree[Parent_Node_Number + 1].State_Number = 0;
				Create_Child_Node(which,Parent_Node_Number + 1);
			
			}else{
				
	
				Current_Depth++;
				Next_Node_Flag = 0;//フラグリセット
				//ノード情報初期化処理
				
				Use_Game_Tree[Next_Parent_Node].State_Number = 0;
				Create_Child_Node(which,Next_Parent_Node);
		
			}



		}


	return 0;
}

//評価関数引数:色（1:黒0:白）:評価値を求めるノード番号:盤面ラベルの横:盤面ラベルの縦
int Evaluation_Fanction(int which,int node_number,int Board_Label_Wide,int Board_Label_Height)
{
	//色の数
	int Collor_Count = 0;
	//各方向に3つそろうことがわかった場合、フラグをセットし、評価値を上昇させる
	int R = 0;//右
	int R_U = 0;//右上
	int R_D = 0;//右下

	int L = 0;//左
	int L_U = 0;//左上
	int L_D = 0;//左下

	int U = 0;//上
	int D = 0;//下

	//置いた場所の縦横から盤面を読めるかどうか


	//右上2
	if(Board_Label_Wide < 11 && Board_Label_Height > 0)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height - 1][Board_Label_Wide + 1] == which)
		{
			Collor_Count+=25;
			//右上3
			if(Board_Label_Wide < 10 && Board_Label_Height > 1)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height - 2][Board_Label_Wide + 2] == which)
				{
					Collor_Count+=500;
					R_U = 1;//右上に3つあることを記憶

				}
			}
		}
	}

	//右下2
	if(Board_Label_Wide < 11 && Board_Label_Height < 11)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height + 1][Board_Label_Wide + 1] == which)
		{
			Collor_Count+=25;
			//右下3
			if(Board_Label_Wide < 10 && Board_Label_Height < 10)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height + 2][Board_Label_Wide + 2] == which)
				{
					Collor_Count+=500;
					R_D = 1;//右上に3つあることを記憶

				}
			}
		}
	}

	/************************************************************************/
		//左上2
	if(Board_Label_Wide > 0 && Board_Label_Height > 0)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height - 1][Board_Label_Wide - 1] == which)
		{
			Collor_Count+=25;
			//左上3
			if(Board_Label_Wide > 1 && Board_Label_Height > 1)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height - 2][Board_Label_Wide - 2] == which)
				{
					Collor_Count+=500;
					L_U = 1;//右上に3つあることを記憶

				}
			}
		}
	}



	//左下2
	if(Board_Label_Wide > 0 && Board_Label_Height < 11)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height + 1][Board_Label_Wide - 1] == which)
		{
			Collor_Count+=25;
			//左下3
			if(Board_Label_Wide > 1 && Board_Label_Height < 10)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height + 2][Board_Label_Wide - 2] == which)
				{
					Collor_Count+=500;
					R_D = 1;//右上に3つあることを記憶

				}
			}
		}
	}
	
	/************************************************************************/


	//上2
	if(Board_Label_Height > 0)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height - 1][Board_Label_Wide] == which)
		{
			Collor_Count+=25;
			//上3
			if(Board_Label_Height > 1)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height - 2][Board_Label_Wide] == which)
				{
					Collor_Count+=500;
					U = 1;//右上に3つあることを記憶

				}
			}
		}
	}



	//下2
	if(Board_Label_Height < 11)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height + 1][Board_Label_Wide] == which)
		{
			Collor_Count+=25;
			//下3
			if(Board_Label_Height < 10)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height + 2][Board_Label_Wide] == which)
				{
					Collor_Count+=500;
					D = 1;//右上に3つあることを記憶

				}
			}
		}
	}
	
	/************************************************************************/

		//右2
	if(Board_Label_Wide < 11)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height][Board_Label_Wide + 1] == which)
		{
			Collor_Count+=25;
			//右3
			if(Board_Label_Wide < 10)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height][Board_Label_Wide + 2] == which)
				{
					Collor_Count+=500;
					R = 1;//右上に3つあることを記憶

				}
			}
		}
	}



	//左2
	if(Board_Label_Wide > 0)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height][Board_Label_Wide - 1] == which)
		{
			Collor_Count+=25;
			//左3
			if(Board_Label_Wide > 1)
			{
				if(Use_Game_Tree[node_number].This_Node_Board_Info[Board_Label_Height][Board_Label_Wide - 2] == which)
				{
					Collor_Count+=500;
					L = 1;//右上に3つあることを記憶

				}
			}
		}
	}
	

	//3つ並びフラグの数に応じてスコア加算
	if(R_U == 1)
		Collor_Count += 17500;

	if(R_D == 1)
		Collor_Count += 17500;

	if(L_U == 1)
		Collor_Count += 17500;

	if(L_D == 1)
		Collor_Count += 17500;

	if(U == 1)
		Collor_Count += 5000;

	if(D == 1)
		Collor_Count += 5000;

	if(R == 1)
		Collor_Count += 10000;

	if(L == 1)
		Collor_Count += 10000;

	//ラベル別評価値
	//置く場所によって評価値を個別に加算する
	switch(Board_Label_Wide)
	{
	case 0:
		Collor_Count += 1;
		break;
	case 1:
		Collor_Count += 3;
		break;
	case 2:
		Collor_Count += 5;
		break;
	case 3:
		Collor_Count += 7;
		break;
	case 4:
		Collor_Count += 9;
		break;
	case 5:
		Collor_Count += 11;
		break;
	case 6:
		Collor_Count += 10;
		break;
	case 7:
		Collor_Count += 8;
		break;
	case 8:
		Collor_Count += 6;
		break;
	case 9:
		Collor_Count += 4;
		break;
	case 10:
		Collor_Count += 2;
		break;
	case 11:
		Collor_Count += 0;
		break;



	}
		


	//置いた周りにいくつ同じ色があったかを返す
//	printf("EV=%d\n",Collor_Count);
	return Collor_Count;


}



//ミニマックス法:引数次の親ノード番号:どっちが箱を億番になっているか
int Min_Max(int which)
{
	//ループ制御変数
	int i,j;
	int Start_Node_Number = 1;//最大深さの最小ノード番号
	int Node_Number_Temp = 0;//評価値を格納するため、ノード番号を一時保存しておく
	int Current_Parent = 0;//現在評価値を求めている親のノード番号

	/***比較用変数***/
	int Comp_Node_Number = 0;
	int Comp_Node_EV = 0;
	/****************/

	int Sarch_Depth = Depth;//評価値を求める子の深さ
	int Sarch_Node_Number;//Comp_Node_Number+1を格納し、ループの開始値を指定する
	int Hand = 0;//最終的に返す

	//引数から深さを可変
	
	if(which == 1)
	{
		Depth = H_Depth;
	}else{
		Depth = C_Depth;
	}


	//step1
	while(1)
	{
		if(Use_Game_Tree[Start_Node_Number].State_Node[0] != -1)
		{
			Start_Node_Number = Use_Game_Tree[Start_Node_Number].State_Node[0];

		}else{
	//		printf("最大深さの最小ノード番号%d\n",Start_Node_Number);
			Current_Parent = Use_Game_Tree[Start_Node_Number].This_Node_Parent;
		//	printf("評価値を求める親ノード番号%d\n",Current_Parent);
			break;
		}

	}

	while(1)
	{
		//step2
		for(i=0;i<Game_Tree_Size;i++)
		{
			//ノード配列を-1で初期化しておく
			Node[i] = -1;
		}

		//step3
		for(i=0;i<Size;i++)
		{
			//親ノードから各遷移先ノード番号を一時確保
			Node_Number_Temp = Use_Game_Tree[Current_Parent].State_Node[i];
			//遷移先ノードの評価値をNode配列に格納
			if(Use_Game_Tree[Node_Number_Temp].This_Node_Evaluation_Value != 0)
				Node[Node_Number_Temp] = Use_Game_Tree[Node_Number_Temp].This_Node_Evaluation_Value;
		}

		//step4
		for(i=0;i<Game_Tree_Size;i++)
		{
			if(Node[i] != -1)
			{
				Comp_Node_Number = i;
				Comp_Node_EV = Node[i];
				break;
			}
		}

		//step5
		//深さ偶数時、そのひとつしたのノード群からは最大のものをとる
		//深さ奇数時、そのひとつしたのノード軍からは最小のものをとる
		Sarch_Node_Number = Comp_Node_Number + 1;
		for(i = Sarch_Node_Number;i<Game_Tree_Size;i++)
		{
			if(Sarch_Depth == 0 || Sarch_Depth % 2 == 0 && Node[i] != -1 && Node[i] < Node[Comp_Node_Number] && Node[i] != 0)//評価値0は探索対象外
			{
				Comp_Node_Number = i;
				Comp_Node_EV = Node[i];
			 
			}else if(Sarch_Depth % 2 != 0 && Node[i] != -1 && Node[i] > Node[Comp_Node_Number] && Node[i] != 0)//評価値0は探索対象外
			{
				Comp_Node_Number = i;
				Comp_Node_EV = Node[i];
			}
		}

		//step6
		Use_Game_Tree[Use_Game_Tree[Comp_Node_Number].This_Node_Parent].This_Node_Evaluation_Value
			= Use_Game_Tree[Comp_Node_Number].This_Node_Evaluation_Value;
		
		//printf("ノード%dの評価値は%dと決定しました\n",Use_Game_Tree[Comp_Node_Number].This_Node_Parent
			//,Use_Game_Tree[Comp_Node_Number].This_Node_Evaluation_Value);

		//ノード0の評価値が求まったら大枠のwhileから抜ける
		if(Use_Game_Tree[Comp_Node_Number].This_Node_Parent == 0)
			break;

		//step7
		//親ノード更新
		if(Use_Game_Tree[Current_Parent].This_Node_Depth == Use_Game_Tree[Current_Parent + 1].This_Node_Depth)
		{
			Current_Parent = Current_Parent + 1;
		//	printf("次の親ノード番号%d\n",Current_Parent);

		}else{
			Start_Node_Number = 0;
			for(i=0;i<Sarch_Depth - 1;i++)
			{
				Start_Node_Number += pow((double)Size,(double)i);
			}
			Sarch_Depth -= 1;
			Current_Parent = Use_Game_Tree[Start_Node_Number].This_Node_Parent;
			//printf("次の親ノード番号%d\n",Current_Parent);




		}

	}
	//ノードの全評価値をリスト表示
	for(i=0;i<Game_Tree_Size;i++)
	{
	//	printf("%d   %d\n",i,Use_Game_Tree[i].This_Node_Evaluation_Value);
	}

	//step8
	//ノード0の評価値が求め終わっているので最適な手を返す
//	printf("Step8\n");
	//ノード0の遷移先ノードの評価値を改めて表示
	for(i=0;i<Size;i++)
	{
 		if(Current_Board[0][i] == -1  && Use_Game_Tree[0].This_Node_Evaluation_Value == Use_Game_Tree[i+1].This_Node_Evaluation_Value)
		{
		//	printf("ノード%d:評価値%d\n",Use_Game_Tree[0].State_Node[i],Use_Game_Tree[Use_Game_Tree[0].State_Node[i]].This_Node_Evaluation_Value);
			return Use_Game_Tree[i + 1].State_Hand;
		}
	}

	//returnされなかった場合,置ける場所に適当に置く

//	printf("選択失敗\n");
	while(1)
	{
		Hand = Player_Random(0,11);
		if(Current_Board[0][Hand] == -1)
			return Hand;
	}
	return;

}

//各ノードの盤面構築(箱置)
void Create_Board(int position, int which,int node_number)
{
	//position:置く場所
	int i,j;

	//ノード構造体の盤面情報に箱を置く
	for(i=0;i<Size;i++)
	{
		if(Use_Game_Tree[node_number].This_Node_Board_Info[i][position] != -1)
		{
			if(which == 0)
			{
				//白置き
				if(/*Current_Board[i-1][position] == -1*/ i != 0)
				{
				//	printf("%dに置いた\n",position);
					Use_Game_Tree[node_number].This_Node_Board_Info[i-1][position] = 0;
					//置いたらその場所から評価値を求め、構造体要素に格納する
					Use_Game_Tree[node_number].This_Node_Evaluation_Value = 
						Evaluation_Fanction(which,node_number,position,i-1);
					break;
				}else{
				//	printf("%dに置けない\n",position);
				//	printf("ノード%dは除外\n",node_number);
					Use_Game_Tree[node_number].This_Node_Evaluation_Value = 0;//おくことができない局面の場合評価値を0にして除外とする
					break;

				}



			}else if(which == 1){
				//黒置き
				if(/*Current_Board[i-1][position] == -1 */i != 0)
				{
					Use_Game_Tree[node_number].This_Node_Board_Info[i-1][position] = 1;
					//置いたらその場所から評価値を求め、構造体要素に格納する
					Use_Game_Tree[node_number].This_Node_Evaluation_Value = 
						Evaluation_Fanction(which,node_number,position,i-1);
					break;
				}else{
				//	printf("%dに置けない\n",position);
				//	printf("ノード%dは除外\n",node_number);
					Use_Game_Tree[node_number].This_Node_Evaluation_Value = 0;//おくことができない局面の場合評価値を0にして除外とする
					break;
				}
		

			}
		}else if(i == Size-1){
		//置く場所に何も置かれていないとき、一番下に置く
			if(which == 0)
			{
				//白置き
				Use_Game_Tree[node_number].This_Node_Board_Info[Size-1][position] = 0;
				//置いたらその場所から評価値を求め、構造体要素に格納する
				Use_Game_Tree[node_number].This_Node_Evaluation_Value = 
					 Evaluation_Fanction(which,node_number,position,Size-1);
				break;
			
			}else if(which == 1){
				//黒置き
				Use_Game_Tree[node_number].This_Node_Board_Info[Size-1][position] = 1;
				//置いたらその場所から評価値を求め、構造体要素に格納する
				Use_Game_Tree[node_number].This_Node_Evaluation_Value = 
					 Evaluation_Fanction(which,node_number,position,Size-1);
				break;
			
			}
		
		}

	}
	/*
	//おき終ったら盤面表示デバッグ

	for(i = 0;i<Size;i++)
	{
		for(j = 0;j<Size;j++)
		{

			if(Use_Game_Tree[node_number].This_Node_Board_Info[i][j] == 0)
			{
				printf("0");
			}else if(Use_Game_Tree[node_number].This_Node_Board_Info[i][j] == 1)
			{

				printf("X");
			}else{

				printf(".");
					
			}

		}

		printf("\n");

	}


	//ラベル表示
	printf("0123456789AB\n");
	//評価値表示
	printf("深さ%d:ノード%d:評価値:%d\n",Current_Depth,node_number,Use_Game_Tree[node_number].This_Node_Evaluation_Value);
	*/

}
//親ノードの盤面情報を子ノードにコピー
int Board_Copy(int node_number)
{
	int i,j;
	

	for(i=0;i<Size;i++)
	{

		for(j=0;j<Size;j++)
		{
			Use_Game_Tree[node_number].This_Node_Board_Info[i][j] = 
				Use_Game_Tree[  Use_Game_Tree[node_number].This_Node_Parent  ].This_Node_Board_Info[i][j];
			
		}

	}


}

//ひとつ置いたとき、勝つ、もしくは相手が3つそろっているところを返す(引数:置く色 黒:1 白:0)
int Find_Vic_Def_Point(int which)
{
	int BP = -1;//ボードポインター
	int i,j;//ループ制御変数
	int Current_Collor = -1;//盤面中の現在の色をバックアップ

	for(i=0;i<Size;i++)
	{
		BP = -1;//初期化
		for(j=0;j<Size;j++)
		{
			//盤面を見て-1以外のところになったとき、その一個上をBPとする
			if(Current_Board[j][i] != -1)
			{
				if(j != 0)
				{
					BP = j;
					BP--;
				break;
				}else{//一番上が埋まっている場合は飛ばす
					BP = -2;//盤面空白情報とは異なる値を持たせておく

				}
			}



		}
		//ここの段階でBPが-1の場合、一番下の11に置くようにする
		if(BP == -1)
		{
			BP = 11;
		}
		if(BP != -2)
		{
		//現在の色を変数にバックアップ
		Current_Collor = Current_Board[BP][i];
		Current_Board[BP][i] = which;
		Game_Judge(which);//置いたとき勝利したかどうか
		if(Game_End_Flag == 0 || Game_End_Flag == 1)//勝てばその位置
		{
		//	Current_Board[BP][i] = -1;//一度置いた手を戻す
			Current_Board[BP][i] = Current_Collor;
			//勝利の場合、手を返す
			return i;
		}else{//置いても勝ってない
		//printf("0は勝ち手ではない\n");//何もない場合ここは0からｂまで表示される(暫定)
			//ロールバック
			Current_Board[BP][i] = Current_Collor;
		}

		}
	}

	//勝つ場所がない場合-1を返しておく
	return -1;


}

//コンピュータ側の手を乱により決定する
int Player_Random(int Min ,int Max)
{
	int Number;//CPU側の手

	Number = Min + (int)(rand() * (Max - Min + 1.0)/(1.0 + RAND_MAX));

//	printf("CPUの手:%d\n",Number);

	//範囲乱数公式使用
	return Number;

}

//ゲーム木の初期化
void Clear_Tree()
{
	int i,j,k;

	for(i=1;i<Game_Tree_Size;i++)
	{
		Use_Game_Tree[i].State_Hand = -1;
		Use_Game_Tree[i].This_Node_Evaluation_Value = -1;
		Use_Game_Tree[i].This_Node_Parent = -1;
		for(j=0;j<Size;j++)
		{
			Use_Game_Tree[i].State_Node[j] = -1;
			for(k=0;k<Size;k++)
			{
				Use_Game_Tree[i].This_Node_Board_Info[j][k] = -1;
			}

		}


	}

	return;

}

//ゲーム木構築時、箱を置けるかを判断する
int Check_Put(int CK_Node,int CK_Position)
{
		if(Use_Game_Tree[CK_Node].This_Node_Board_Info[0][CK_Position] == -1)
		{

			return 0;//置けるならば1を返す

		}else{

			return 1;//置けないならば0を返す


		}


}
/*
//メモリ確保
void Malloc(int n)
{
	if(n == 1)
	{
	Use_Game_Tree = (struct Game_Tree_Node*)malloc(sizeof(struct Game_Tree_Node) * Game_Tree_Size);
	printf("%d\n",Use_Game_Tree);
	if(Use_Game_Tree == NULL)
	{
		//メモリが取れない場合その旨を表示し、強制終了させる
		//free(Use_Game_Tree);
		printf("memory missed\n");
		exit(0);	
	}
	//各ノードの評価値を格納する配列を動的確保
	//Node動的確保
	Evaluation_Value = (int*)malloc(sizeof(int) * Game_Tree_Size);
	printf("%d\n",Evaluation_Value);
	if(Evaluation_Value == NULL)
	{
		//free(Evaluation_Value);
		printf("error\n");
		exit(0);
	}
	
	//Node配列動的確保
	Node = (int*)malloc(sizeof(int) * Game_Tree_Size);
	printf("%d\n",Node);
	if(Node == NULL)
	{
		//free(Node);
		printf("error\n");
		exit(0);
	}


	}else if(n == 0)
	{

	//	free(Use_Game_Tree);
	//	free(Evaluation_Value);
	//	free(Node);


	}

}


*/

//棋譜表示
int Show_Record()
{
	int i,j;
	j=1;

	printf("経過ターン:先手 後手\n");
	for(i=0;i<Number_of_Move_Count;i++)
	{
		printf("%dTurn:%d %d\n",j,Game_Record[i],Game_Record[i+1]);
		i++;
		j++;


	}

}






