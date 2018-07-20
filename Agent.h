#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>
#include "Environment.h"

using namespace std;

class Agent{
	double persuasion;
	double susceptibility;
	double* ideas;
	double* ideas_pre_step;
	int actual_chosen_idea;
	int view;
	int followers_pre_step;
	int actual_followers;
	bool leader;

	int position[2];
	double* direction;

	Environment* env;
	string name;
	int id;

public:
	Agent(Environment* e, int x, int y, int s, double pers, double susc, int rd);
	~Agent();
	void tick();

	void follow();
	void set_followers_pre_step(int fllw);
	void set_idea(int i, double val);
	void set_pre_idea(int i, double val);
	void set_actual_followers(int fllw);
	void setLeader(bool nominee);
	int get_actual_followers();
	int get_followers_pre_step();
	int* get_position();
	double* get_ideas();
	double get_persuasion();
	int get_prominent_idea_pre_step();
	int get_actual_prominent_idea();
	int get_id();
	string get_name();
	bool isLeader();
	int get_idea_to_play();
	void set_idea_to_play();

private:
	void move(int* canto);
	void move();
	void interact();
	void setDirection();
	int get_max(double *arr);
};

#endif // AGENT_HPP