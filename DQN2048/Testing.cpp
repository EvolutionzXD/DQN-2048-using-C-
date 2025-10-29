#include<bits/stdc++.h>
#include "Network.cpp"
using namespace std;

network Peter;
double MAX = -100000;
double MIN = 100000;
int main(){
//	ios_base::sync_with_stdio(0);
//	cin.tie(0); cout.tie(0);
	
	vector <int> X = set_board();
	Peter.load("Model2048.txt");
	Peter.eps = 0.01;
	int score = 0;
	while (true){
			pair <int, vector <double>> tmp = Peter.Action(X);
			pair <int, vector <double>> tmp2 = Peter.Action(moveLeft(X));
			pair <int, vector <double>> tmp3 = Peter.Action(moveDown(X));
			int choice = tmp.first;
			vector <double> Q_Now = tmp.second;
			
			for (double i : Q_Now)
				cout << i <<" ";
			cout <<endl;
			for (double i : tmp2.second)
				cout << i <<" ";
			cout <<endl;
			for (double i : tmp3.second)
				cout << i <<" ";
			cout <<endl;
			vector <int> nxt;
			
			if (choice == 0) nxt = moveUp(X);
			if (choice == 1) nxt = moveDown(X);
			if (choice == 2) nxt = moveLeft(X);
			if (choice == 3) nxt = moveRight(X);
				
		
			score += (Rating(nxt) - Rating(X))/2;
			cout << "Score: " << score <<endl;
			nxt = add(nxt);	
			cout <<endl;
				for (int i = 0; i < nxt.size(); i ++){
					int h = 100000;
					int k = nxt[i];
					while (h > (1 << k)){
						cout << " ";
						h /= 10;
					}
					
					if (k > 0) cout << (1 << k);
					else cout << "_";
					
					if (i % 4 == 3) cout << endl;
				}
				cout <<"-------------------------" <<endl;
				cout <<"-------------------------" <<endl;
			
//			Peter.train(X, {0, 1, 2, 0});
			system("pause");
			
			if (isLosing(nxt)){
				break;
			}
			X = nxt;
	}
//	Peter.save("Model2049.txt");
//	vector <double> output;Peter.eps = 0.01;
	
}

