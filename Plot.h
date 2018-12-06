#ifndef PLOT_HPP
#define PLOT_HPP

#include "Environment.h"
#include "Agent.h"

using namespace std;

class Plot {
	Environment* env;

public:
	Plot(Environment* e, string directory, int tiers_number, int* agents_tiers, double** agents_properties, double** agents_ideas);
	~Plot();
	void update_tick(int tick);
};

#endif // PLOT_HPP
