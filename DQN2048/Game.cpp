#include<bits/stdc++.h>
#define ll long long
#define endl "\n"
using namespace std;

vector <int> set_board(){
	vector <int> X(16, 0);
	
	int a = rand() % 15;
	int b = a + 1 + rand() % (15 - a);
	
	X[a] = rand() % 100 > 10 ? 1 : 2;
	X[b] = rand() % 100 > 10 ? 1 : 2;
	
	return X;
}

int Max(vector <int> X){
	int res = 0;
	
	for (int i : X)
		res = max(res, i);
	
	return res;
}

vector <int> add(vector <int> X){
	int pos = rand() % 16;
	while (X[pos] != 0) pos = rand() % 16;
	X[pos] = rand() % 100 > 50? 1 : 2;
	
	return X;
}

void SHOW_BOARD(vector <int> &X){
	for (int i = 0; i < X.size(); i ++ ){
		if (i % 4 == 0) cout <<endl;
		 cout << X[i] <<" ";
		
	}
}

vector <int> moveUp(vector <int> X){
	int UP[4] = {0, 1, 2, 3};
	
	for (int i = 4; i < 16; i ++ ){
		if (X[i] == 0) continue;
		if (i <= UP[i % 4]) continue;
		
		if (X[i] == X[UP[i % 4]]){
			X[UP[i % 4]] ++;
			UP[i % 4] += 4;
			X[i] = 0;
			continue;
		}
		
		if (X[i] != X[UP[i % 4]] && X[UP[i % 4]] != 0)
			UP[i % 4] += 4;
		
		if (X[UP[i % 4]] == 0)
			swap(X[i], X[UP[i % 4]]);
	}
	
	return X;
}
vector <int> moveDown(vector <int> X){
	int DOWN[4] = {12, 13, 14, 15};
	
	for (int i = 11; i >= 0; i -- ){
		if (X[i] == 0) continue;
		if (i >= DOWN[i % 4]) continue;
		
		if (X[i] == X[DOWN[i % 4]]){
			X[DOWN[i % 4]] ++;
			DOWN[i % 4] -= 4;
			X[i] = 0;
			continue;
		}
		
		if (X[i] != X[DOWN[i % 4]] && X[DOWN[i % 4]] != 0)
			DOWN[i % 4] -= 4;
		
		if (X[DOWN[i % 4]] == 0)
			swap(X[i], X[DOWN[i % 4]]);
	}
	
	return X;
}

vector <int> moveLeft(vector <int> X){
	int lf = 0;
	for (int i = 0; i < 16; i ++ ){
		if (i % 4 == 0) lf = i;
		if (X[i] == 0) continue;
		if (i <= lf) continue;
		
		if (X[i] == X[lf]){
			X[lf] ++;
			lf ++;
			X[i] = 0;
			continue;
		}
		
		if (X[i] != X[lf] && X[lf] != 0)
			lf ++;
		
		if (X[lf] == 0)
			swap(X[i], X[lf]);
	}
	
	return X;
}

vector <int> moveRight(vector <int> X){
	int rt = 0;
	for (int i = 15; i >= 0; i -- ){
		if (i % 4 == 3) rt = i;
		if (X[i] == 0) continue;
		if (i >= rt) continue;
		
		if (X[i] == X[rt]){
			X[rt] ++;
			rt --;
			X[i] = 0;
			continue;
		}
		
		if (X[i] != X[rt] && X[rt] != 0)
			rt --;
		
		if (X[rt] == 0)
			swap(X[i], X[rt]);
	}
	
	return X;
}
vector <bool> State_Check(vector <int> X){
	vector <bool> res;
	
	res.push_back((X != moveUp(X)));
	res.push_back((X != moveDown(X)));
	res.push_back((X != moveLeft(X)));
	res.push_back((X != moveRight(X)));
		
	return res;
}

bool isLosing(vector <int> &X){
	if (X != moveUp(X)) return false;
	if (X != moveDown(X)) return false;
	if (X != moveLeft(X)) return false;
	if (X != moveRight(X)) return false;
	return true;
}

