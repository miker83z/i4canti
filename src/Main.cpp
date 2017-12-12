#include <FL/Fl.H>
#include "form.h"
#include "frame.h"
#include "Agent.h"

extern Frame   *scene;

//-------------------------------------------------------------------------------------------------
void idle_cb(void*)
{
  scene->redraw();    
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv) {
<<<<<<< HEAD
	Matrix* mat = new Matrix();
	Agent* a = new Agent(mat);
	a->move();
	a->move();
	a->move();
	//CreateMyWindow();
	//Fl::add_idle(idle_cb, 0);
	//Fl::run();
	system("pause");
=======
	Agent* a = new Agent();
	a.move();
	//CreateMyWindow();
	//Fl::add_idle(idle_cb, 0);
	//Fl::run();
>>>>>>> ff5c2712dba9798375967436e6dcf6a6d793d468
	return 0;
}
//-------------------------------------------------------------------------------------------------
