#include <iostream>
#include <random>
#include <time.h>
#include "Environment.h"
#include "Agent.h"


const int IDEAS_MODE = 1;		//0 = uniform distr, 1 = (ideas for agent position) 01230123...
mt19937 *gen;

Environment::Environment(int n, int na, int nc, int radius, int tiers_number, int* agents_tiers, double** agents_properties, double** agents_ideas) {
	this->N = n;
	this->NA = na;
	this->num_canti = nc;
	
	//matrix
	mat = new int*[n]();
	for (int i = 0; i < n; i++) {
		mat[i] = new int[n]();
		for (int j = 0; j < n; j++)
			mat[i][j] = -1;
	}

	//centers & canti
	centers = new int*[nc];
	for (int i = 0; i < nc; i++)
		centers[i] = new int[2];
	canti = new int*[nc];
	for (int i = 0; i < nc; i++)
		canti[i] = new int[2];
	set_canti();

	//agents
	srand(time(NULL));
	int agent_counter = 0;
	for (int i = 0; i < tiers_number; i++) {
		for (int agent = 0; agent < agents_tiers[i]; agent++) {
			int x = rand() % n, y = rand() % n;
			while (!is_allowed_in_position(x, y)) {
				x = rand() % n;
				y = rand() % n;
			}
			Agent* ag = new Agent(this, x, y, agent_counter, agents_properties[i][0], agents_properties[i][1], radius);
			agents.push_back(ag);
			for (int j = 0; j < num_canti; j++) {
				ag->set_idea(j, agents_ideas[i][j]);
				ag->set_pre_idea(j, agents_ideas[i][j]);
			}
			cout << ag->get_id() << ", " << ag->get_ideas()[0] << ", " << ag->get_ideas()[1] << ", " << ag->get_persuasion() << ", " << ag->get_susceptibility() << "\n";
			mat[x][y] = agent_counter++;
		}
	}

	update_centers();

	mat_prec = new int*[n]();
	for (int i = 0; i < n; i++) {
		mat_prec[i] = new int[n]();
		for (int j = 0; j < n; j++)
			mat_prec[i][j] = mat[i][j];
	}
}

Environment::~Environment() {
	//matrix
	for (int i = 0; i < N; i++) {
		delete mat[i];
		delete mat_prec[i];
	}
	delete mat;
	delete mat_prec;

	//centers
	for (int i = 0; i < num_canti; i++) {
		delete centers[i];
		delete canti[i];
	}
	delete centers;
	delete canti;

	//agents
	for (int i = 0; i < NA; i++)
		delete agents[i];
	agents.clear();
}

bool Environment::is_allowed_in_position(int x, int y) {
	if ((x >= 0 && x < N && y >= 0 && y < N) && (mat[x][y] == -1)) return true;
	return false;
}

void Environment::set_in_position(int x, int y, int old_x, int old_y) {
	int tmp = mat[old_x][old_y];
	mat[old_x][old_y] = -1;
	mat[x][y] = tmp;
}

void Environment::init_interactions() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			mat_prec[i][j] = mat[i][j];

	update_centers();
	
	for (int i = 0; i < NA; i++) {
		agents[i]->set_followers_pre_step(agents[i]->get_actual_followers());
		agents[i]->set_actual_followers(0);
		agents[i]->set_idea_to_play();
	}
	set_leaders();
}