int Rating(vector <int> X){
	int res = 0;
	for (int i = 0; i < 16; i ++ )
		if (X[i] > 0) res += (1 << X[i]) * (X[i] - 1);
	
	return res;
}
int MaxTile(vector <int> X){
	int res = 0;
	for (int i : X)
		res = max(res, i);
	
	return res;
}
int SeTile(vector <int> X){
	int res = 0;
	int avoid = MaxTile(X);
	
	for (int i : X)
		if (i < avoid)
			res = max(res, i);
	return res;
}
void Move_transfer(vector <int> X, vector <double> &mat){
	int col = 0;
	int row = 0;
	for (int i = 0; i < 16; i ++ ){
		//up
		int j = i - 4;
		mat.push_back((j >= 0 && X[j] == X[i] ? 1 : 0));
		//down
		j = i + 4;
		mat.push_back((j < 16 && X[j] == X[i] ? 1 : 0));
		col += (j < 16 && X[j] == X[i]);
		//left
		j = i - 1;
		mat.push_back((j >= 0 && j/4 == i/4 && X[j] == X[i] ? 1 : 0));
		
		//right
		j = i + 1;
		mat.push_back((j < 16 && j/4 == i/4 && X[j] == X[i] ? 1 : 0));
		row += ((j < 16 && j/4 == i/4 && X[j] == X[i] ? 1 : 0));
	}
	int h = MaxTile(X);
	
	for (int i = 0; i < 16; i ++ )
		mat.push_back((X[i] == h ? 1 : 0));
		
	h = SeTile(X);
	
	for (int i = 0; i < 16; i ++ )
		mat.push_back((h != 0 && X[i] == h ? 1 : 0));
	int empty = count(X.begin(), X.end(), 0);
	mat.push_back(double(empty) / 15.0);
	
	mat.push_back(double(col)/12.0);
	mat.push_back(double(row)/12.0);
}
vector <double> transfer(vector <int> X){
	vector <double> mat(X.size() * 16, 0.0);
	for (int i = 0; i < 16; i ++ ){
		mat[i*16 + X[i]] = 1;
	}
	int col = 0;
	int row = 0;
	for (int i = 0; i < 16; i ++ ){
		//up
		int j = i - 4;
		while (j >= 0 && X[j] == 0) j -= 4;
		mat.push_back((j >= 0 && X[j] == X[i] ? 1 : 0));
		//down
		j = i + 4;
		while (j < 16 && X[j] == 0) j += 4;
		mat.push_back((j < 16 && X[j] == X[i] ? 1 : 0));
		col += (j < 16 && X[j] == X[i]);
		//left
		j = i - 1;
		while (j >= 0 && j/4 == i/4 && X[j] == 0) j --;
		mat.push_back((j >= 0 && j/4 == i/4 && X[j] == X[i] ? 1 : 0));
		
		//right
		j = i + 1;
		while (j < 16 && j/4 == i/4 && X[j] == 0) j ++;
		mat.push_back((j < 16 && j/4 == i/4 && X[j] == X[i] ? 1 : 0));
		row += ((j < 16 && j/4 == i/4 && X[j] == X[i] ? 1 : 0));
	}
	int h = MaxTile(X);
	
	for (int i = 0; i < 16; i ++ )
		mat.push_back((X[i] == h ? 1 : 0));
		
	h = SeTile(X);
	
	for (int i = 0; i < 16; i ++ )
		mat.push_back((h != 0 && X[i] == h ? 1 : 0));
	
		
	int empty = count(X.begin(), X.end(), 0);
	mat.push_back(double(empty) / 15.0);
	
	mat.push_back(double(col)/12.0);
	mat.push_back(double(row)/12.0);
	
	Move_transfer(moveUp(X), mat);
	Move_transfer(moveDown(X), mat);
	Move_transfer(moveLeft(X), mat);
	Move_transfer(moveRight(X), mat);
	return mat;
}


