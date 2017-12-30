#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include<vector>

using namespace std;

class Agent;

class Environment {
	int N;
	int NA;
	int num_canti;
	int** mat;
	vector<Agent*> agents;
	int** canti;
	int* pos_verified;
	int* idea_verified;
	int** interaction_verified;
public:
	Environment(int N, int NA, int n_canti);
	void set_in_position(int x, int y, int o_x, int o_y);
	int get_dim();
	int get_canti();
	int* get_canto_pos(int i);
	Agent* get_agent(int i);
	Agent* get_agent_in_position(int x, int y);
	bool is_allowed_in_position(int x, int y);
	bool check_interaction(Agent* first, Agent* second);
	void set_interaction(Agent* first, Agent* second);
	void init_interactions();
	void print_mat();
	void print_agents_position();
	void print_agents_ideas();
private: 
	void set_canti(int canto);
};

#endif // ENVIRONMENT_HPP
