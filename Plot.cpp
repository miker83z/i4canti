#include "Plot.h"
#include "csvfile.h"

csvfile* agts;
csvfile* prop; 

Plot::Plot(Environment* e, string directory) {
	agts = new csvfile(directory +"\\Agents.csv");
	prop = new csvfile(directory + "\\SimProperties.csv");

	env = e;

	//Agents.csv
	*agts << "Time/Agent";
	for (int i = 0; i < env->get_agents_number(); i++) {
		*agts << to_string(env->get_agent(i)->get_id());
	}
	*agts << endrow;

	*agts << "Initial Values";
	for (int i = 0; i < env->get_agents_number(); i++) {
		Agent *a = env->get_agent(i);
		string s = "";
		for (int j = 0; j < env->get_ideas_number(); j++)
			s += to_string(a->get_ideas()[j]) + ',';
		s += to_string(a->get_persuasion()) + ',' + to_string(a->get_susceptibility());
		*agts << s;
	}
	*agts << endrow;

	//SimulationProperties
	*prop << "Environment Dimension N" << env->get_dimension_size() << endrow;
	*prop << "Number of Agents" << env->get_agents_number() << endrow;
	*prop << "Number of Ideas" << env->get_ideas_number() << endrow;
	*prop << "Radius" << env->get_radius() << endrow;
	string tmp_s = "Ideas based";
	if (!env->is_ideas_based())
		tmp_s = to_string(env->get_threshold());
	*prop << "Dissimilar Threshold" << tmp_s << endrow;
}

Plot::~Plot() {
	delete agts;
	delete prop;
}

void Plot::update_tick(int tick) {
	*agts << tick;
	for (int i = 0; i < env->get_agents_number(); i++) {
		Agent *a = env->get_agent(i);
		string s = "";
		for (int j = 0; j < env->get_ideas_number(); j++) 
				s += to_string(a->get_ideas()[j]) + ',';
		s += to_string(a->hasMoved());
		*agts << s;
	}
	*agts << endrow;
}