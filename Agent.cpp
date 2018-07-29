#include <cstdlib>
#include <map>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

int dimension_size;
int agents_number;
int ideas_number;
int initial_radius;
double min_value_for_ideas;
double max_value_for_ideas;

double compute_distance(int* a, int* b);

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

	//cout << id << ", " << ideas[0] << ", " << ideas[1] << ", " << persuasion << ", " << susceptibility << "\n";

	leader = false;
	followers_pre_step = actual_followers = 1;
}

Agent::~Agent() {
	delete ideas;
	delete ideas_pre_step;
}

void Agent::tick() {

	//Save actual ideas before interaction
	for (int i = 0; i < ideas_number; i++)
		ideas_pre_step[i] = ideas[i];

	//Update view considering followers
	int tmp_number = environment->get_ideas_agents_number()[actual_prominent_idea];
	int tmp_follower_view = tmp_number > 0 ? tmp_number : agents_number;
	double tmpView = initial_radius * (1 - followers_pre_step / tmp_follower_view);
	view = tmpView > 1 ? (static_cast<int>(tmpView)) : 2;
	view = view > initial_radius ? initial_radius : view;

	// Interact with neighbors in the view
	int designed_agent = interact();

	// Move 
	if (!leader && designed_agent >= 0) {
		// Move to your most influential agent
		Agent* agent_to_follow = environment->get_agent(designed_agent);
		agent_to_follow->follow();
		move(agent_to_follow->get_previous_position());
	}
	else {
		// If you are alone or leader move to another leader
		//if (environment->random_int(0, 1)) {
			int random_idea;
			do {
				random_idea = environment->random_int(0, ideas_number - 1);
			} while (actual_prominent_idea == random_idea);
			move(environment->get_ideas_centers()[random_idea]);
		/*}
		else {
			int* canto;
			canto[0] = environment->random_int(0, dimension_size - 1);
			canto[1] = environment->random_int(0, dimension_size - 1);
			move(canto);
		}
		*/
	}
}

void Agent::move(int* post) {
	int new_position[2] = { position[0], position[1] };
	int pos[2] = { post[0], post[1] };
	double min = numeric_limits<double>::max();
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) j++;
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

int Agent::interact() {
	// Matrix where an entry ij consist in an agent j influencing with an idea i
	vector<vector<int>> neighbors(ideas_number);
	// Matrix where each entry consists in the persuasion value of the agent in the matrix above
	vector<vector<double>> neighbors_persuasion_values(ideas_number);
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

	// Get influenced by neighbors
	for (int i = 0; i < ideas_number; i++) {
		persuasion_values_for_ideas[i] = 0;
		for (size_t j = 0; j < neighbors[i].size(); j++) {
			// Formula's parameters
			Agent *influencer = environment->get_agent(neighbors[i][j]);
			int influencer_followers = influencer->get_followers_pre_step();
			double influencer_persuasion = influencer->get_persuasion();
			int* influencer_position = influencer->get_position();
			// Main interaction formula
			double interaction_formula = 100000 * influencer_persuasion * (((double)influencer_followers) / ((double)neighbors_size)) / pow(compute_distance(position, influencer_position), 2);
			neighbors_persuasion_values[i].push_back(interaction_formula);
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

	set_actual_prominent_idea();
	supportive_neighbors_previous_step = neighbors[actual_prominent_idea].size();

	// Pick best supportive Agent
	int designed_agent = -1;
	double max = numeric_limits<double>::min();
	for (int i = 0; i < ideas_number; i++)
		for (size_t j = 0; j < neighbors_persuasion_values[i].size(); j++)
				if (neighbors_persuasion_values[i][j] > max) {
					max = neighbors_persuasion_values[i][j];
					designed_agent = neighbors[i][j];
				}

	return designed_agent;
}

int Agent::get_actual_prominent_idea() {
	// return get_max_idea(ideas); 	
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

double compute_distance(int* a, int* b) {
	// Compute distance considering a square field for x
	int tmp1 = abs(a[0] - b[0]);
	// or a continuos field
	int tmp_min = a[0], tmp_max = b[0];
	if (a[0] > b[0]) {
		tmp_min = b[0];
		tmp_max = a[0];
	}
	int tmp2 = tmp_min + dimension_size - tmp_max;
	int x = tmp1 < tmp1 ? tmp1 : tmp2;

	// Compute distance considering a square field for y
	tmp1 = abs(a[1] - b[1]);
	// or a continuos field
	tmp_min = a[1], tmp_max = b[1];
	if (a[1] > b[1]) {
		tmp_min = b[1];
		tmp_max = a[1];
	}
	tmp2 = tmp_min + dimension_size - tmp_max;
	int y = tmp1 < tmp1 ? tmp1 : tmp2;

	// Euclidean distance
	return sqrt(pow(x, 2) + pow(y, 2));
}