#include <iostream>
#include <random>
#include <chrono>
#include <time.h>
#include "Environment.h"
#include "Agent.h"

mt19937 rng;

Environment::Environment(int n, int na, int nc, int radius, int tiers_number, int* agents_tiers, double** agents_properties, double** agents_ideas, bool id_basd, double thr) {
	rng = mt19937(chrono::high_resolution_clock::now().time_since_epoch().count());
	
	this->dimension_size = n;
	this->agents_number = na;
	this->ideas_number = nc;

	//field allocation
	field = new int*[dimension_size]();
	field_precedent_step = new int*[dimension_size]();
	for (int i = 0; i < dimension_size; i++) {
		field[i] = new int[dimension_size]();
		field_precedent_step[i] = new int[dimension_size]();
		for (int j = 0; j < dimension_size; j++)
			field_precedent_step[i][j] = field[i][j] = -1;
	}

	//agents creation
	int agent_counter = 0;
	for (int i = 0; i < tiers_number; i++)
		// for each tier i 
		for (int agent = 0; agent < agents_tiers[i]; agent++) {
			// for each agent in tier i find a position in the field
			int x = random_int(0, dimension_size - 1), y = random_int(0, dimension_size - 1);
			while (!is_allowed_in_position(x, y)) {
				x = random_int(0, dimension_size); 
				y = random_int(0, dimension_size);
			}
			field[x][y] = agent_counter;

			double pers = agents_properties[i][0] < 0 ? (double)random_double(0,1) : agents_properties[i][0];
			double susc = agents_properties[i][1] < 0 ? (double)random_double(0,1) : agents_properties[i][1];

			// create agent
			Agent* ag = new Agent(this, x, y, agent_counter++, pers, susc, agents_ideas[i], radius, id_basd, thr);
			agents.push_back(ag);
		}

	radius_global = radius;
	ideas_based = id_basd;
	threshold = thr;

	agents_shuffle = new int[agents_number]();
	for (int i = 0; i < agents_number; i++)
		agents_shuffle[i] = i;
}

Environment::~Environment() {
	//matrix
	for (int i = 0; i < dimension_size; i++) {
		delete field[i];
		delete field_precedent_step[i];
	}
	delete field;
	delete field_precedent_step;

	//agents
	for (int i = 0; i < agents_number; i++)
		delete agents[i];
	agents.clear();
	delete agents_shuffle;
}

void Environment::init_interactions() {
	// setup field at previous step
	for (int i = 0; i < dimension_size; i++)
		for (int j = 0; j < dimension_size; j++)
			field_precedent_step[i][j] = field[i][j];

	//setup agents
	for (int i = 0; i < agents_number; i++) {
		// position
		agents[i]->set_previous_position(agents[i]->get_position()[0], agents[i]->get_position()[1]);
		// ideas
		for (int idea = 0; idea < ideas_number; idea++)
			agents[i]->set_pre_idea(idea, agents[i]->get_ideas()[idea]); 
		agents[i]->set_idea_to_play(); // choose idea to play
		agents[i]->setup_has_moved();
	}
}

Agent* Environment::get_agent(int i) {
	return agents[i];
}

Agent* Environment::get_agent_in_position(int x, int y) {
	if (field[x][y] >= 0)
		return get_agent(field[x][y]);
	return NULL;
}

Agent* Environment::get_agent_in_position_at_prevoius_step(int x, int y) {
	if (field_precedent_step[x][y] >= 0)
		return get_agent(field_precedent_step[x][y]);
	return NULL;
}

bool Environment::is_allowed_in_position(int x, int y) {
	if ((x >= 0 && x < dimension_size && y >= 0 && y < dimension_size) && (field[x][y] == -1)) 
		return true;
	return false;
}

void Environment::set_in_position(int x, int y, int old_x, int old_y) {
	int tmp = field[old_x][old_y];
	field[old_x][old_y] = -1;
	field[x][y] = tmp;
}

int Environment::get_dimension_size() {
	return dimension_size;
}

int Environment::get_agents_number() {
	return agents_number;
}

int Environment::get_ideas_number() {
	return ideas_number;
}

double Environment::get_radius() {
	return radius_global;
}

bool Environment::is_ideas_based() {
	return ideas_based;
}

double Environment::get_threshold() {
	return threshold;
}

int* Environment::get_agents_shuffle() {
	return agents_shuffle;
}

double Environment::random_double(int floor, int ceil) {
	uniform_real_distribution<> distribution(floor, ceil);
	return distribution(rng);
}

int Environment::random_int(int floor, int ceil) {
	uniform_int_distribution<> distribution(floor, ceil);
	return distribution(rng);
}