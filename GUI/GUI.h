#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using Callback = void(*)(Fl_Widget*, void*);

struct Windows;

int NA;
int N;
int NC = 2;
int radius;
int agents_tiers[4];
double* agents_properties[4];
double* agents_ideas[4];
int* numbers_shuffle;
int END_TIME;
double R;
Fl_Color canti_col[4];

bool PAUSE = FALSE;
bool CONTINUE = TRUE;
bool STOP = FALSE;
bool RUNNING = FALSE;
bool FIRST_TIME = TRUE;

bool SIM_DATA_FLAG = FALSE;
bool random_flag[18] = { FALSE };

bool init();
void callback1(Fl_Widget*, void*);
void closing();
void pause(Fl_Widget*, void*);
void continueF(Fl_Widget*, void*);
void stop(Fl_Widget*, void*);
void open_dialog(string s);
void close_dialog(Fl_Widget*, void*);
void set_flag(Fl_Widget*, void*);
void slider_change(Fl_Widget*, void*);
void radio_change(Fl_Widget*, void*);
Fl_Color get_color_from_ideas(double *ideas);
void show_sliders(); 
void show_sliders_after_change(int old_nc);

#include "Objects/Point.h"
#include "Objects/Shape.h"
#include "Objects/Line.h"
#include "Objects/Circle.h"
#include "Objects/Text.h"
#include "Objects/Widget.h"
#include "Objects/Slider.h"
#include "Objects/In_box.h"
#include "Objects/Button.h"
#include "Objects/CheckButton.h"
#include "Objects/RadioButton.h"
#include "Objects/Out_box.h"
#include "Objects/Windows.h"