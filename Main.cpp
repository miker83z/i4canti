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
#include "GUI.h"

using namespace std;
const bool CONSOLE = FALSE;

Windows window2(Point(w1 + 10, 0), w2, h2, "Controller");
//Line line1(Point(0, h / 4), Point(w, h / 4), 3, FL_WHITE);
//Line line2(Point(w / 2, 0), Point(w / 2, h / 4), 3, FL_WHITE);
//Text text1(Point(50, 50), FL_HELVETICA, 25, FL_RED, "Parametri");

Slider slider1(Point(50, 80), 100, 30, "Time speed");
In_box inbox1(Point(50, 120), 100, 30, "Agents (MAX 15k)");
In_box inbox2(Point(50, 160), 100, 30, "Field dim NxN (MAX 300)");
In_box inbox3(Point(50, 200), 100, 30, "Canti (1, 2, 3 or 4)");
In_box inbox4(Point(50, 240), 100, 30, "EndTime");
Text textT(Point(215, 260), FL_HELVETICA, 15, FL_WHITE, "0");

vector<Circle*> circles;
Button* b1p;

#include "GUI.cpp"
Button button1(Point(50, h / 2 - 50), 100, 30, "Go", callback1);

int main(int argc, char **argv) {
	if (CONSOLE) {
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
	}
	
	srand(time(NULL));

	window2.attach(slider1);
	window2.attach(inbox1);
	window2.attach(inbox2);
	window2.attach(inbox3);
	window2.attach(inbox4);
	window2.attach(button1);
	window2.attach(textT);

	slider1.set_value(500);
	slider1.set_bounds(25, 1000);
	inbox1.set_value("100");
	inbox2.set_value("50");
	inbox3.set_value("4");
	inbox4.set_value("1000");
	b1p = &button1;
	return Fl::run();
}
//-------------------------------------------------------------------------------------------------
