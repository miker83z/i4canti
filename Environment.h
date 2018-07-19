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
	int** mat_prec;
	vector<Agent*> agents;
	int** centers;
	int** canti;
public:
	Environment(int N, int NA, int n_canti, int radius, int tiers_number, int* agents_tiers, double** agents_properties, double** agents_ideas);
	~Environment();
	bool is_allowed_in_position(int x, int y);
	void init_interactions();
	void set_in_position(int x, int y, int o_x, int o_y);
	int get_dim();
	int get_num_canti();
	int** get_centers();
	int** get_canti();
	int get_num_agents();
	Agent* get_agent(int i);
	Agent* get_agent_in_position(int x, int y, int z);
	void print_mat();
	void print_agents_position();
	void print_agents_ideas();
	void update_centers();
private: 
	void set_canti();
	void set_leaders();
};

#endif // ENVIRONMENT_HPP
