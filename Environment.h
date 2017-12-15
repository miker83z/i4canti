#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include<vector>

using namespace std;

class Agent;

class Environment {
	int N;
	int num_canti;
	int** mat;
	vector<Agent*>* agents;
	int** canti;
	bool* pos_verified;
	bool* idea_verified;
	bool** interaction_verified;
public:
	Environment(int N, int NA, int n_canti, vector<Agent*>* agts);
	void init_position(int id, int x, int y);
	void set_in_position(int x, int y, int o_x, int o_y);
	int get_dim();
	int get_canti();
	int* get_canto_pos(int i);
	Agent* get_agent(int i);
	Agent* get_agent_in_position(int x, int y);
	bool is_allowed_in_position(int x, int y);
private: 
	void set_canti(int canto);
};

#endif // ENVIRONMENT_HPP
