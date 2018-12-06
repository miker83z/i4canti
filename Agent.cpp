#include <cstdlib>
#include <map>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

int dimension_size;
int agents_number;
int ideas_number;
double min_value_for_ideas;
double max_value_for_ideas;

Agent::Agent(Environment* e, int x, int y, int agt_counter, double pers, double susc, double* tier_ideas, int global_radius, bool id_basd, double thr) {
	name = "Agent " + to_string(agt_counter);
	id = agt_counter;
	environment = e;
	previous_position[0] = x;
	position[0] = x;
	previous_position[1] = y;
	position[1] = y;

	persuasion = pers;
	susceptibility = susc;

	IDEA_BASED = id_basd;
	threshold = thr;

	// miscellaneous
	dimension_size = environment->get_dimension_size();
	agents_number = environment->get_agents_number();
	view = global_radius;
	ideas_number = environment->get_ideas_number();
	min_value_for_ideas = .03 / (double)(ideas_number - 1);
	max_value_for_ideas = .97;

	//ideas
	ideas = new double[ideas_number]();
	ideas_pre_step = new double[ideas_number]();
	if (tier_ideas[0] > 0)
		for (int i = 0; i < ideas_number; i++)
			ideas_pre_step[i] = ideas[i] = tier_ideas[i];
	else {
		for (int i = 0; i < ideas_number; i++)
			ideas[i] = environment->random_double(0, 10);

		double tmp_sum = 0;
		for (int i = 0; i < ideas_number; i++) {
			if (ideas[i] < min_value_for_ideas) ideas[i] = min_value_for_ideas;
			tmp_sum += ideas[i];
		}
		for (int i = 0; i < ideas_number; i++)
			ideas_pre_step[i] = ideas[i] /= tmp_sum;
	}
	set_actual_prominent_idea();
}

Agent::~Agent() {
	delete ideas;
	delete ideas_pre_step;
}

void Agent::tick() {
	//Store threshold for this tick
	if (IDEA_BASED) threshold = 1 - ideas[actual_prominent_idea];

	// Interact with neighbors in the view
	double percent_of_dissimilar = interact();

	//Move according to similars in the neighborhood
	if(percent_of_dissimilar > threshold)
		move_randomly();
}

double Agent::interact() {
	// Matrix where an entry ij consist in an agent j influencing with an idea i
	vector<vector<int>> neighbors(ideas_number);
	// Final summed up persuasion values for each ideas
	vector<double>persuasion_values_for_ideas(ideas_number);

	// Search neighbors in the view
	for (int i = (-view / 2); i <= (view / 2); i++)
		for (int j = (-view / 2); j <= (view / 2); j++) {
			if (i == 0 && j == 0) j++;
			int other_position[2] = { position[0] + i, position[1] + j };
			if (other_position[0] < 0) other_position[0] = (other_position[0] % dimension_size) + dimension_size;
			if (other_position[0] >= dimension_size) other_position[0] = other_position[0] % dimension_size;
			if (other_position[1] < 0) other_position[1] = (other_position[1] % dimension_size) + dimension_size;
			if (other_position[1] >= dimension_size) other_position[1] = other_position[1] % dimension_size;
			//Agent to interact with
			Agent* other = environment->get_agent_in_position_at_prevoius_step(other_position[0], other_position[1]);
			if (other != NULL)
				neighbors[other->get_idea_to_play()].push_back(other->get_id());
		}

	// Count neighbors
	int neighbors_size = 0;
	for (int i = 0; i < ideas_number; i++)
		neighbors_size += neighbors[i].size();

	if (neighbors_size <= 0)
		return 2.0;

	// Get influenced by neighbors
	for (int i = 0; i < ideas_number; i++) {
		persuasion_values_for_ideas[i] = 0;
		for (size_t j = 0; j < neighbors[i].size(); j++) {
			// Formula's parameters
			Agent *influencer = environment->get_agent(neighbors[i][j]);
			double influencer_persuasion = influencer->get_persuasion();
			int* influencer_position = influencer->get_position();
			// Main interaction formula
			double interaction_formula = influencer_persuasion / pow(compute_distance(position, influencer_position), 2);
			persuasion_values_for_ideas[i] += interaction_formula;
		}
	}

	//Compute total payoff for all ideas
	double ideas_values_sum = 0.0;
	for (int i = 0; i < ideas_number; i++)
		ideas_values_sum += persuasion_values_for_ideas[i];

	if (ideas_values_sum > 0.0) {
		double ideas_values_sum2 = 0.0;
		for (int i = 0; i < ideas_number; i++) {
			persuasion_values_for_ideas[i] /= ideas_values_sum;
			if (persuasion_values_for_ideas[i] < min_value_for_ideas)
				persuasion_values_for_ideas[i] = min_value_for_ideas;
			ideas_values_sum2 += persuasion_values_for_ideas[i];
		}

		//Compute the new information considering agent susceptibility
		double* tmp_ideas = new double[ideas_number]();
		for (int i = 0; i < ideas_number; i++)
			tmp_ideas[i] = (((persuasion_values_for_ideas[i] / ideas_values_sum2) - ideas[i]) * susceptibility) + ideas[i];

		double total_ideas_tmp = 0.0;
		for (int i = 0; i < ideas_number; i++) {
			if (tmp_ideas[i] < min_value_for_ideas)
				tmp_ideas[i] = min_value_for_ideas;
			total_ideas_tmp += tmp_ideas[i];
		}
		//Then update ideas
		for (int i = 0; i < ideas_number; i++)
			set_idea(i, tmp_ideas[i] / total_ideas_tmp);
		delete tmp_ideas;
	}
	
	int idea_before_influence = actual_prominent_idea;
	set_actual_prominent_idea(); //for next iteration

	return 1.0 - ((double)neighbors[idea_before_influence].size() / (double)neighbors_size);
}

