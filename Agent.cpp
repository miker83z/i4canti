#include "Agent.h"
#include <iostream>

int dimension_size;
int agents_number;
int ideas_number;
int initial_radius;
double min_value_for_ideas;
double max_value_for_ideas;

double compute_distance(int *posa, int *posb);

Agent::Agent(Environment* e, int x, int y, int agt_counter, double pers, double susc, double* tier_ideas, int global_radius) {
	name = "Agent " + to_string(agt_counter);
	id = agt_counter;
	environment = e;
	previous_position[0] = x;
	position[0] = x;
	previous_position[1] = y; 
	position[1] = y;

	persuasion = pers;
	susceptibility = susc;

	// miscellaneous
	dimension_size = environment->get_dimension_size();
	agents_number = environment->get_agents_number();
	view = initial_radius = global_radius;
	ideas_number = environment->get_ideas_number();
	min_value_for_ideas = .03 / (double)(ideas_number - 1);
	max_value_for_ideas = .97; 
	supportive_neighbors_previous_step = 0;

	//ideas
	ideas = new double[ideas_number]();
	ideas_pre_step = new double[ideas_number]();
	for (int i = 0; i < ideas_number; i++) {
		ideas_pre_step[i] = tier_ideas[i];
		ideas[i] = tier_ideas[i];
	}

	leader = false;
	followers_pre_step = actual_followers = 1 /*himself*/;
}

Agent::~Agent() {
	delete ideas;
	delete ideas_pre_step;
}

void Agent::tick() {
	// Save actual ideas before interaction
	for (int i = 0; i < ideas_number; i++)
		ideas_pre_step[i] = ideas[i];

	// Update view considering followers
	int tmp_number = environment->get_ideas_agents_number()[actual_prominent_idea];
	int tmp_follower_view = tmp_number > 0 ? tmp_number : agents_number;
	double tmpView = initial_radius * ( 1 - followers_pre_step / tmp_follower_view);
	view = tmpView > 1 ? (static_cast<int>(tmpView)) : 2;
	view = view > initial_radius ? initial_radius : view;

	// Interact with neighbors in the view
	int most_influential_agent_id = interact();
	// Move to your most influential agent
	if (most_influential_agent_id >= 0 && !leader) {
		Agent *most_influential_agent = environment->get_agent(most_influential_agent_id);
		most_influential_agent->follow();
		//move(most_influential_agent->get_previous_position()[0], most_influential_agent->get_previous_position()[1]);
	}
	// If you are alone or leader move to another leader
	else {
		int prominent_idea = get_actual_prominent_idea();
		int random_idea;
		do {
			random_idea = environment->random_int(0, ideas_number);
		} while (prominent_idea == random_idea);
		//move(environment->get_ideas_centers()[random_idea][0], environment->get_ideas_centers()[random_idea][1]);
	}
}

int Agent::interact() {
	vector<vector<int>> neighbors(ideas_number);
	vector<vector<double>> neighbors_persuasion_values(ideas_number);
	vector<double>persuasion_values_for_ideas(ideas_number);

	// Search neighbors in the view
	for (int i = (- view / 2) ; i <= (view / 2); i++) 
		for (int j = (- view / 2); j <= (view / 2); j++) 
			if( i != 0 && j != 0 ){
				int other_position[2] = { position[0] + i, position[1] + j };
				if (other_position[0] < 0) other_position[0] = (other_position[0] % dimension_size) + dimension_size;
				if (other_position[0] >= dimension_size) other_position[0] = other_position[0] % dimension_size;
				if (other_position[1] < 0) other_position[1] = (other_position[1] % dimension_size) + dimension_size;
				if (other_position[1] >= dimension_size) other_position[1] = other_position[1] % dimension_size;
				Agent *other = environment->get_agent_in_position(other_position[0], other_position[1]);
				if (other != NULL) 
					neighbors[other->get_idea_to_play()].push_back(other->get_id());
			}

	int neighbors_size = 0;
	for (int i = 0; i < ideas_number; i++)
		neighbors_size += neighbors[i].size();

	// Get influenced by neighbors
	for (int other_idea = 0; other_idea < ideas_number; other_idea++) {
		persuasion_values_for_ideas[other_idea] = 0;
		for (size_t i = 0; i < neighbors[other_idea].size(); i++) {
			// Formula's parameters
			Agent *other = environment->get_agent(neighbors[other_idea][i]);
			int other_followers = other->get_followers_pre_step();
			double other_persuasion = other->get_persuasion();
			int* other_position = other->get_position();
			// Main interaction formula
			double interaction_formula = 100000 * other_persuasion * (((double)other_followers) / ((double)neighbors_size)) / pow(compute_distance(position, other_position), 2);
			neighbors_persuasion_values[other_idea].push_back(interaction_formula);
			persuasion_values_for_ideas[other_idea] += interaction_formula;
		}
	}
	/*
	// Relative ideas values
	double ideas_values_sum = 0.0;
	for (int i = 0; i < ideas_number; i++)
		ideas_values_sum += persuasion_values_for_ideas[i];

	double ideas_values_sum2 = 0.0;
	for (int i = 0; i < ideas_number; i++) {
		persuasion_values_for_ideas[i] /= ideas_values_sum;
		if (persuasion_values_for_ideas[i] < min_value_for_ideas) persuasion_values_for_ideas[i] = min_value_for_ideas;
		ideas_values_sum2 += persuasion_values_for_ideas[i];
	}
	// Susceptibility
	for (int i = 0; i < ideas_number; i++) 
		set_idea(i, ((persuasion_values_for_ideas[i] / ideas_values_sum2) - ideas[i]) * susceptibility + ideas[i]);
	
	// Pick best supportive Agent
	int prominent_idea = get_actual_prominent_idea();
	supportive_neighbors_previous_step = neighbors[prominent_idea].size();
	int designed_agent = -1;
	double max = numeric_limits<double>::min();
	for (int idea = 0; idea < ideas_number; idea++) {
		int supportives_size = neighbors_persuasion_values[idea].size();
		if (supportives_size > 0) 
			for (size_t i = 0; i < neighbors_persuasion_values[idea].size(); i++)
				if (neighbors_persuasion_values[idea][i] > max) {
					max = neighbors_persuasion_values[idea][i];
					designed_agent = neighbors[idea][i];
				}
	}*/

	return 0;
}

