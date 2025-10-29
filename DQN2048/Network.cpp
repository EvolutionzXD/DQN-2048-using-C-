#include<bits/stdc++.h>
#include "Game.cpp"

using namespace std;


ll randInt(ll l, ll r){
	return l + 1ll * rand() * rand() * rand()  % (r - l + 1);
}
struct network{
	vector <vector <double>> layer[3];
	double eps;
	double gamma;
	int batch_size = 32;
	vector <vector <double>> w_upd[3];
	int layer_size[4];
	
	double learning_rate;
	
	struct transition{
		vector <int> Now;
		vector <int> Then;
		double reward;
		int choice;
		
		transition(vector <int> &_Now, int &_reward, int &_choice, vector <int> &_Then){
			Now = _Now;
			Then = _Then;
			reward = _reward >= 0 ? log2(max(1, _reward))/15: -log2(-_reward)/15;
			choice = _choice;
		} 
	};
	
	vector <transition> Memory;
	vector <transition> Death_memory;
	int Dea_cnt;
	int Mem_cnt;

	network(){
		Mem_cnt = 0;
		Dea_cnt = 0;
		srand(time(0));
		learning_rate = 0.0005;
		gamma = 0.9;
		eps = 1;
		layer_size[0] = 751;// input
		layer_size[1] = 512;
		layer_size[2] = 128;
		layer_size[3] = 4;// output
		for (int I = 0; I < 3; I ++ ){
			w_upd[I].assign(layer_size[I], vector <double> (layer_size[I + 1], 0.0));
			layer[I].assign(layer_size[I], vector <double> (layer_size[I + 1], 0.0));
			for (int i = 0; i < layer_size[I]; i ++ )
				for (int j = 0; j < layer_size[I + 1]; j ++ ){
					double scale = sqrt(2.0 / layer_size[I]);
					int limit = int(scale * 1e8);
					int r = randInt(-limit, limit); // có âm
					layer[I][i][j] = (double)r / 1e8;

				}
		}
	}

	void ReLu_activation(vector <double> &X){
		for (int i = 0; i < X.size(); i ++ )
			X[i] = max(X[i], 0.0);
	}
	
	double Relu_deActivation(double a){
		return a > 0 ? 1.0 : 0.0;
	}
	
	vector <double> predict(vector <int> X){
		vector <double> input = transfer(X); 
		vector <double> output;
	
		for (int I = 1; I <= 3; I ++ ){
			output.assign(layer_size[I], 0);
		
//			system("pause");
			for (int i = 0; i < layer_size[I - 1]; i ++ )
				for (int j = 0; j < layer_size[I]; j ++ ){
					output[j] += input[i]*layer[I - 1][i][j];
				}
	
//			cout << "layer " << I <<": " << *max_element(output.begin(), output.end()) <<" " << *min_element(output.begin(), output.end()) <<endl;
			if (I != 3) ReLu_activation(output);
//			for (double i : output)
//				cout << i <<" ";
//			cout <<endl;
//			cout.flush();
			input = output;
//			cout << "Layer " << I <<": " << output.size() <<" " << input.size() <<endl;	
			
		}
		
		return output;
	}
	
	void train(vector <int> x, vector <double> New_output){
		vector <double> input = transfer(x);
		vector<vector <double>> activations;
		vector<vector <double>> no_activations;
		activations.push_back(input);
		no_activations.push_back(input);
		for (int I = 1; I <= 3; I ++ ){
			vector <double> output(layer_size[I], 0);
			
			for (int i = 0; i < layer_size[I - 1]; i ++ )
				for (int j = 0; j < layer_size[I]; j ++ )
					output[j] += activations[I - 1][i] * layer[I - 1][i][j];
			
			no_activations.push_back(output);
			if (I != 3) ReLu_activation(output);
			activations.push_back(output);
		}
		
		vector <double> deltas[4];
		
		deltas[3].assign(layer_size[3], 0.0);
		for (int i = 0; i < 4; i ++ ){
			double a = activations[3][i];
			deltas[3][i] = (a - New_output[i]);
		}
		
		for (int I = 2; I >= 0; I -- ){
			deltas[I].assign(layer_size[I], 0.0);
			
			for (int i = 0; i < layer_size[I]; i ++ )
				for (int j = 0; j < layer_size[I + 1]; j ++ ){
					deltas[I][i] += deltas[I + 1][j] * layer[I][i][j];
				
				}
					
			if (I != 0)
				for (int i = 0; i < layer_size[I]; i++)
				deltas[I][i] *= Relu_deActivation(no_activations[I][i]);
		}
		
		for (int I = 1; I <= 3; I ++ ){
			for (int i = 0; i < layer_size[I - 1]; i ++ )
				for (int j = 0; j < layer_size[I]; j ++ ){
					w_upd[I - 1][i][j] -= learning_rate	* activations[I - 1][i] * deltas[I][j];
				}
		}
	}
	
