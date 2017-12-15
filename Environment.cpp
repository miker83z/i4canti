#include <iostream>
#include "Environment.h"
#include "Agent.h"

Environment::Environment(int n, int na, int n_canti, vector<Agent*>* agts) {
	this->N = n;
	this->num_canti = n_canti;
	mat = new int*[n];
	for (int i = 0; i < n; i++)
		mat[i] = new int[n];
	canti = new int*[n_canti];
	for (int i = 0; i < n_canti; i++) 
		canti[i] = new int[2];
	set_canti(n_canti);

	agents = agts;
	pos_verified = new bool[na];
	idea_verified = new bool[na];
	interaction_verified = new bool*[na+1];
	for (int i = 0; i < n+1; i++) 
		interaction_verified[i] = new bool[na+1];
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
	return agents->at(i);
}

Agent* Environment::get_agent_in_position(int x, int y) {
	if (mat[x][y] > 0) return get_agent(mat[x][y]);
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
		canti[1][0] = N - 1;
		canti[1][1] = 0;
		break;
	case 3:
		canti[0][0] = 0;
		canti[0][1] = 0;
		canti[1][0] = N - 1;
		canti[1][1] = 0;
		canti[2][0] = 0;
		canti[2][1] = N - 1;
		break;
	case 4:
		canti[0][0] = 0;
		canti[0][1] = 0;
		canti[1][0] = N - 1;
		canti[1][1] = 0;
		canti[2][0] = 0;
		canti[2][1] = N - 1;
		canti[3][0] = N - 1;
		canti[3][1] = N - 1;
		break;
	}
}

bool Environment::is_allowed_in_position(int x, int y) {
	for (int i = 0; i < num_canti; i++) 
		if (x == canti[i][0] && y == canti[i][1]) return false;
	if (x < 0 || x > N || y < 0 || y > N || mat[x][y] == 1) return false;
	else return true;
}

void Environment::init_position(int id, int x, int y) {
	mat[x][y] = id;
}

void Environment::set_in_position(int x, int y, int o_x, int o_y) {
	mat[x][y] = mat[o_x][o_y];
	mat[o_x][o_y] = 0;
}