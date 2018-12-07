#ifndef PLOT_HPP
#define PLOT_HPP

#include "Environment.h"
#include "Agent.h"

using namespace std;

class Plot {
	Environment* env;
	int sims_end;
	int time_end;
	int num_ideas;
	double** ticks_values;
	int** ticks_agts;
	int* ticks_moves;

	int tiers_number;
	int* agents_tiers;
	double** agents_properties;
	double** agents_ideas;

public:
	Plot(string directory, int sims_en, int time_en, int num_i, int tiers_n, int* agents_ti, double** agents_prop, double** agents_id);
	~Plot();
	void update_tick(int tick);
	void close_plot();
	void set_env(Environment* e);
private:
	void set_prop();
};

#endif // PLOT_HPP
