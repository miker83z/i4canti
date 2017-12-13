#include "form.h"
#include "Agent.h"
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <DrawAgents.h>
<<<<<<< HEAD
=======

>>>>>>> 4e82b116715f55f7b4b7ac4a4572c713a051a00f

const int N = 100;
const int canti = 2;
extern Frame   *scene;

//-------------------------------------------------------------------------------------------------
void idle_cb(void*)
{
  scene->redraw();    
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv) {
	Matrix* mat = new Matrix(N, canti);
	Agent a(mat);
	a.move();
	a.move();
	a.move();
	//CreateMyWindow();
	//Fl::add_idle(idle_cb, 0);
	//Fl::run();
	//Fl_Double_Window win(400, 400);
	//MyTimer tim(10, 10, win.w() - 20, win.h() - 20);
	//win.show();
	return(Fl::run());
	system("pause");
	return 0;
	Fl_Double_Window win(400, 400);
	MyTimer tim(10, 10, win.w() - 20, win.h() - 20);
	win.show();
	return(Fl::run());
}
//-------------------------------------------------------------------------------------------------
