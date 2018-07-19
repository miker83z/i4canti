#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>
#include "Agent.h"
#include "Environment.h"
#include "Plot.h"
#include "GUI.h"
#include "GUI.cpp"

using namespace std;
const bool CONSOLE = FALSE;

//Line line1(Point(0, h / 4), Point(w, h / 4), 3, FL_WHITE);
//Line line2(Point(w / 2, 0), Point(w / 2, h / 4), 3, FL_WHITE);
//Text text1(Point(50, 50), FL_HELVETICA, 25, FL_RED, "Parametri");

int main(int argc, char **argv) {
	/*if (CONSOLE) {
		int N = 10;		//Matrix Dim
		int NC = 4;		//Number of Canti (ideas)
		int NA = 20;	//Number of Agents
		int STEP = 10;
		int END_TIME = 100;
		if (FALSE) {
			cout << "Insert Matrix Dim : ";
			cin >> N;
			cout << "Insert Number of Agents: ";
			cin >> NA;
			cout << "Insert Number of Canti : ";
			cin >> NC;
			cout << "Insert Number of Iterations(Time) : ";
			cin >> END_TIME;
			cout << "Insert Number of Iterations between Steps : ";
			cin >> STEP;
		}

		Environment* env = new Environment(N, NA, NC, N/10);
		env->print_mat();

		for (int time = 0; time < END_TIME; time++) {
			env->init_interactions();
			for (int i = 0; i < NA; i++) {
				env->get_agent(i)->tick();
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
	}*/
	
	srand(time(NULL));
	gui_start();
	return Fl::run();
}
//-------------------------------------------------------------------------------------------------
