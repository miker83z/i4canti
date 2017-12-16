#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "DrawAgents.h"
#include "Agent.h"
#include "Matrix.h"
#include <vector>

using namespace std;

const int N = 100;
const int canti = 2;
const int NA = 10;

vector<DrawAgents*> allAgents;

int main(int argc, char **argv) {
	//vector<Agent> agents;
	//bool pos_verified[NA];
	//bool id_verified[NA];
	//Matrix* mat = new Matrix(N, canti);
	//Agent a(mat);

	//a.move();
	//a.move();
	//a.move();
	for (int i = 0; i<8; ++i) {
		allAgents.push_back(new DrawAgents(i * 30, i * 30, 50, 50, i));
	}
	Fl_Double_Window *window = new Fl_Double_Window(400, 400, "FLTK Test");
	//Fl::add_timeout(2, winUpdate, window);
	window->end();
	Fl::visual(FL_DOUBLE | FL_INDEX);
	window->show();
	return Fl::run();
}
//-------------------------------------------------------------------------------------------------
