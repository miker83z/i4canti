#include <cstdlib>
#include <random>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

const int IDEAS_MODE = 2;		//0 = uniform distr, 1 = (ideas for agent position) 01230123... , 2 = 00..011..122..233..3 
const int DIRECTION_MODE = 0;	//0 = direction equal to ideas

int num_canti;
int n;
vector<int *> canti;

double get_distance(int* a, int* b);
int uniform_decision_pick(double* arr, int size, int tru);

void Agent::setup_ideas() {
	ideas = new double[num_canti]();
	initial_ideas = new double[num_canti]();
	switch (IDEAS_MODE) {
	case 0:
		for (int i = 0; i < num_canti; i++)
			ideas[i] = 1 / (double)num_canti;
	case 1:
		if (num_canti == 1) {
			ideas[0] = 1;
			return;
		}
		if (num_canti == 2) {
			ideas[0] = .99;
			ideas[1] = .01;
		}
		ideas[id % num_canti] = .5;
		for (int i = 0; i < num_canti; i++)
			if (i != id % num_canti)
				ideas[i] = .5 / (double)(num_canti - 1);
	case 2:
		if (num_canti == 1) {
			ideas[0] = 1;
			return;
		}
		double tmp = (double)env->get_num_agents() / (double)num_canti;
		double floor = tmp;
		int chosen_idea = 0;
		for (int i = 1; i < num_canti; i++) {
			if (id >= floor && id <= (floor + tmp))
				chosen_idea = i;
			floor += tmp;
		}
		ideas[chosen_idea] = .9;
		for (int i = 0; i < num_canti; i++)
			if (i != chosen_idea)
				ideas[i] = .1 / (double)(num_canti - 1);

	}
	for (int i = 0; i < num_canti; i++)
		initial_ideas[i] = ideas[i];
}

void Agent::setup_direction() {
	direction = new double[num_canti];
	switch (DIRECTION_MODE) {
	case 0:
		for (int i = 0; i < num_canti; i++)
			direction[i] = ideas[i];
	}
}

Agent::Agent(Environment* e, int x, int y, int s) {
	name = "Agent " + to_string(s);
	id = s;
	env = e;
	n = env->get_dim();

	//canti
	num_canti = e->get_num_canti();
	for (int i = 0; i < num_canti; i++) 
		canti.push_back(env->get_canto_pos(i));

	//position
	position[0] = x;
	position[1] = y;

	//ideas
	setup_ideas();
	setup_direction();
}

Agent::~Agent() {
	canti.clear();
	delete ideas;
	delete initial_ideas;
	delete direction;
}

void Agent::move() {
	set_new_position( uniform_decision_pick(direction, num_canti, 0) );
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
			if (tmp_positions[0] >= 0 && tmp_positions[0] < n && tmp_positions[1] >= 0 && tmp_positions[1] < n && env->is_allowed_in_position(tmp_positions[0], tmp_positions[1])) {
				double tmp = get_distance(tmp_positions, canti[canto]);
				if (tmp < min) {
					min = tmp;
					new_position[0] = tmp_positions[0];
					new_position[1] = tmp_positions[1];
				}
			}
		}
	}
	if (new_position[0] != position[0] || new_position[1] != position[1]) {
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

	float diff = env->get_interpersonal_influence(id, other->get_id()) - env->get_interpersonal_influence(other->get_id(), id);
	if( diff > 0 ) {
		influencer = other;
		influenced = this;
	}
	else if (diff == 0) { //Flip a coin
		double coin[] = { .5, .5 };
		if (uniform_decision_pick(coin, 2, 0)) {
			influencer = other;
			influenced = this;
		}
	}
	
	influenced->get_influenced(influencer);
	env->set_interaction(influencer, influenced);
	//cout << influencer->get_name() << " influence " << influenced->get_name() << ": " << influenced->get_ideas()[0] << " + " << influenced->get_ideas()[1] << " = " << influenced->get_ideas()[0] + influenced->get_ideas()[1] << "\n";
}

void Agent::get_influenced(Agent* influencer) {
	int influenced_idea = influencer->get_prominent_idea();
	double value = ((double)env->get_susceptibility(id)) * ((double)env->get_interpersonal_influence(id, influencer->get_id())) * ideas[influenced_idea] + (1 - (double)env->get_susceptibility(id)) * initial_ideas[influenced_idea];

	double *tmp = new double[num_canti];
	double min_sum = .1 * (num_canti - 1);
	double max_value = 1 - min_sum;
	int flag = -1;

	if (value > max_value)
		value = max_value;

	double old_diff = 1 - ideas[influenced_idea];
	double diff = 1 - value;

	if (diff != old_diff) {
		tmp[influenced_idea] = value;
		for (int i = 0; i < num_canti; i++)
			if (i != influenced_idea) {
				tmp[i] = (ideas[i] * diff) / old_diff;
				if (tmp[i] > max_value) flag = i;
			}

		if (flag != -1) {
			tmp[flag] = max_value;
			double sum = 0;
			for (int i = 0; i < num_canti; i++)
				if (i != flag)
					sum += tmp[i];

			for (int i = 0; i < num_canti; i++)
				if (i != flag)
					tmp[i] = (tmp[i] * min_sum) / sum;
		}

		for (int i = 0; i < num_canti; i++)
			update_idea(i, tmp[i]);
	}

	delete tmp;
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

void Agent::update_idea(int idea, double value) {
	switch (DIRECTION_MODE) {
	case 0:
		ideas[idea] = value;
		direction[idea] = value;
	default:
		break;
	}
}

double get_distance(int* a, int* b) {
	return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

int uniform_decision_pick(double* arr, int size, int tru) {
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(0.0, 1.0);
	double decision = dis(gen);
	int i = 0;
	for (double floor = 0.0; i < size - 1; i++) {
		if (decision >= floor && decision <= (floor + arr[i])) {
			return i;
		}
		floor += arr[i];
	}
}