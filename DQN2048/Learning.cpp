#include<bits/stdc++.h>
#include<raylib.h>
#include "Network.cpp"
#include <conio.h>
using namespace std;

network Thomas;
deque <pair <int, int>> x;
vector <vector <int>> weak;
vector <vector <int>> match_save;
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
	for (int episode = 0; episode < 8000; episode ++){	
		vector <int> X;
		match_save.clear();
		X = set_board();
		int f = 0;
		int tmptmp = step;
		
		int score = 0;
		while (true){	
			match_save.push_back(X);
			pair <int, vector <double>> tmp = Thomas.Action(X);
			int choice = tmp.first;
			vector <double> Q_Now = tmp.second;
			vector <int> nxt;
			if (choice == 0) nxt = moveUp(X);
			if (choice == 1) nxt = moveDown(X);
			if (choice == 2) nxt = moveLeft(X);
			if (choice == 3) nxt = moveRight(X);
			
			int reward = Rating(nxt) - Rating(X);
			score += reward/2;
			if (check_conner(nxt))
				reward += Max(nxt)/4;
			
			if (!check_conner(nxt))
				reward -= Max(nxt)/2;
			if (!check_link(SeTile(nxt), nxt))
				reward -= SeTile(nxt)/2;
			nxt = add(nxt);
			
			BeginDrawing();
    		ClearBackground(WHITE);
    	
   		 	for (int i = 0, X = 10, Y = 10; i < 16; i ++ ){
    		
  		  		DrawRectangle(X + 5, Y + 5, 198 - 5, 198 - 5, bg[nxt[i]]);
  		  		if (nxt[i] > 0)
				DrawText(TextFormat("%0i",(1 << nxt[i])), X + 100 - log10((1 << nxt[i])) * 15, Y + 70, 60, color[nxt[i]]);
			
    			X += 200;
    			if (i % 4 == 3) Y += 200, X = 10;
			}
			
			DrawText(TextFormat("Score: %0i", score), 40, 830, 80, BLACK);
			EndDrawing();
			
			if (isLosing(nxt)){
				int reward = Rating(nxt) ;
				system("cls");
				Thomas.store_trasition(X, -MaxTile(nxt), choice, nxt);
				cout << endl << "Game Over [ reward: " << score <<" reached: " << (1 << Max(nxt)) <<" Step: " << step - tmptmp<<" ]" <<endl;
				cout << "Are u sure: " << Thomas.eps * 100 <<endl;
				
				
				f = max(f, Max(nxt)); 
				x.push_back({f, score});
				num[f] ++;
				sum += score;
				if (x.size() > 100){
					num[x.front().first] --;
					sum -= x.front().second;
					x.pop_front();
				}
				break;
			}
			

			

			Thomas.store_trasition(X, reward, choice, nxt);
			step ++;
			int fred = 4;//(Thomas.eps > 0.7 ? 5 : (Thomas.eps > 0.3 ? 3 : 2));
			if (step > 200 && step % fred == 0){
				Thomas.learn();
			}

				
			X = nxt;
			if (kbhit()){
				if (getch() == 'k') return;
			}
		}

		
		for (int i = 1; i <= 17; i ++ )
			cout <<(1 << i) <<": " << num[i] <<"%" <<endl;
		cout <<"Average point: " << 1.0*sum/x.size() << endl;
		cout << step << " " << episode <<endl;
		cout << Thomas.layer[0][0][0] <<" " << Thomas.layer[0][0][1]<< endl;
		cout.flush();
		ofstream out("log.txt", ios::app);
		out << x.back().second << '\n';
		out.close();
		Thomas.save("Model2048.txt");
		if (kbhit()){
			if (getch() == 'k') return;
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
	
	const int screenWidth = 820;
    const int screenHeight = 950;
	
	InitWindow(screenWidth, screenHeight, "FirstGame");
	SetTargetFPS(60);
	
	Thomas.eps = 0.00;
	
	run();
	
	CloseWindow(); 
}
/*

*/

