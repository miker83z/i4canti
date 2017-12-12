#ifndef AGENT_HPP
#define AGENT_HPP
<<<<<<< HEAD
#include "Matrix.h"

class Agent{
public:
	Agent(Matrix a);
=======

class Agent{
public:
	Agent();
>>>>>>> ff5c2712dba9798375967436e6dcf6a6d793d468
	~Agent();
	void move();

private:
	int position[2];
<<<<<<< HEAD
	double ideas[4];
	Matrix mat;
	void set_new_position(int i);
=======
>>>>>>> ff5c2712dba9798375967436e6dcf6a6d793d468
};

#endif // AGENT_HPP