	void store_trasition(vector <int> Now, int reward, int choice, vector <int> Then){
		if (Memory.size() < 5000)
			Memory.push_back(transition(Now, reward, choice, Then));
		else Memory[Mem_cnt % 5000] = transition(Now, reward, choice, Then);
		
		Mem_cnt ++;
	}
	
	void add_death(vector <int> Now, int reward, int choice, vector <int> Then){
		if (Death_memory.size() < 100)
			Death_memory.push_back(transition(Now, reward, choice, Then));
		else Death_memory[Dea_cnt % 100] = transition(Now, reward, choice, Then);
		
		Dea_cnt ++;
	}
	
	pair <int, vector<double>> Action(vector <int> x){
		vector <double> choices;
		vector <int> valid;	
		
		int choice = 0;
		choices = predict(x);
		vector <bool> ways = State_Check(x);
	
		for (int i = 0; i < 4; i ++ ){
			if (!ways[i]) choices[i] = -10;
			else valid.push_back(i);
			if (choices[choice] < choices[i]) choice = i;
		}
		
		if (valid.size() == 0) return {choice,choices};
		
		if ((double)rand() / RAND_MAX > eps){}
		else choice = valid[rand() % valid.size()];
		
		return {choice, choices};
	}
	
	void learn(){
		for (int I = 0; I < 3; I ++ )
			w_upd[I].assign(layer_size[I], vector <double> (layer_size[I + 1], 0.0));
		
		for (int I = 0; I < batch_size; I ++ ){
			int ID = 0;
			ID = rand() % min(5000, Mem_cnt);
			
			
			transition Batch = Memory[ID];
			
			vector <int> Now = Batch.Now;
			vector <int> Then = Batch.Then;
			double reward = Batch.reward;
			int action = Batch.choice;
			
			vector <double> q_Now = predict(Now);
			vector <double> q_Then = predict(Then);
			double max_then = -100000000;
			
			for (double i : q_Then)
				max_then = max(max_then, i);
				
	//		cout << "?Q = " << (reward + gamma * max_then - q_Now[action]) <<" " << reward <<" " << max_then << " " << q_Now[action]<< endl;
				
			q_Now[action] = reward + gamma * max_then ;
			
			train(Now, q_Now);
		}
		for (int I = 1; I <= 3; I ++ ){
			for (int i = 0; i < layer_size[I - 1]; i ++ )
				for (int j = 0; j < layer_size[I]; j ++ ){
					layer[I - 1][i][j] += w_upd[I - 1][i][j]/batch_size;
				}
		}
		eps = max(0.01, eps*0.99995);
		
	}
	
	void load(string s){
		ifstream inp(s);
		
		if (!inp) return;
		for (int I = 0; I < 3; I ++ ){
			for (int i = 0; i < layer_size[I]; i ++ )
				for (int j = 0; j < layer_size[I + 1]; j ++ )
					inp >> layer[I][i][j];
		}
		inp.close();
	}
	
	void save(string s){
		ofstream out(s);
		
		for (int I = 0; I < 3; I ++ )
			for (int i = 0; i < layer_size[I]; i ++ ){
				for (int j = 0; j < layer_size[I + 1]; j ++ ){
					out << layer[I][i][j] <<endl;
					}
			}
					
		out.close();
	}
	
	void OverRide(string s){
		ofstream out(s);
		
		for (int I = 0; I < 3; I ++ )
			for (int i = 0; i < layer_size[I]; i ++ )
				for (int j = 0; j < layer_size[I + 1]; j ++ )
					out << 0<<" ";
					
		out.close();
	}
};


