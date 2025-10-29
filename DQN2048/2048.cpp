#include<bits/stdc++.h>
#include<raylib.h>
#include "Network.cpp"
#include <conio.h>
using namespace std;

network Thomas;
int weak_cnt = 0;
int sum = 0;
int num[20];

vector <Color> bg = {
	{238, 228, 218, 255},
	{237, 224, 200, 255},
	{237, 200, 80, 255},
	{237, 197, 63, 255},
	{246, 124, 95, 255},
	{246, 94, 59, 255},
	{237, 207, 114, 255},
	{237, 204, 97, 255},
	{242, 177, 121, 255},
	{245, 149, 99, 255},
	{237, 194, 46, 255},
};
vector <Color> color = {
	{119, 110, 101, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},
	{119, 110, 101, 255},
	{249, 246, 242, 255},
	{249, 246, 242, 255},


};
char *Direction[4] = {"UP", "DOWN", "LEFT", "RIGHT"};
bool check_conner(vector <int> &X){
	int k = Max(X);
	
	return (X[0] == k || X[3] == k || X[12] == k || X[15] == k);
}
bool check_link(int h, vector <int> &X){
	if (h == 0) return true;
	for (int i = 0; i < 16; i ++ )
		if (X[i] == h){
			bool check = false;
			if (i % 4 && X[i - 1] >= X[i]) check = true;
			if (i > 3 && X[i - 4] >= X[i]) check = true;
			if (i < 12 && X[i + 4] >= X[i]) check = true;
			if ((i + 1) % 4 && X[i + 1] >= X[i]) check = true;
			
			if (!check) return false;
		}
		
	return true;
}
void run(){
	
	int step = 0;
	int Auto = 0;
	for (int episode = 0; episode < 1; episode ++){	
		vector <int> board;
		board = set_board();
		int f = 0;
		int tmptmp = step;
		
		int score = 0;
		while (true){
			pair <int, vector <double>> tmp = Thomas.Action(board);
			int AI_recommend = tmp.first;
			vector <double> Q_Now = tmp.second;
			vector <int> nxt = board;
			


			
			BeginDrawing();
    		ClearBackground(WHITE);
    	
   		 	for (int i = 0, X = 10, Y = 10; i < 16; i ++ ){
    		
  		  		DrawRectangle(X + 5, Y + 5, 198 - 5, 198 - 5, bg[board[i]]);
  		  		if (board[i] > 0)
				DrawText(TextFormat("%0i",(1 << board[i])), X + 100 - log10((1 << board[i])) * 15, Y + 70, 60, color[board[i]]);
			
    			X += 200;
    			if (i % 4 == 3) Y += 200, X = 10;
			}
			DrawText(TextFormat("AI move: "), 850, 50, 80, BLACK);
			DrawText(TextFormat(Direction[AI_recommend]), 850, 150, 80, BLACK);
			DrawText(TextFormat("Score: %0i", score), 40, 830, 80, BLACK);
			DrawText(TextFormat("Use wasd to play"), 850, 850, 30, GRAY);
			DrawText(TextFormat("Press R for auto"), 850, 900, 30, GRAY);
			EndDrawing();
			
			bool moved = 0;
			if (char choice = GetCharPressed()){
				if (Auto == 0){
					if (choice == 'w') nxt = moveUp(board), moved = 1;
					if (choice == 's') nxt = moveDown(board), moved = 1;
					if (choice == 'a') nxt = moveLeft(board), moved = 1;
					if (choice == 'd') nxt = moveRight(board), moved = 1;
					
				}
					
				if (choice == 'k') return;
				if (choice == 'r') Auto = Auto^1;
			}
			if (Auto == 1){
				if (AI_recommend == 0) nxt = moveUp(board);
				if (AI_recommend == 1) nxt = moveDown(board);
				if (AI_recommend == 2) nxt = moveLeft(board);
				if (AI_recommend == 3) nxt = moveRight(board);	
				moved = 1;
			}
		
			
			if (moved){
				int reward = Rating(nxt) - Rating(board);
				score += reward/2;
				nxt = add(nxt);
				board = nxt;
			}
			
			if (isLosing(board)) break;

		}
	}
	
	
}
int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);
//	#define TASK ""
//	freopen(TASK".inp", "r", stdin);
//	freopen(TASK".out", "w", stdout);
	srand(time(0));
	Thomas.load("Model2048.txt");
	
	const int screenWidth = 1200;
    const int screenHeight = 950;
	
	InitWindow(screenWidth, screenHeight, "2048");
	SetTargetFPS(60);
	
	Thomas.eps = 0.00;
	
	run();
	
	CloseWindow(); 
}
/*

*/

