#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>
#include "Environment.h"

using namespace std;

class Agent{
	string name;		// Agent name
	int id;				// Agent id, integer used to represent the agent in the field
	int position[2];	// Position in the field, x and y coordinates
	int previous_position[2];	// Position in the field at previous step, x and y coordinates

	double* ideas;			// Agent array of ideas
	double* ideas_pre_step;	// Array of ideas saved from the previous time step
	int actual_prominent_idea;	// Actual prominent idea
	double persuasion;		// Agent value of persuasion (0...1)
	double susceptibility;	// Agent value of susceptibility (0...1)

	int actual_chosen_idea;	// Idea chosen at each time step
	int view;				// Parameter that indicates the size of the window used to select agent to interact with
	int followers_pre_step;	// Followers at previous time step
	int actual_followers;	// Followers at this time step
	int supportive_neighbors_previous_step;	// Number of supportive neighbors at previous step
	bool leader;			// Is leader or not?

	Environment* environment;

public:
	Agent(Environment* e, int x, int y, int agt_counter, double pers, double susc, double* tier_ideas, int global_radius);
	~Agent();
	void tick();

	int get_actual_prominent_idea();
	int get_idea_to_play();
	void set_idea_to_play();
	void follow();

	void set_previous_position(int x, int y);
	void set_idea(int i, double val);
	void set_pre_idea(int i, double val);
	void set_actual_prominent_idea();
	void set_followers_pre_step(int fllw);
	void set_actual_followers(int fllw);
	void setLeader(bool nominee);

	string get_name();
	int get_id();
	int* get_position();
	int* get_previous_position();
	double* get_ideas();
	double get_persuasion();
	double get_susceptibility();
	int get_followers_pre_step();
	int get_actual_followers();
	bool isLeader();

private:
	int interact();
	void move(int x, int y);
	int uniform_decision_pick(double* arr, int size);
	int get_max_idea(double * arr);
};

#endif // AGENT_HPP