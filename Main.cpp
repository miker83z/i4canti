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

const int N = 100;
const int canti = 2;

int main(int argc, char **argv) {
	//Matrix* mat = new Matrix(N, canti);
	//Agent a(mat);
	//a.move();
	//a.move();
	//a.move();
	Fl_Double_Window win(400, 400);
	DrawAgents tim(10, 10, win.w() - 20, win.h() - 20, 0);
	win.show();
	return(Fl::run());
}
//-------------------------------------------------------------------------------------------------