void Environment::update_centers() {
	//Vector of agents for each idea
	vector<Agent*>* arr = new vector<Agent*>[num_canti]();
	//Divide agents considering their prominent ideas
	for (int i = 0; i < NA; i++) {
		arr[agents[i]->get_actual_prominent_idea()].push_back(agents[i]);
		agents[i]->setLeader(false);
	}

	/*Group Leader*/
	for (int i = 0; i < num_canti; i++) {
		int max = 0;
		int most_followed_agent_position = -1;
		//Search for most followed agent
		for (int j = 0; j < arr[i].size(); j++)
			if (arr[i][j]->get_actual_followers() > max)
				most_followed_agent_position = j;
		if (most_followed_agent_position > -1) {
			arr[i][most_followed_agent_position]->setLeader(true);
			centers[i][0] = arr[i][most_followed_agent_position]->get_position()[0];
			centers[i][1] = arr[i][most_followed_agent_position]->get_position()[1];
		}
		else {
			centers[i][0] = N / 2;
			centers[i][1] = N / 2;
		}
	}
	//Cleaning
	for (int i = 0; i < num_canti; i++) {
		while (!arr[i].empty())
			arr[i].pop_back();
		arr[i].clear();
	}
	delete[] arr;

	/* Group Center
	for (int i = 0; i < num_canti; i++) {
	double x_m = 0, y_m = 0;
	for (int j = 0; j < arr[i].size(); j++) {
	x_m += arr[i][j]->get_position()[0];
	y_m += arr[i][j]->get_position()[1];
	}
	if (arr[i].size() > 0) {
	centers[i][0] = x_m / ((double)arr[i].size());
	centers[i][1] = y_m / ((double)arr[i].size());
	}
	else {
	centers[i][0] = N/2;
	centers[i][1] = N/2;
	}
	}*/
}

void Environment::set_leaders() {
	//
}

int Environment::get_dim() {
	return N;
}

int Environment::get_num_canti() {
	return num_canti;
}

int** Environment::get_canti() {
	return canti;
}

int** Environment::get_centers() {
	return centers;
}

int Environment::get_num_agents() {
	return NA;
}

Agent* Environment::get_agent(int i) {
	return agents[i];
}

Agent* Environment::get_agent_in_position(int x, int y, int z) {
	if (z == 0)
		if (mat[x][y] >= 0) return get_agent(mat[x][y]);
	if (z == 1)
		if (mat_prec[x][y] >= 0) return get_agent(mat_prec[x][y]);
	return NULL;
}

void Environment::set_canti() {
	switch (num_canti) {
	case 1:
		canti[0][0] = 0;
		canti[0][1] = 0;
		break;
	case 2:
		canti[0][0] = 0;
		canti[0][1] = 0;
		canti[1][0] = N - 1;
		canti[1][1] = N - 1;
		break;
	case 3:
		canti[0][0] = 0;
		canti[0][1] = 0;
		canti[1][0] = 0;
		canti[1][1] = N - 1;
		canti[2][0] = N - 1;
		canti[2][1] = 0;
		break;
	case 4:
		canti[0][0] = 0;
		canti[0][1] = 0;
		canti[1][0] = 0;
		canti[1][1] = N - 1;
		canti[2][0] = N - 1;
		canti[2][1] = 0;
		canti[3][0] = N - 1;
		canti[3][1] = N - 1;
		break;
	}
}

void Environment::print_mat() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			if(mat[i][j] == -1)
				cout << ".\t";
			else
				cout << mat[i][j] << "\t";
		cout << "\n\n";
	}
}

void Environment::print_agents_position() {
	for (int i = 0; i < NA; i++) {
		cout << agents[i]->get_name() << " position: " << agents[i]->get_position()[0] << "," << agents[i]->get_position()[1] << "\n";
	}
}

void Environment::print_agents_ideas() {
	for (int i = 0; i < NA; i++) {
		cout << agents[i]->get_name() << " ideas: ";
		int j = 0;
		for (; j < num_canti - 1; j++){
			cout << agents[i]->get_ideas()[j] << ", ";
		}
		cout << agents[i]->get_ideas()[j] << "\n";
	}
}

int Environment::uniform_decision_pick(double* arr, int size) {
	double decision = rand() % 1;
	int i = 0;
	for (double floor = 0.0; i < size - 1; i++) {
		if (decision >= floor && decision <= (floor + arr[i]))
			return i;
		floor += arr[i];
	}
}