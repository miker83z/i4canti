#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>
#include "Environment.h"

using namespace std;

class Agent{

	int position[2];
	double* ideas;
	double* direction;
	Environment* env;
	string name;
	int id;

public:
	Agent(Environment* e, int x, int y, int s);
	~Agent();
	void move();
	void interact();
	int* get_position();
	double* get_ideas();
	int get_id();
private:
	void set_new_position(int i);
};

#endif // AGENT_HPP