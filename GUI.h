#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using Callback = void(*)(Fl_Widget*, void*);

int w = Fl::w(), h = Fl::h();
int w1 = w * 3 / 8, h1 = w * 3 / 8;	//Window1
int w2 = w / 4, h2 = h / 2;			//Window2
struct Windows;

#include "Point.h"
#include "Shape.h"
#include "Line.h"
#include "Circle.h"
#include "Text.h"
#include "Widget.h"
#include "Slider.h"
#include "In_box.h"
#include "Button.h"
#include "Out_box.h"
#include "Windows.h"