void Agent::move_randomly() {
	int new_position[2] = { position[0], position[1] };
	bool cant_move = true;
	int tmp_count = 0;
	while (cant_move && tmp_count++ < 16) {
		int i = environment->random_int(-1, 1);
		int j = environment->random_int(-1, 1);
		if (i != 0 || j != 0) {
			int other_position[2] = { position[0] + i, position[1] + j };
			if (other_position[0] < 0) other_position[0] = (other_position[0] % dimension_size) + dimension_size;
			if (other_position[0] >= dimension_size) other_position[0] = other_position[0] % dimension_size;
			if (other_position[1] < 0) other_position[1] = (other_position[1] % dimension_size) + dimension_size;
			if (other_position[1] >= dimension_size) other_position[1] = other_position[1] % dimension_size;

			if (environment->is_allowed_in_position(other_position[0], other_position[1])) {
				new_position[0] = other_position[0];
				new_position[1] = other_position[1];
				cant_move = false;
			}
		}
	}

	if (new_position[0] != position[0] || new_position[1] != position[1]) {
		environment->set_in_position(new_position[0], new_position[1], position[0], position[1]);
		position[0] = new_position[0];
		position[1] = new_position[1];
		has_moved = 1.0;
	}
}

int Agent::uniform_decision_pick(double* arr, int size) {
	double decision = environment->random_double(0, 1);
	int i = 0;
	for (double floor = 0.0; i < size; i++) {
		if (decision >= floor && decision <= (floor + arr[i]))
			return i;
		floor += arr[i];
	}
	return static_cast<int>((decision - 0.01) * size);
}

int Agent::get_max_idea(double * arr) {
	int max = environment->random_int(0, ideas_number - 1);
	for (int i = 0; i < ideas_number; i++) {
		if (i != max)
			if (arr[i] > arr[max])
				max = i;
	}
	return max;
}

double Agent::compute_distance(int* a, int* b) {
	// Compute distance considering a field with limits
	// Euclidean distance 1
	double euc1 = sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));


	// or a continuos field without limits
	int tmp_min = a[0], tmp_max = b[0];
	if (a[0] > b[0]) {
		tmp_min = b[0];
		tmp_max = a[0];
	}
	int x = tmp_min + dimension_size - tmp_max;

	tmp_min = a[1], tmp_max = b[1];
	if (a[1] > b[1]) {
		tmp_min = b[1];
		tmp_max = a[1];
	}
	int y = tmp_min + dimension_size - tmp_max;
	// Euclidean distance 2
	double euc2 = sqrt(pow(x, 2) + pow(y, 2));

	return euc1 < euc2 ? euc1 : euc2;
}

int Agent::get_idea_to_play() {
	return idea_to_play;
}

void Agent::set_idea_to_play() {
	idea_to_play = uniform_decision_pick(ideas_pre_step, ideas_number);
}

int Agent::get_actual_prominent_idea() {
	return actual_prominent_idea;
}


void Agent::set_previous_position(int x, int y) {
	previous_position[0] = x;
	previous_position[1] = y;
}

void Agent::set_actual_prominent_idea() {
	actual_prominent_idea = get_max_idea(ideas);
}

void Agent::set_idea(int idea, double val) {
	ideas[idea] = val;
}

void Agent::set_pre_idea(int i, double val) {
	ideas_pre_step[i] = val;
}

void Agent::setup_has_moved() {
	has_moved = 0.0;
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

double Agent::hasMoved() {
	return has_moved;
}