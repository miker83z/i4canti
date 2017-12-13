#ifndef AGENT_HPP
#define AGENT_HPP
#include "Matrix.h"

class Agent{
public:
	Agent(Matrix* a);
	~Agent();
	void move();
	int* get_position();
	double* get_ideas();

private:
	int position[2];
	double* ideas;
	Matrix* mat;
	void set_new_position(int i);
};

#endif // AGENT_HPP