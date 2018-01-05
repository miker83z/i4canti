#include <iostream>
#include <time.h>
#include "Environment.h"
#include "Agent.h"

const int W_MODE = 2;	//interpersonal influence, 0 = all equal, 1 = upper triangular

void Environment::set_interpersonal_influence() {
	switch (W_MODE) {
	case 0:
		for (int i = 0; i < NA; i++)
			for (int j = 0; j < NA; j++)
				interpersonal_influence[i][j] = 1 / (float)NA;
	case 1:
		for (int i = 0; i < NA; i++)
			if (i < NA / 2)
				for (int j = i; j < NA; j++)
					interpersonal_influence[i][j] = 1 / (float)(NA - i);
			else
				interpersonal_influence[i][i] = 1;
	case 2:
		for (int i = 0; i < NA; i++) {
			interpersonal_influence[i][i] = .7;
			if (i < NA / 2) {
				for (int j = 0; j < NA; j++)
					if (j != i)
						interpersonal_influence[i][j] = .3 / (float)(NA - 1);
			}
			else
				for (int j = i + 1; j < NA; j++)
					interpersonal_influence[i][j] = .3 / (float)(NA - i - 1);
		}
	}
}

Environment::Environment(int n, int na, int nc) {
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

	//canti
	canti = new int*[nc];
	for (int i = 0; i < nc; i++)
		canti[i] = new int[2];
	set_canti();

	//agents
	srand(time(NULL));
	for (int i = 0; i < na; i++) {
		int x = rand() % n, y = rand() % n;
		while (!is_allowed_in_position(x, y)) {
			x = rand() % n;
			y = rand() % n;
		}
		agents.push_back(new Agent(this, x, y, i));
		mat[x][y] = i;
	}
	interaction_verified = new int*[na]();
	for (int i = 0; i < na; i++) 
		interaction_verified[i] = new int[i+1]();

	interpersonal_influence = new float*[na]();
	for (int i = 0; i < na; i++) 
		interpersonal_influence[i] = new float[na]();
	set_interpersonal_influence();

	susceptibility = new float[na];
	for (int i = 0; i < na; i++)
		susceptibility[i] = 1 - interpersonal_influence[i][i];
}

Environment::~Environment() {
	//matrix
	for (int i = 0; i < N; i++)
		delete mat[i];
	delete mat;

	//canti
	for (int i = 0; i < num_canti; i++)
		delete canti[i];
	delete canti;

	//agents
	for (int i = 0; i < NA; i++)
		delete agents[i];
	agents.clear();

	for (int i = 0; i < NA; i++)
		delete interaction_verified[i];
	delete interaction_verified;

	for (int i = 0; i < NA; i++)
		delete interpersonal_influence[i];
	delete interpersonal_influence;

	delete susceptibility;
}

bool Environment::is_allowed_in_position(int x, int y) {
	for (int i = 0; i < num_canti; i++)
		if (canti[i][0] == x && canti[i][1] == y) return false;
	if ((x >= 0 && x < N && y >= 0 && y < N) && (mat[x][y] == -1)) return true;
	return false;
}

void Environment::set_in_position(int x, int y, int old_x, int old_y) {
	int tmp = mat[old_x][old_y];
	mat[old_x][old_y] = -1;
	mat[x][y] = tmp;
}

bool Environment::check_interaction(Agent* first, Agent* second) {
	int i = first->get_id();
	int j = second->get_id();
	if (i < j) {
		int tmp = i;
		i = j;
		j = tmp;
	}
	if (interaction_verified[i][j]) return false;
	return true;
}

void Environment::set_interaction(Agent* first, Agent* second) {
	int i = first->get_id();
	int j = second->get_id();
	if (i < j) {
		int tmp = i;
		i = j;
		j = tmp;
	}
	interaction_verified[i][j] = 1;
}

void Environment::init_interactions() {
	for (int i = 0; i < NA; i++)
		for (int j = 0; j < i + 1; j++)
			interaction_verified[i][j] = 0;
}

int Environment::get_dim() {
	return N;
}

int Environment::get_num_canti() {
	return num_canti;
}

int Environment::get_num_agents() {
	return NA;
}

int* Environment::get_canto_pos(int canto) {
	return canti[canto];
}

Agent* Environment::get_agent(int i) {
	return agents[i];
}

Agent* Environment::get_agent_in_position(int x, int y) {
	if (mat[x][y] >= 0) return get_agent(mat[x][y]);
	return NULL;
}

float Environment::get_interpersonal_influence(int influenced, int influencer) {
	return interpersonal_influence[influenced][influencer];
}

float Environment::get_susceptibility(int i) {
	return susceptibility[i];
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
		canti[1][0] = 0;
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