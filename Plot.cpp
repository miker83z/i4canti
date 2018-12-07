#include "Plot.h"
#include "csvfile.h"

csvfile* agts;
csvfile* prop;
bool first_time = true;

Plot::Plot(string directory, int sims_en, int time_en, int num_i, int tiers_n, int* agents_ti, double** agents_prop, double** agents_id) {
	agts = new csvfile(directory +"\\Agents.csv");
	prop = new csvfile(directory + "\\SimProperties.csv");
	sims_end = sims_en;
	time_end = time_en;
	num_ideas = num_i;

	ticks_moves = new int[time_end]();
	ticks_values = new double*[time_end]();
	ticks_agts = new int*[time_end]();
	for (int i = 0; i < time_end; i++) {
		ticks_values[i] = new double[num_ideas]();
		ticks_agts[i] = new int[num_ideas]();
	}

	//Agents.csv
	*agts << "Time/Mean Values" << "Mean moves";
	for (int i = 0; i < num_ideas; i++) {
		string i_s = to_string(i);
		*agts << "Idea " + i_s + " mean value" << "Num of Agents for Idea " + i_s;
	}
	*agts << endrow;

	tiers_number = tiers_n;
	agents_tiers = agents_ti;
	agents_properties = agents_prop;
	agents_ideas = agents_id;
}

Plot::~Plot() {
	delete agts;
	delete prop;
}

void Plot::update_tick(int tick) {
	vector<double> ideas_tmp(num_ideas, 0.0);
	vector<int> num_agts_tmp(num_ideas, 0);
	int tmp_moves = 0;

	for (int i = 0; i < env->get_agents_number(); i++) {
		Agent *a = env->get_agent(i);

		for (int j = 0; j < num_ideas; j++)
			ideas_tmp[j] += a->get_ideas()[j];
		num_agts_tmp[a->get_actual_prominent_idea()]++;
		if(a->hasMoved()) tmp_moves++;
	}
	ticks_moves[tick] += tmp_moves;
	for (int j = 0; j < num_ideas; j++) {
		ticks_values[tick][j] += (double)(ideas_tmp[j] / (double)env->get_agents_number());
		ticks_agts[tick][j] += num_agts_tmp[j];
	}
}


void Plot::close_plot() {
	for (int i = 0; i < time_end; i++) {
		*agts << to_string(i) << to_string( ((double)ticks_moves[i] / (double)sims_end) );
		for (int j = 0; j < num_ideas; j++)
			*agts << to_string((double)(ticks_values[i][j] / (double)sims_end));
		for (int j = 0; j < num_ideas; j++)
			*agts << to_string(((double)ticks_agts[i][j] / (double)sims_end));
		*agts << endrow;
	}
}

void Plot::set_env(Environment* e) {
	env = e;
	if (first_time) {
		first_time = false;
		set_prop();
	}
}

void Plot::set_prop() {
	//SimulationProperties
	*prop << "Environment Dimension N" << env->get_dimension_size() << endrow;
	*prop << "Number of Agents" << env->get_agents_number() << endrow;
	*prop << "Number of Ideas" << env->get_ideas_number() << endrow;
	*prop << "Duration" << time_end << endrow;
	*prop << "Simulations" << sims_end << endrow;
	*prop << "Radius" << env->get_radius() << endrow;
	string tmp_s = "Ideas based";
	if (!env->is_ideas_based())
		tmp_s = to_string(env->get_threshold());
	*prop << "Dissimilar Threshold" << tmp_s << endrow;
	for (int i = 0; i < tiers_number; i++) {
		string tmp_ideas = "";
		for (int j = 0; j < env->get_ideas_number(); j++)
			tmp_ideas += (agents_ideas[i][j] < 0 ? "random" : to_string(agents_ideas[i][j])) + ",";
		tmp_ideas = tmp_ideas.substr(0, tmp_ideas.size() - 1);
		*prop << "Tier " + to_string(i + 1) << "NA=" + to_string(agents_tiers[i]) << "p=" + (agents_properties[i][0] < 0 ? "random" : to_string(agents_properties[i][0])) << "s=" +
			(agents_properties[i][1] < 0 ? "random" : to_string(agents_properties[i][1])) << tmp_ideas << endrow;
	}
}