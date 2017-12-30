#include <iostream>
#include <time.h>
#include "Environment.h"
#include "Agent.h"

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
	set_canti(nc);

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
	print_agents_position();
	pos_verified = new int[na]();
	idea_verified = new int[na]();
	interaction_verified = new int*[na]();	//could be optimized
	for (int i = 0; i < na; i++) 
		interaction_verified[i] = new int[na]();
}

int Environment::get_dim() {
	return N;
}

int Environment::get_canti() {
	return num_canti;
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

void Environment::set_canti(int canto) {
	switch (canto) {
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

bool Environment::is_allowed_in_position(int x, int y) {
	for (int i = 0; i < num_canti; i++)
		if (canti[i][0] == x && canti[i][1] == y) return false;
	if ( (x >= 0 && x < N && y >= 0 && y < N) && (mat[x][y] == -1)) return true;
	return false;
}

void Environment::set_in_position(int x, int y, int old_x, int old_y) {
	mat[x][y] = mat[old_x][old_y];
	mat[old_x][old_y] = -1;
}

bool Environment::check_interaction(Agent* first, Agent* second) {
	if (interaction_verified[first->get_id()][second->get_id()]) return false;
	return true;
}

void Environment::set_interaction(Agent* first, Agent* second) {
	interaction_verified[first->get_id()][second->get_id()] = 1;
	interaction_verified[second->get_id()][first->get_id()] = 1;
}

void Environment::init_interactions() {
	interaction_verified = new int*[NA]();
	for (int i = 0; i < NA; i++)
		interaction_verified[i] = new int[NA]();

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