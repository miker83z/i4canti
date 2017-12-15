#include <cstdlib>
#include <random>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

int num_canti;
vector<int *> canti;
double get_distance(int* a, int* b);

Agent::Agent(Environment* e, int x, int y, int s) {
	name = "Agent " + to_string(s);
	id = s;
	env = e;
	num_canti = e->get_canti();
	for (int i = 0; i < num_canti; i++) {
		canti.push_back(new int[2]);
		canti[i][0] = env->get_canto_pos(i)[0];
		canti[i][1] = env->get_canto_pos(i)[1];
	}

	position[0] = x;
	position[1] = y;
	env->init_position(id, position[0], position[1]);
	cout << name << " initial position: " << position[0] << "," << position[1] << "\n";

	ideas = new double[num_canti];
	for (int i = 0; i < num_canti; i++)
		ideas[i] = 1 / (double) num_canti;
	direction = new double[num_canti];
	for (int i = 0; i < num_canti; i++)
		direction[i] = ideas[i];
}

Agent::~Agent() {
}

void Agent::move() {
	
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(0.0, 1.0);
	double decision = dis(gen);
	cout << name << ", decision: " << decision << '\n';
	
	int i = 0;
	for ( double floor = 0.0; i < num_canti - 1; i++){
		if ( decision >= floor && decision <= (floor + direction[i]) ) break;
		floor += direction[i];
	}
	set_new_position(i);

	cout << name << " new position: " << position[0] << "," << position[1] << "\n";
}

void Agent::set_new_position( int canto ) {
	int new_position[2];
	new_position[0] = position[0];
	new_position[1] = position[1];
	
	double min = numeric_limits<double>::max();
	int tmp_positions[2];
	for (int i = -1; i < 2; i++){
		for (int j = -1; j < 2; j++){
			if ( i == 0 && j == 0 ) j++;
			tmp_positions[0] = position[0] + i;
			tmp_positions[1] = position[1] + j;
			double tmp = get_distance(tmp_positions, canti[canto]);
			if (tmp < min && env->is_allowed_in_position(tmp_positions[0], tmp_positions[1]) ) {
				min = tmp;
				new_position[0] = tmp_positions[0];
				new_position[1] = tmp_positions[1];
			}
		}
	}
	env->set_in_position(new_position[0], new_position[0], position[0], position[0]);
	position[0] = new_position[0];
	position[1] = new_position[1];
}

void Agent::interact() {
	int tmp_positions[2];
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0) j++;
			tmp_positions[0] = position[0] + i;
			tmp_positions[1] = position[1] + j;
			Agent* influencer = env->get_agent_in_position(tmp_positions[0], tmp_positions[1]);
			if (influencer != NULL) {

			}
		}
	}
}

int* Agent::get_position() {
	return position;
}

double* Agent::get_ideas() {
	return ideas;
}

int Agent::get_id() {
	return id;
}

double get_distance( int* a, int* b) {
	return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

