#include <cstdlib>
#include <random>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

const int IDEAS_MODE = 0;		//0 = uniform distr, 1 =
const int DIRECTION_MODE = 0;	//0 = direct equal to ideas, 1 =

int num_canti;
int n;
vector<int *> canti;
double get_distance(int* a, int* b);
int uniform_decision_pick(double* arr, int size, int tru);

Agent::Agent(Environment* e, int x, int y, int s) {
	name = "Agent " + to_string(s);
	id = s;
	env = e;
	n = env->get_dim();

	//canti
	num_canti = e->get_canti();
	for (int i = 0; i < num_canti; i++) {
		canti.push_back(new int[2]);
		canti[i][0] = env->get_canto_pos(i)[0];
		canti[i][1] = env->get_canto_pos(i)[1];
	}

	//position
	position[0] = x;
	position[1] = y;

	//ideas
	setup_ideas(num_canti);
	setup_direction(num_canti, ideas);
}

Agent::~Agent() {
}

void Agent::move() {
	set_new_position( uniform_decision_pick(direction, num_canti, 0) );
	//cout << name << " new position: " << position[0] << "," << position[1] << "\n";
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
			if (tmp_positions[0] >= 0 && tmp_positions[0] < n && tmp_positions[1] >= 0 && tmp_positions[1] < n) {
				double tmp = get_distance(tmp_positions, canti[canto]);
				if (tmp < min && env->is_allowed_in_position(tmp_positions[0], tmp_positions[1])) {
					min = tmp;
					new_position[0] = tmp_positions[0];
					new_position[1] = tmp_positions[1];
				}
			}
		}
	}
	if(new_position[0] != position[0] && new_position[1] != position[1]){
		env->set_in_position(new_position[0], new_position[1], position[0], position[1]);
		position[0] = new_position[0];
		position[1] = new_position[1];
	}
}

void Agent::interact() {
	int tmp_positions[2];
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0) j++;
			tmp_positions[0] = position[0] + i;
			tmp_positions[1] = position[1] + j;
			if (tmp_positions[0] >= 0 && tmp_positions[0] < n && tmp_positions[1] >= 0 && tmp_positions[1] < n) {
				Agent* other = env->get_agent_in_position(tmp_positions[0], tmp_positions[1]);
				if (other != NULL && env->check_interaction(this, other))
					influence_game(other);
			}
		}
	}
}

void Agent::influence_game(Agent *other) {
	Agent *influencer = this, *influenced = other;
	double coin[] = { .5, .5 };

	if (uniform_decision_pick(coin, 2, 0)) {
		influencer = other;
		influenced = this;
	}

	int influencer_idea = influencer->get_prominent_idea();
	influenced->get_influenced(influencer_idea, .005);
	env->set_interaction(influencer, influenced);
	//cout << influencer->get_name() << " influence " << influenced->get_name() << ": " << influenced->get_ideas()[0] << " + " << influenced->get_ideas()[1] << " = " << influenced->get_ideas()[0] + influenced->get_ideas()[1] << "\n";
}

void Agent::get_influenced(int influenced_idea, double weight) {
	double value = ideas[influenced_idea] * weight; //weight from 0 to 1
	if (ideas[influenced_idea] + value > 1) {
		update_idea(influenced_idea, 1);
		for (int i = 0; i < num_canti; i++)
			if (i != influenced_idea)
				update_idea(i,0);
	}
	else {
		double old_diff = 1 - ideas[influenced_idea];
		update_idea(influenced_idea, ideas[influenced_idea] + value);
		double diff = 1 - ideas[influenced_idea];

		for (int i = 0; i < num_canti; i++)
			if (i != influenced_idea)
				update_idea(i, (ideas[i] * diff) / old_diff);
	}
}

int* Agent::get_position() {
	return position;
}

double* Agent::get_ideas() {
	return ideas;
}

int Agent::get_prominent_idea() {
	vector<int> max;
	max.push_back(0);
	for (int i = 1; i < num_canti; i++)
		if (ideas[i] > ideas[max[0]]){
			max.clear();
			max.push_back(i);
		}
		else if (ideas[i] == ideas[max[0]])
			max.push_back(i);
	
	int dp = 0;
	int tmp_s = max.size();
	if (tmp_s > 1) {
		double* tmp = new double[tmp_s];
		for (int i = 0; i < tmp_s; i++)
			tmp[i] = 1 / double(tmp_s);
		dp = uniform_decision_pick(tmp, tmp_s, 1);
		delete[] tmp;
	}
	return max[dp];
}

int Agent::get_id() {
	return id;
}

string Agent::get_name() {
	return name;
}

void Agent::setup_ideas( int n ) {
	ideas = new double[n];
	switch (IDEAS_MODE) {
	case 0:
		for (int i = 0; i < n; i++)
			ideas[i] = 1 / (double)num_canti;
	default:
		break;
	}
}

void Agent::setup_direction( int n, double *ideas ) {
	direction = new double[n];
	switch (DIRECTION_MODE) {
	case 0:
		for (int i = 0; i < n; i++)
			direction[i] = ideas[i];
	default:
		break;
	}
}

void Agent::update_idea(int idea, double value) {
	switch (DIRECTION_MODE) {
	case 0:
		ideas[idea] = value;
		direction[idea] = value;
	default:
		break;
	}
}

double get_distance( int* a, int* b) {
	return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

int uniform_decision_pick(double* arr, int size, int tru) {
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(0.0, 1.0);
	double decision = dis(gen);
	int i = 0;
	for (double floor = 0.0; i < size - 1; i++) {
		if (decision >= floor && decision <= (floor + arr[i])){
			return i;
		}
		floor += arr[i];
	}
}
