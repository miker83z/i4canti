#include "Environment.h"
#include "Agent.h"

#include <random>
#include <chrono>
#include <iostream>

mt19937 rng;	// For Random values
int *agents_shuffle;

Environment::Environment(int N, int NA, int n_ideas, int tiers_number, int* agents_tiers, double** agents_properties, double** agents_ideas, int global_radius) {
	rng = mt19937(chrono::high_resolution_clock::now().time_since_epoch().count());

	dimension_size = N;
	agents_number = NA;
	ideas_number = n_ideas;

	//field allocation 
	field = new int*[dimension_size]();
	field_precedent_step = new int*[dimension_size]();
	for (int i = 0; i < dimension_size; i++) {
		field[i] = new int[dimension_size]();
		field_precedent_step[i] = new int[dimension_size]();
		for (int j = 0; j < dimension_size; j++) {
			field_precedent_step[i][j] = -1;
			field[i][j] = -1;
		}
	}

	//centers allocation
	ideas_centers = new int*[ideas_number];
	for (int i = 0; i < ideas_number; i++)
		ideas_centers[i] = new int[2];
	ideas_agents_number = new int[ideas_number]();

	//agents creation
	int agent_counter = 0;
	for (int i = 0; i < tiers_number; i++)	
		// for each tier i 
		for (int agent = 0; agent < agents_tiers[i]; agent++) {
			// for each agent in tier i
			// find a position in the field
			int x = random_int(0, dimension_size), y = random_int(0, dimension_size);
			while (!is_allowed_in_position(x, y)) {
				x = random_int(0, dimension_size); y = random_int(0, dimension_size);
			}
			field[x][y] = agent_counter;

			// create agent
			Agent* ag = new Agent(this, x, y, agent_counter++, agents_properties[i][0], agents_properties[i][1], agents_ideas[i], global_radius);
			agents.push_back(ag);
		}

	agents_shuffle = new int[agents_number]();	// used to shuffle agents when picked 
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

	//centers
	for (int i = 0; i < ideas_number; i++) {
		delete ideas_centers[i];
	}
	delete ideas_centers;
	delete ideas_agents_number;

	//agents
	for (int i = 0; i < agents_number; i++)
		delete agents[i];
	agents.clear();
}

void Environment::update_centers() {
	// Initialize arrays for max function
	int *max_followers = new int[ideas_number]();
	int *most_followed_agents = new int[ideas_number]();
	for (int i = 0; i < ideas_number; i++) {
		max_followers[i] = -1;
		most_followed_agents[i] = -1;
		ideas_agents_number[i] = 0;
	}

	// Shuffle agents before iterating over
	//random_shuffle(&agents_shuffle[0], &agents_shuffle[agents_number]);

	// Search the most followed agent for each idea
	for (int i = 0; i < agents_number; i++) {
		int id = i; // = agents_shuffle[i];	// agent
		//std::cout << agents_shuffle[i] << "\n";
		agents[id]->setLeader(false);	//
		int idea = agents[id]->get_actual_prominent_idea();	// idea
		ideas_agents_number[i]++;	// count agents
		// Most followed agent
		if (agents[id]->get_followers_pre_step() > max_followers[idea]) {
			max_followers[idea] = id;
			max_followers[idea] = agents[id]->get_followers_pre_step();
		}
	}
	std::cout << "here1\n";

	// Set centers
	for (int i = 0; i < ideas_number; i++) {
		if (most_followed_agents[i] >= 0) {
			agents[most_followed_agents[i]]->setLeader(true);
			ideas_centers[i][0] = agents[most_followed_agents[i]]->get_position()[0];
			ideas_centers[i][1] = agents[most_followed_agents[i]]->get_position()[1];
			std::cout << "here2\n";
		}
		else {
			ideas_centers[i][0] = dimension_size / 2;
			ideas_centers[i][1] = dimension_size / 2;
		}
	}

	//delete[] max;
	//delete[] most_followed_agents;
}

void Environment::init_time_step() {
	std::cout << "here3\n";
	// setup field at previous step
	for (int i = 0; i < dimension_size; i++)
		for (int j = 0; j < dimension_size; j++)
			field_precedent_step[i][j] = field[i][j];

	//setup agents
	for (int i = 0; i < agents_number; i++) {
		// followers
		agents[i]->set_followers_pre_step(agents[i]->get_actual_followers());
		agents[i]->set_actual_followers(1);
		// position
		agents[i]->set_previous_position(agents[i]->get_position()[0], agents[i]->get_position()[1]);
		// ideas
		for (int idea = 0; idea < ideas_number; idea++)
			agents[i]->set_pre_idea(idea, agents[i]->get_ideas()[idea]);
		agents[i]->set_actual_prominent_idea();	// actual prominent idea (not to play)
		agents[i]->set_idea_to_play(); // chosen idea to play
	}

	update_centers();

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
	if (x >= 0 && x < dimension_size && y >= 0 && y < dimension_size && field[x][y] == -1) 
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

int** Environment::get_ideas_centers() {
	return ideas_centers;
}

int* Environment::get_ideas_agents_number() {
	return ideas_agents_number;
}

double Environment::random_double(int floor, int ceil) {
	uniform_int_distribution<> distribution(floor, ceil);
	return distribution(rng);
}

int Environment::random_int(int floor, int ceil) {
	return static_cast<int>(random_double(floor, ceil));
}