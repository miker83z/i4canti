#include <cstdlib>
#include <map>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Agent.h"

int num_canti;
int n;
int na;
int initial_radius;
double min_sum;
double max_sum;
vector<int> suppotive_neighbors;
vector<double> neig_val;

double get_distance(int* a, int* b);

Agent::Agent(Environment* e, int x, int y, int s, double pers, double susc, int vw) {
	name = "Agent " + to_string(s);
	id = s;
	env = e;
	n = env->get_dim();
	na = env->get_num_agents();
	view = initial_radius = vw;

	persuasion = pers;
	susceptibility = susc;

	//canti
	num_canti = e->get_num_canti();
	min_sum = .03 / (num_canti - 1.0);
	max_sum = .97;

	//position
	position[0] = x;
	position[1] = y;

	//ideas
	ideas = new double[num_canti]();
	for (int i = 0; i < num_canti; i++)
		ideas[i] = 1.0 / num_canti;
	ideas_pre_step = new double[num_canti]();
	direction = new double[num_canti]();

	leader = false;
	followers_pre_step = 0;
	actual_followers = 0;
}

Agent::~Agent() {
	delete ideas;
	delete ideas_pre_step;
	if(direction != NULL)
		delete direction;
	suppotive_neighbors.clear();
	suppotive_neighbors.shrink_to_fit();
	neig_val.clear();
	neig_val.shrink_to_fit();
}

void Agent::tick() {
	int* canto; 

	//Save actual ideas before interaction
	for (int i = 0; i < num_canti; i++)
		ideas_pre_step[i] = ideas[i];

	//Update view considering followers
	double tmpView = initial_radius * (suppotive_neighbors.size() / (followers_pre_step + 1));
	view = tmpView > 1 ? tmpView : 1;

	//INTERACTION
	interact();

	if (!leader && neig_val.size() > 0) {
		//Follow a Leader
		int max = -1;
		for (int i = 0; i < neig_val.size(); i++)
			if(neig_val[i] > max)
				max = i;
		Agent* designed_agent = env->get_agent(suppotive_neighbors[max]);	//Agent picked
		canto = designed_agent->get_position();	//Agent picked position
		designed_agent->follow();
			
		//MOVEMENT
		move(canto);
	}
	else {
		//Agent is Leader or has no group
		setDirection();
		canto = env->get_centers()[env->uniform_decision_pick(direction, num_canti)];
		if (canto[0] && canto[1])
			move(canto);
	}

	suppotive_neighbors.clear();
	neig_val.clear();
}

