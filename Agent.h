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

	int idea_to_play;		// Idea chosen at each time step
	int view;				// Parameter that indicates the size of the window used to select agent to interact with

	double threshold;
	bool IDEA_BASED;
	double has_moved;

	Environment* environment;

public:
	Agent(Environment* e, int x, int y, int agt_counter, double pers, double susc, double* tier_ideas, int global_radius, bool id_basd, double thr);
	~Agent();
	void tick();

	int get_idea_to_play();
	void set_idea_to_play();
	int get_actual_prominent_idea();
	void set_actual_prominent_idea();

	void set_previous_position(int x, int y);
	void set_idea(int i, double val);
	void set_pre_idea(int i, double val);
	void setup_has_moved();

	string get_name();
	int get_id();
	int* get_position();
	int* get_previous_position();
	double* get_ideas();
	double get_persuasion();
	double get_susceptibility();
	double hasMoved();

private:
	double interact();
	void move_randomly();
	int uniform_decision_pick(double* arr, int size);
	int get_max_idea(double * arr);
	double compute_distance(int* a, int* b);
};

#endif // AGENT_HPP