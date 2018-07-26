#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <vector>

using namespace std;

class Agent;

class Environment {
	int dimension_size;	// Size N of a field dimension, used to create the NxN matrix
	int agents_number;	// Number of agents in the field	
	int ideas_number;	// Number of different ideas an agent has
	int** field;		// Matrix where agents moves and interact
	int** field_precedent_step;	// Matrix used at time t to store field status at time t-1
	vector<Agent*> agents;	// Vector of Agents
	int** ideas_centers;	// Positions in the field of ideas centers, representing the main point to reach for each idea (could be an agent position)
	int *ideas_agents_number;	// Vector containing the number of agents having an idea at a time step

public:
	Environment(int N, int NA, int n_canti, int tiers_number, int* agents_tiers, double** agents_properties, double** agents_ideas, int global_radius);
	~Environment();
	void update_centers();
	void init_time_step();

	Agent* get_agent(int i);
	Agent* get_agent_in_position(int x, int y);
	Agent* get_agent_in_position_at_prevoius_step(int x, int y);
	bool is_allowed_in_position(int x, int y);
	void set_in_position(int x, int y, int old_x, int old_y);
	
	int get_dimension_size();
	int get_agents_number();
	int get_ideas_number();
	int** get_ideas_centers();
	int* get_ideas_agents_number();
	
	double random_double(int floor, int ceil);
	int random_int(int floor, int ceil);
};

#endif // ENVIRONMENT_HPP
