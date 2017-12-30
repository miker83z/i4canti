#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>
#include "DrawAgents.h"
#include "Agent.h"
#include "Environment.h"

using namespace std;

const int N = 14;		//Matrix Dim
const int NC = 4;		//Number of Canti (ideas)
const int NA = 50;		//Number of Agents

const int STEP = 10;
const int END_TIME = 200;

//static unsigned i = 0;
vector<DrawAgents*> allAgents;

int main(int argc, char **argv) {
	Environment* env = new Environment(N, NA, NC);
	env->print_mat();

	for (int time = 0; time < END_TIME; time++) {
		env->init_interactions();
		for (int i = 0; i < NA; i++) {
			env->get_agent(i)->move();
			env->get_agent(i)->interact();
		}
		if (!(time % STEP)) {
			char c;
			c = cin.get();
			if (c == 'q') break;
			cout << "__________________________________________________________________________________________________________\n";
			env->print_mat();
		}
	}
	
	env->print_agents_ideas();
	env->print_mat();
	system("PAUSE");
	return 0;
	
	/*Fl_Double_Window *window = new Fl_Double_Window(400, 400, "FLTK Test");

	for (i = 0; i < 10; i++) {

		allAgents.push_back(new DrawAgents(10*i, 10*i, 50, 50, 0));

	}

	window->end();
	Fl::visual(FL_DOUBLE|FL_INDEX);
	window->show();

	return(Fl::run());
	*/
}
//-------------------------------------------------------------------------------------------------
