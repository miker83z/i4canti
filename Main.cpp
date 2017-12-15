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
	/*Fl_Double_Window win(400, 400);
	DrawAgents tim(10, 10, win.w() - 20, win.h() - 20, 0);
	win.show();
	return(Fl::run());*/
	system("PAUSE");
	return 0;
}
//-------------------------------------------------------------------------------------------------
