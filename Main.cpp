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

const int N = 100;
const int canti = 2;
const int NA = 10;
static unsigned i = 0;

using namespace std;

vector<DrawAgents*> allAgents;

int main(int argc, char **argv) {

	bool pos_verified[NA];
	bool id_verified[NA];
	//Matrix* mat = new Matrix(N, canti);
	//Agent a(mat);
	//a.move();
	//a.move();
	//a.move();
	Fl_Double_Window *window = new Fl_Double_Window(400, 400, "FLTK Test");

	for (i = 0; i < 10; i++) {

		allAgents.push_back(new DrawAgents(10*i, 10*i, 50, 50, 0));

	}

	window->end();
	Fl::visual(FL_DOUBLE|FL_INDEX);
	window->show();

	return(Fl::run());

}
//-------------------------------------------------------------------------------------------------
