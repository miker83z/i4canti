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
#include "GUI.h"

using namespace std;

const int N = 14;		//Matrix Dim
const int NC = 4;		//Number of Canti (ideas)
const int NA = 50;		//Number of Agents

const int STEP = 10;
const int END_TIME = 200;

Windows window(Point(w, 0), w, h, "Progetto");
Line line1(Point(0, h / 4), Point(w, h / 4), 3, FL_WHITE);
Line line2(Point(w / 2, 0), Point(w / 2, h / 4), 3, FL_WHITE);
Text text1(Point(50, 50), FL_HELVETICA, 25, FL_RED, "Parametri");

Slider slider1(Point(50, 80), 100, 30, "Velocità agenti");
In_box inbox1(Point(50, 120), 100, 30, "Numero di agenti");

#include "callback1.h"
Button button1(Point(w / 2 + 200, 50), 100, 30, "Avvia", callback1);

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
	
	srand(time(NULL));
	window.attach(line1);
	window.attach(line2);
	window.attach(text1);

	window.attach(slider1);
	window.attach(inbox1);
	window.attach(button1);

	slider1.set_value(500);
	slider1.set_bounds(100, 1000);
	inbox1.set_value("10");
	return Fl::run();
}
//-------------------------------------------------------------------------------------------------
