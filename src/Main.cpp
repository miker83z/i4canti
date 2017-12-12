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
	Matrix* mat = new Matrix();
	Agent* a = new Agent(mat);
	a->move();
	a->move();
	a->move();
	//CreateMyWindow();
	//Fl::add_idle(idle_cb, 0);
	//Fl::run();
	system("pause");
	return 0;
}
//-------------------------------------------------------------------------------------------------
