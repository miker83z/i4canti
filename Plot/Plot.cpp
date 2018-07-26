#include "Plot.h"
#include "csvfile.h"

csvfile* agts;
csvfile* prop; 

Plot::Plot( Environment* e, string directory) {
	agts = new csvfile(directory +"\\Agents.csv");
	prop = new csvfile(directory + "\\SimProperties.csv");

	env = e;

	//Agents.csv
	for (int i = 0; i < env->get_agents_number(); i++) {
		*agts << to_string(env->get_agent(i)->get_id());
	}
	*agts << endrow;

	//SimulationProperties
	*prop << "Environment Dimension N" << env->get_dimension_size() << endrow;
	*prop << "Number of Agents" << env->get_agents_number() << endrow;
	*prop << "Number of Ideas" << env->get_ideas_number() << endrow;
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
		for (int j = 0; j < env->get_ideas_number(); j++) {
			if (j != env->get_ideas_number() - 1)
				s += to_string(a->get_ideas()[j]) + ',';
			else
				s += to_string(a->get_ideas()[j]);
		}
		*agts << s;
	}
	*agts << endrow;
}