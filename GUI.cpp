
int NA = 100;
int N = 50;
int NC = 4;
int END_TIME = 1000;
double R = 4;

bool PAUSE = FALSE;
bool CONTINUE = TRUE;
bool STOP = FALSE;

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
	if (NA > 20000) { cout << "Too many agents\n"; return false; }
	NC = stoi(inbox3.get_value());
	if (NC > 4) { cout << "Only 1,2,3 or 4 Canti are allowed"; return false; }
	END_TIME = 0;
	if (N < 1 || NA < 1 || NC < 1 ) { cout << "Error"; return false; }

	R = 300 / N;
	PAUSE = FALSE;

	if (!circles.empty()) { 
		circles.clear();
		Fl::check();
		Fl::redraw();
	}

	canti_col[0] = fl_rgb_color(255, 0, 0);
	canti_col[1] = fl_rgb_color(0, 255, 0);
	canti_col[2] = fl_rgb_color(0, 0, 255);
	canti_col[3] = fl_rgb_color(255, 255, 255);

	window2.attach(button2);
	button2.show();
	window2.attach(button3);
	button2.show();
	window2.attach(button4);
	button2.show();
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

	delete window;
	window = new Windows(Point(0, 0), w1, h1, "Environment");

	PAUSE = FALSE;
	CONTINUE = TRUE;
	STOP = FALSE;

	Fl::check();
	Fl::redraw();
}

void callback1(Fl_Widget*, void*) {
	if (init()) {
		double xmin = R, xmax = w1 + R, ymin = R, ymax = h1 + R;

		Environment* env = new Environment(N, NA, NC);

		for (int i = 0; i < NA; i++) {
			double x0 = (double)env->get_agent(i)->get_position()[0] / N * (xmax - xmin) + xmin;
			double y0 = (double)env->get_agent(i)->get_position()[1] / N * (ymax - ymin) + ymin;
			circles.push_back(new Circle(Point(x0, y0), R, 1, FL_WHITE, FL_WHITE));
			window->attach(*circles[i]);
		}

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

				env->init_interactions();
				for (int i = 0; i < NA; i++) {
					env->get_agent(i)->move();
					double x0 = (double)env->get_agent(i)->get_position()[0] / N * (xmax - xmin) + xmin;
					double y0 = (double)env->get_agent(i)->get_position()[1] / N * (ymax - ymin) + ymin;
					circles[i]->relocate(x0, y0);

					env->get_agent(i)->interact();
					circles[i]->recolor(get_color_from_ideas(env->get_agent(i)->get_ideas()));
				}
				Fl::check();
				Fl::redraw();
				Sleep(slider1.get_value());
			}
			if (STOP) break;
		}
		delete env;
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

/*
double R{15}, D{2*R}, dt{1};
double xmin{R}, xmax{w-R}, ymin{h/4+R}, ymax{h-R-30};
vector<double>x, y, vx, vy;

void boundary(int i) {
	if (x[i]>xmax) {
		vx[i] *= -1;
		x[i] = xmax;
	}
	else if (x[i]<xmin) {
		vx[i] *= -1;
		x[i] = xmin;
	}
	if (y[i]>ymax) {
		vy[i] *= -1;
		y[i] = ymax;
	}
	else if (y[i]<ymin) {
		vy[i] *= -1;
		y[i] = ymin;
	}
}

void collision(int i, int j) {
	double xij = x[j] - x[i];
	double yij = y[j] - y[i];
	double rij = sqrt(xij*xij + yij*yij);
	if (rij<D) {
		double xiju = xij / rij;
		double yiju = yij / rij;
		double dv = xiju * (vx[j] - vx[i]) + yiju * (vy[j] - vy[i]);
		vx[i] += xiju * dv;
		vy[i] += yiju * dv;
		vx[j] -= xiju * dv;
		vy[j] -= yiju * dv;
		x[i] += vx[i] * dt;
		y[i] += vy[i] * dt;
		x[j] += vx[j] * dt;
		y[j] += vy[j] * dt;
		xij = x[j] - x[i];
		yij = y[j] - y[i];
		rij = sqrt(xij*xij + yij*yij);
		if (rij < D) {
			x[i] += (D - rij) * xiju;
			y[i] += (D - rij) * yiju;
			x[j] -= (D - rij) * xiju;
			y[j] -= (D - rij) * yiju;
		}
	}
}

void callback1(Fl_Widget*, void*) {
	ofstream out{ "collisions_vs_time.txt" };
	int N = stoi(inbox1.get_value());
	double v0{ slider1.get_value()*0.0001 };
	vector<Circle*> circles;
	for (int i = 0; i<N; i++) {
		double x0{ rand() / double(RAND_MAX)*(xmax - xmin) + xmin }, y0{ rand() / double(RAND_MAX)*(ymax - ymin) + ymin };
		x.push_back(x0);
		y.push_back(y0);
		if (i % 2 == 0) {
			circles.push_back(new Circle(Point(x0, y0), R, 1, FL_WHITE, FL_WHITE));
		}
		else {
			circles.push_back(new Circle(Point(x0, y0), R, 1, FL_RED, FL_RED));
		}
		window.attach(*circles[i]);
		double theta{ rand() / double(RAND_MAX) * 2 * 3.1416 };
		vx.push_back(v0*cos(theta));
		vy.push_back(v0*sin(theta));
	}

	while (true) {
		for (int i = 0; i<N; i++) {
			for (int j = 0; j<N && j != i; j++) {
				boundary(i);
				collision(i, j);
			}
			x[i] += vx[i] * dt;
			y[i] += vy[i] * dt;
			circles[i]->relocate(x[i], y[i]);
			double v = slider1.get_value()*0.0001;
			v0 = sqrt(vx[i] * vx[i] + vy[i] * vy[i]);
			vx[i] *= v / v0;
			vy[i] *= v / v0;
		}
		Fl::check();
		Fl::redraw();
	}
}*/


