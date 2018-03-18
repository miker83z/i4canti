#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>
#include "Environment.h"

using namespace std;

class Agent{
	int position[2];
	double* ideas;
	double* initial_ideas;
	double* direction;
	int current_influencing_idea;
	Environment* env;
	string name;
	int id;

	double interpersonal_influence;
	double susceptibility;

public:
	Agent(Environment* e, int x, int y, int s);
	~Agent();
	void move();
	void interact(); 
	void influence_game(Agent *other);
	int* get_position();
	double* get_ideas();
	double* get_direction();
	void get_influenced(Agent* influencer);
	int get_prominent_idea();
	int get_id();
	string get_name();

	void set_interpersonal_influence(double val);
	void set_susceptibility(double val);
	double get_interpersonal_influence();
	double get_susceptibility();
private:
	void set_new_position(int i);
	void setup_ideas();
	void setup_direction();
	void update_idea(int idea, double value);
};

#endif // AGENT_HPP