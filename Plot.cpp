#include "Plot.h"
#include "csvfile.h"

csvfile* agts;
csvfile* prop; 

Plot::Plot( Environment* e, string directory) {
	agts = new csvfile(directory +"\\Agents.csv");
	prop = new csvfile(directory + "\\SimProperties.csv");

	env = e;

	//Agents.csv
	for (int i = 0; i < env->get_num_agents(); i++) {
		*agts << to_string(env->get_agent(i)->get_id());
	}
	*agts << endrow;

	//SimulationProperties
	*prop << "Environment Dimension N" << env->get_dim() << endrow;
	*prop << "Number of Agents" << env->get_num_agents() << endrow;
	*prop << "Number of Ideas" << env->get_num_canti() << endrow;
}

Plot::~Plot() {
	delete agts;
	delete prop;
}

void Plot::update_tick(int tick) {
	*agts << tick;
	for (int i = 0; i < env->get_num_agents(); i++) {
		Agent *a = env->get_agent(i);
		string s = "";
		for (int j = 0; j < env->get_num_canti(); j++) {
			if (j != env->get_num_canti() - 1)
				s += to_string(a->get_ideas()[j]) + ',';
			else
				s += to_string(a->get_ideas()[j]);
		}
		*agts << s;
	}
	*agts << endrow;
}