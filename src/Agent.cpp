#include <cstdlib>
#include <random>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

using namespace std;
int num_canti;
double get_distance(int* a, int* b);

Agent::Agent(Matrix* a) {
	mat = a;
	num_canti = a->get_canti();

	srand(time(0));
	position[0] = rand()%100;
	position[1] = rand()%100;
	cout << "Position " << position[0] << "," << position[1] << "\n";

	ideas = new double[num_canti];
	for (int i = 0; i < num_canti; i++){
		ideas[i] = 1 / (double) num_canti;
	}
}

Agent::~Agent() {
}

void Agent::move() {
	
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(0.0, 1.0);
	double decision = dis(gen);
	cout << decision << '\n';

	int i;
	double floor = 0.0;
	for (i = 0; i < num_canti - 1; i++){
		if ( decision >= floor && decision <= (floor + ideas[i]) ){
			cout << i;
			break;
		}
		floor += ideas[i];
	}
	set_new_position(i);
	cout << "Position " << position[0] << "," << position[1] << "\n";
}

void Agent::set_new_position( int canto ) {
	cout << " Canto" << canto << "\n";
	int* canto_pos = mat->get_canto_pos(canto);
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
			double tmp = get_distance(tmp_positions, canto_pos);
			if (tmp < min && mat->is_not_occupied(tmp_positions[0], tmp_positions[1]) ) {
				min = tmp;
				new_position[0] = tmp_positions[0];
				new_position[1] = tmp_positions[1];
			}
		}
	}
	mat->set_in_position(new_position[0], new_position[0], position[0], position[0]);
	position[0] = new_position[0];
	position[1] = new_position[1];
}

double get_distance( int* a, int* b) {
	return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