void Agent::move(int* canto) {
	int new_position[2];
	new_position[0] = position[0];
	new_position[1] = position[1];
	
	double min = numeric_limits<double>::max();
	int tmp_positions[2];
	for (int i = -1; i < 2; i++){
		for (int j = -1; j < 2; j++){
			if ( i == 0 && j == 0 ) j++;
			////////////////
			int i2 = position[0] + i, j2 = position[1] + j;
			if (i2 < 0) i2 = n + i2;
			if (i2 >= n) i2 = i2 - n + 2;
			if (j2 < 0) j2 = n + j2;
			if (j2 >= n) j2 = j2 - n + 2;
			tmp_positions[0] = i2;
			tmp_positions[1] = j2;
			////////////////
			//if(tmp_positions[1] >= n || tmp_positions[0] >= n )
				//cout << tmp_positions[0] << " " << tmp_positions[1] << "......" << position[0] << " " << position[1] << "\n";
			if (env->is_allowed_in_position(tmp_positions[0], tmp_positions[1])) {
				double tmp = get_distance(tmp_positions, canto);
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
	//Matrix where rows contains ideas id and columns contains agents id, an entry ij consist in an agent j influencing with an idea i
	vector<vector<int>> agent_map;
	agent_map.resize(num_canti);
	//Matrix where rows contains ideas id and columns contains payoff values associated to agents described above
	vector<vector<double>> a_value;
	a_value.resize(num_canti);
	//Sum of all payoffs for a specific idea
	double* tmp_payoff_sum = new double[num_canti]();	 

	for (int i = -view; i <= view; i++) {		//Search in agent view
		for (int j = -view; j <= view; j++) {	//
			int tmp_positions[2];
			if (i == 0 && j == 0) j++;
			////////////////
			int i2 = position[0] + i, j2 = position[1] + j;
			if (i2 < 0) i2 = n + i2;
			if (i2 >= n) i2 = i2 - n + 2;
			if (j2 < 0) j2 = n + j2;
			if (j2 >= n) j2 = j2 - n + 2;
			tmp_positions[0] = i2;
			tmp_positions[1] = j2;
			////////////////
			//Boundaries control
			if (tmp_positions[0] >= 0 && tmp_positions[0] < n && tmp_positions[1] >= 0 && tmp_positions[1] < n) {
				//Agent to interact with
				Agent* other = env->get_agent_in_position(tmp_positions[0], tmp_positions[1], 1);
				if (other != NULL) {
					int otherStrat = other->get_idea_to_play();	//Other agent ideas played during this interaction
					double tmpValue = ( other->get_persuasion() /* ( (double)(1 + other->get_followers_pre_step()) / na )*/ ) / pow(get_distance(position, tmp_positions), 2);	//Payoff choosing opponent idea 
					tmp_payoff_sum[otherStrat] += tmpValue;	//Sum the payoff with the others that are choosing that idea 
					//cout << tmpValue << " \n";
					//Save interaction results in matrices
					agent_map[otherStrat].push_back(other->get_id());
					a_value[otherStrat].push_back(tmpValue);
				}
			}
		}
	}	

	//Compute total payoff for all ideas
	double total_ideas_tmp_payoff = 0.0;	
	for (int i = 0; i < num_canti; i++) {
		if (tmp_payoff_sum[i] < min_sum) 
			tmp_payoff_sum[i] = min_sum;
		total_ideas_tmp_payoff += tmp_payoff_sum[i];
	}
	//Payoffs are set to a the relative value (0-1)
	for (int i = 0; i < num_canti; i++) 
		tmp_payoff_sum[i] /= total_ideas_tmp_payoff;

	//Compute the new information considering agent susceptibility
	double* tmp_ideas = new double[num_canti]();
	for (int i = 0; i < num_canti; i++) 
		tmp_ideas[i] = ((tmp_payoff_sum[i] - ideas[i]) * susceptibility) + ideas[i];

	double total_ideas_tmp = 0.0;
	for (int i = 0; i < num_canti; i++) {
		if (tmp_ideas[i] < min_sum)
			tmp_ideas[i] = min_sum;
		total_ideas_tmp += tmp_ideas[i];
	}
	//Then update ideas
	for (int i = 0; i < num_canti; i++)
		ideas[i] = tmp_ideas[i] / total_ideas_tmp;
		//set_idea(i, tmp_ideas[i] / total_ideas_tmp);

	//Save most influencial agents informations and clear the remaining
	int tmpIdea = get_actual_prominent_idea();
	suppotive_neighbors.clear();
	suppotive_neighbors.shrink_to_fit();
	neig_val.clear();
	neig_val.shrink_to_fit();
	for (int i = 0; i < agent_map[tmpIdea].size(); i++) {
		suppotive_neighbors.push_back(agent_map[tmpIdea][i]);
		neig_val.push_back(a_value[tmpIdea][i]);
	}
	delete tmp_payoff_sum;
	delete tmp_ideas;
}

void Agent::setLeader(bool nominee) {
	leader = nominee;
}

void Agent::follow() {
	actual_followers++;
}

int Agent::get_followers_pre_step() {
	return followers_pre_step;
}

void Agent::set_followers_pre_step(int fllw) {
	followers_pre_step = fllw;
}

void Agent::setDirection() {
	if (direction != NULL)
		delete direction;
	direction = new double[num_canti]();
	for (int i = 0; i < num_canti; i++)
		if (i != get_actual_prominent_idea())
			direction[i] = 1.0 / ((double)num_canti - 1);
}

void Agent::set_idea(int idea, double val) {
	ideas[idea] = val;
	/*	double* ideas_tmp = new double[num_canti]();
	double total_ideas_tmp = 0.0;
	if (val < min_sum)	val = min_sum;
	if (val > max_sum)	val = max_sum;
	for (int i = 0; i < num_canti; i++) {
		if (i == idea) {
			ideas_tmp[i] = val;
			total_ideas_tmp += val;
		}
		else {
			ideas_tmp[i] = ideas[i];
			total_ideas_tmp += ideas[i];
		}
	}
	for (int i = 0; i < num_canti; i++)
		ideas[i] = ideas_tmp[i] / total_ideas_tmp;

	ideas[idea] = val;
	double newsum = 1.0 - val;
	double sum = 0.0;
	for (int i = 0; i < num_canti; i++)
		if (i != idea)
			sum += ideas[i];
	if (sum < min_sum * (num_canti - 1)) { sum = min_sum * (num_canti - 1); ideas[idea] = max_sum; }
	if (sum > max_sum) { sum = max_sum; ideas[idea] = min_sum; }
	for (int i = 0; i < num_canti; i++)
		if (i != idea)
			ideas[i] = (ideas[i] * newsum) / sum;*/
	//cout << ideas[0] << " " << ideas[1] << " " << ideas[2] << " " << ideas[3] << "\n";
}

void Agent::set_pre_idea(int i, double val) {
	ideas_pre_step[i] = val;
}

void Agent::set_actual_followers(int fllw) {
	actual_followers = fllw;
}

int Agent::get_actual_followers() {
	return actual_followers;
}

double Agent::get_persuasion() {
	return persuasion;
}

double Agent::get_susceptibility() {
	return susceptibility;
}

int* Agent::get_position() {
	return position;
}

double* Agent::get_ideas() {
	return ideas;
}

int Agent::get_prominent_idea_pre_step() {
	return get_max(ideas_pre_step);
}

void Agent::set_idea_to_play() {
	actual_chosen_idea = env->uniform_decision_pick(ideas_pre_step, num_canti);
}

int Agent::get_idea_to_play() {
	//return uniform_decision_pick(ideas_pre_step, num_canti);
	return actual_chosen_idea;
}

int Agent::get_actual_prominent_idea() {
	return get_max(ideas);
}

int Agent::get_max(double *arr) {
	vector<int> max;
	max.push_back(0);
	for (int i = 1; i < num_canti; i++)
		if (arr[i] > arr[max[0]]) {
			max.clear();
			max.push_back(i);
		}
		else if (arr[i] == arr[max[0]])
			max.push_back(i);

	int dp = 0;
	int tmp_s = max.size();
	if (tmp_s > 1) {
		double* tmp = new double[tmp_s];
		for (int i = 0; i < tmp_s; i++)
			tmp[i] = 1 / double(tmp_s);
		dp = env->uniform_decision_pick(tmp, tmp_s);
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

bool Agent::isLeader() {
	return leader;
}

double get_distance(int* a, int* b) {
	int tmp1 = a[0] - b[0];
	int tmp2 = (a[0] + b[0]) % ( n - 1 ) + 1;
	int x = tmp1 < tmp1 ? tmp1 : tmp2;
	tmp1 = a[1] - b[1];
	tmp2 = (a[1] + b[1]) % ( n - 1 ) + 1;
	int y = tmp1 < tmp1 ? tmp1 : tmp2;
	return sqrt(pow(x, 2) + pow(y, 2));
}