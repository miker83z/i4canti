#include <iomanip> 
#include <sstream>

int NA;
int N;
int NC;
int END_TIME;
double R;

bool PAUSE = FALSE;
bool CONTINUE = TRUE;
bool STOP = FALSE;
bool FIRST_TIME = TRUE;

void pause(Fl_Widget*, void*);
void continueF(Fl_Widget*, void*);
void stop(Fl_Widget*, void*);
Fl_Color get_color_from_ideas(double *ideas);

Fl_Color canti_col[4];

Windows* window = new Windows(Point(0, 0), w1, h1, "Environment");
Button button2(Point(250, 80), 70, 30, "Pause", pause);
Button button3(Point(250, 240), 70, 30, "Continue", continueF);
Button button4(Point(200, h / 2 - 50), 100, 30, "Stop", stop);

bool init() {
	N = stoi(inbox2.get_value());
	if (N > 300) { cout << "Field Dim too large\n"; return false; }
	NA = stoi(inbox1.get_value());
	if (NA > 20000 || NA > ((N*N) - 4)) { cout << "Too many agents\n"; return false; }
	NC = stoi(inbox3.get_value());
	if (NC > 4) { cout << "Only 1,2,3 or 4 Canti are allowed"; return false; }
	END_TIME = 0;
	if (N < 1 || NA < 1 || NC < 1 ) { cout << "Error"; return false; }

	R = 300 / N;

	canti_col[0] = fl_rgb_color(255, 0, 0);
	canti_col[1] = fl_rgb_color(0, 255, 0);
	canti_col[2] = fl_rgb_color(0, 0, 255);
	canti_col[3] = fl_rgb_color(255, 255, 255);

	if (FIRST_TIME) {
		window2.attach(button2);
		window2.attach(button3);
		window2.attach(button4);
		FIRST_TIME = FALSE;
	}
	button2.show();
	button3.show();
	button4.show();
	b1p->hide();

	return true;
}

void closing() {
	button2.hide();
	button3.hide();
	button4.hide();
	b1p->show();

	for (int i = 0; i < NA; i++)
		delete circles[i];
	circles.clear();

	PAUSE = FALSE;
	CONTINUE = TRUE;
	STOP = FALSE;

	delete window;
	window = new Windows(Point(0, 0), w1, h1, "Environment");
	Fl::check();
	Fl::redraw();
}

void callback1(Fl_Widget*, void*) {
	if (init()) {
		double xmin = R, xmax = w1 + R, ymin = R, ymax = h1 + R;

		Environment env(N, NA, NC);
		//env.print_interpersonal_influence();

		/////////Temp
		Fl_Double_Window win(w1 + w2 + 13, 0, w1 / 2, h1);
		Fl_Scroll scroll(0, 0, w1 / 2, h1);
		vector<Fl_Multiline_Output *> o;

		for (int i = 0; i < NA; i++) {
			double x0 = (double)env.get_agent(i)->get_position()[0] / N * (xmax - xmin) + xmin;
			double y0 = (double)env.get_agent(i)->get_position()[1] / N * (ymax - ymin) + ymin;
			circles.push_back(new Circle(Point(x0, y0), R, 1, get_color_from_ideas(env.get_agent(i)->get_ideas()), FL_WHITE));
			window->attach(*circles[i]);

			o.push_back(new Fl_Multiline_Output(0, i * 44, (w1 / 2) - 15, 44));
			o[i]->value(("Agt " + to_string(i)).c_str());
		}

		scroll.end();
		win.end();
		win.resizable(win);
		win.show();
		/////////Temp

		int time = 0;
		while (TRUE) {
			Sleep(200);
			Fl::check();
			if (CONTINUE) {								//CONTINUE
				END_TIME += stoi(inbox4.get_value());
				CONTINUE = FALSE;
			}
			for (; time < END_TIME && !STOP; time++) {

				if (PAUSE) {							//PAUSE											
					button2.set_label("Play");
					while (PAUSE) {
						Sleep(200);
						Fl::check();
					}
					button2.set_label("Pause");
				}

				textT.set_content(to_string(time));

				env.init_interactions();
				for (int i = 0; i < NA; i++) {
					env.get_agent(i)->move();
					double x0 = (double)env.get_agent(i)->get_position()[0] / N * (xmax - xmin) + xmin;
					double y0 = (double)env.get_agent(i)->get_position()[1] / N * (ymax - ymin) + ymin;
					circles[i]->relocate(x0, y0);

					env.get_agent(i)->interact();
					circles[i]->recolor(get_color_from_ideas(env.get_agent(i)->get_ideas()));

					double *tmp = env.get_agent(i)->get_ideas();
					string s = "Agt\t\t";
					for (int j = 0; j < env.get_num_canti(); j++) {
						stringstream stream;
						stream << fixed << setprecision(2) << tmp[j];
						s += stream.str() + "\t";
					}
					s +="\n" + to_string(i) + "\t\t";
					tmp = env.get_agent(i)->get_direction();
					for (int j = 0; j < env.get_num_canti(); j++) {
						stringstream stream;
						stream << fixed << setprecision(2) << tmp[j];
						s += stream.str() + "\t";
					}

					o[i]->value(s.c_str());
					o[i]->color(canti_col[env.get_agent(i)->get_prominent_idea()]);

				}
				Fl::check();
				Fl::redraw();
				Sleep(slider1.get_value());
			}
			if (STOP) {
				for (int i = 0; i < NA; i++)
					delete o[i];
				o.clear();
				break;
			}
		}
		closing();
	}
}

Fl_Color get_color_from_ideas(double *ideas) {
	if (NC == 1) return canti_col[0];
	int max[] = { 0, 1 };
	for (int i = 2; i < NC; i++) {
		if (ideas[i] > ideas[max[0]]) {
			if (ideas[max[0]] > ideas[max[1]])
				max[1] = max[0];
			max[0] = i;
		}
		else if (ideas[i] > ideas[max[1]])
			max[1] = i;
	}
	if (ideas[max[1]] > ideas[max[0]]) {
		int tmp = max[0];
		max[0] = max[1];
		max[1] = tmp;
	}
	return fl_color_average(canti_col[max[0]], canti_col[max[1]], ideas[max[0]] / (ideas[max[0]] + ideas[max[1]]));
}

void pause(Fl_Widget*, void*) {
	PAUSE = !PAUSE;
}

void continueF(Fl_Widget*, void*) {
	CONTINUE = TRUE;
}

void stop(Fl_Widget*, void*) {
	STOP = TRUE;
}