void Agent::move(int x, int y) {
	int new_position[2] = {position[0], position[1]};
	int pos[2] = { x, y };
	double min = numeric_limits<double>::max();
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++) 
			if (i != 0 && j != 0) {
				int other_position[2] = { position[0] + i, position[1] + j };
				if (other_position[0] < 0) other_position[0] = (other_position[0] % dimension_size) + dimension_size;
				if (other_position[0] >= dimension_size) other_position[0] = other_position[0] % dimension_size;
				if (other_position[1] < 0) other_position[1] = (other_position[1] % dimension_size) + dimension_size;
				if (other_position[1] >= dimension_size) other_position[1] = other_position[1] % dimension_size;

				if (environment->is_allowed_in_position(other_position[0], other_position[1])) {
					double tmp = compute_distance(other_position, pos);
					if (tmp < min) {
						min = tmp;
						new_position[0] = other_position[0];
						new_position[1] = other_position[1];
					}
				}
			}
	
	if (new_position[0] != position[0] || new_position[1] != position[1]) {
		environment->set_in_position(new_position[0], new_position[1], position[0], position[1]);
		position[0] = new_position[0];
		position[1] = new_position[1];
	}
}

int Agent::get_actual_prominent_idea() {
	return actual_prominent_idea;
}

int Agent::get_idea_to_play() {
	//return uniform_decision_pick(ideas_pre_step, ideas_number);
	return actual_chosen_idea;
}

void Agent::set_idea_to_play() {
	actual_chosen_idea = uniform_decision_pick(ideas_pre_step, ideas_number);
}

void Agent::follow() {
	actual_followers++;
}

void Agent::set_previous_position(int x, int y) {
	previous_position[0] = x;
	previous_position[1] = y;
}

void Agent::set_idea(int idea, double val) {
	ideas[idea] = val;
}

void Agent::set_pre_idea(int i, double val) {
	ideas_pre_step[i] = val;
}

void Agent::set_actual_prominent_idea() {
	actual_prominent_idea = get_max_idea(ideas);
}

void Agent::set_followers_pre_step(int fllw) {
	followers_pre_step = fllw;
}

void Agent::set_actual_followers(int fllw) {
	actual_followers = fllw;
}

void Agent::setLeader(bool nominee) {
	leader = nominee;
}

string Agent::get_name() {
	return name;
}

int Agent::get_id() {
	return id;
}

int* Agent::get_position() {
	return position;
}

int* Agent::get_previous_position() {
	return previous_position;
}

double* Agent::get_ideas() {
	return ideas;
}

double Agent::get_persuasion() {
	return persuasion;
}

double Agent::get_susceptibility() {
	return susceptibility;
}

int Agent::get_followers_pre_step() {
	return followers_pre_step;
}

int Agent::get_actual_followers() {
	return actual_followers;
}

bool Agent::isLeader() {
	return leader;
}

int Agent::uniform_decision_pick(double* arr, int size) {
	double decision = environment->random_double(0, 1);
	int i = 0;
	for (double floor = 0.0; i < size; i++) {
		if (decision >= floor && decision <= (floor + arr[i]))
			return i;
		floor += arr[i];
	}
	return static_cast<int>((decision - 0.01 ) * size);
}

int Agent::get_max_idea(double * arr) {
	int max = environment->random_int(0, ideas_number);
	for (int i = 0; i < ideas_number; i++) {
		if (i != max)
			if (arr[i] > arr[max])
				max = i;
	}
	return max;
}

double compute_distance(int *posa, int *posb) {
	// Compute distance considering a square field for x
	int dist1 = abs(posa[0] - posb[0]);
	// Compute distance considering a continuos field for x
	int tmpMin, tmpOther;
	if (posa[0] < posb[0]) {
		tmpMin = posa[0];
		tmpOther = posb[0];
	}
	else {
		tmpMin = posb[0];
		tmpOther = posa[0];
	}
	int dist2 = tmpMin + dimension_size - tmpOther;
	int x = dist1 < dist2 ? dist1 : dist2;

	// Compute distance considering a square field for y
	dist1 = abs(posa[1] - posb[1]);
	// Compute distance considering a continuos field for y
	if (posa[1] < posb[1]) {
		tmpMin = posa[1];
		tmpOther = posb[1];
	}
	else {
		tmpMin = posb[1];
		tmpOther = posa[1];
	}
	dist2 = tmpMin + dimension_size - tmpOther;
	int y = dist1 < dist2 ? dist1 : dist2;

	// Euclidean distance
	return sqrt(pow(x, 2) + pow(y, 2));
}