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

const int N = 100;
const int canti = 2;
const int NA = 10;
static unsigned i = 0;

using namespace std;

vector<DrawAgents*> allAgents;

int main(int argc, char **argv) {
	srand(time(NULL));
	vector<Agent*> agents;
	Environment* env = new Environment(N, NA, canti, &agents);
	
	for (int i = 0; i < NA; i++) {
		agents.push_back(new Agent(env, rand() % N, rand() % N, i+1));
	}
	//env->get_agent(0)->move();
	for (int time = 0; time < 100; time++) {
		for (int i = 0; i < 1; i++) {
			agents[i]->move();
		}
	}
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
