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

int main(int argc, char **argv) {	
	srand(time(NULL));
	gui_start();
	return Fl::run();
}
