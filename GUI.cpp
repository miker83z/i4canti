#include <iomanip>
#include <algorithm> 
#include <ctime>
#include <sstream>

int w = Fl::w(), h = Fl::h();
double scal = (1920.0 / (double)w );
double scal2 = (1080.0 / (double)h);
int w1 = (scal - abs(log(scal)) )* (w * 3 / 8), h1 = (scal - abs(log(scal))) * (w * 3 / 8);	//Window1
int w2 = scal * (w / 6), h2 = scal2 * (h / 1.87);			//Window2
int w3 = scal * (w / 3), h3 = scal2 * (h / 1.87);		//Window3
Plot *plot;

//Window2 Controller
Windows window2(Point(w1 + 8, 200), w2, h2, "Controller", fl_rgb_color(45, 45, 45));
In_box inboxN(Point(50, 160), 50, 30, "Env dim NxN (MAX 300)", FL_WHITE);
In_box inboxEndTime(Point(50, 240), 50, 30, "EndTime", FL_WHITE);
Text textN(Point(50, 300), FL_HELVETICA, 15, FL_WHITE, "N /");
In_box inboxRadius(Point(70, 280), 50, 30, "Radius", FL_WHITE);
Button buttonGO(Point(50, 465), 100, 30, "Go", callback1);

RadioButton *radio = new RadioButton[3]();
Text textIdeas(Point(145, 220), FL_HELVETICA, 15, FL_WHITE, "Ideas");
RadioButton *radio_thresh = new RadioButton[2]();
Text textThresh(Point(50, 340), FL_HELVETICA, 15, FL_WHITE, "Dissimilar Threshold");
Slider sliderThresh(Point(155, 370), 125, 20, "", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);

//Window3 Agents
Windows window3(Point(w1 + 8 + w2, 200), w3, h3, "Agents", fl_rgb_color(45, 45, 45));

Text textTier1(Point(20, 45), FL_HELVETICA_BOLD, 20, fl_rgb_color(224, 180, 203), "AGENTS TIER 1");
In_box inboxTier1(Point(180, 24), 50, 30, "", fl_rgb_color(255, 40, 40));
Slider *sliderTier1I = new Slider[4]();
CheckButton buttonTier1I(Point(150, 100), 70, 30, "Rand", set_flag, 1);
Slider sliderTier1P(Point(20, 200), 100, 30, "Persuasion", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier1P(Point(220, 200), 70, 30, "Rand", set_flag, 2);
Slider sliderTier1S(Point(20, 240), 100, 30, "Susceptibility", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier1S(Point(220, 240), 60, 30, "Rand", set_flag, 3);

Text textTier2(Point(350, 45), FL_HELVETICA_BOLD, 20, fl_rgb_color(194, 180, 223), "AGENTS TIER 2");
In_box inboxTier2(Point(510, 24), 50, 30, "", fl_rgb_color(255, 40, 40));
Slider *sliderTier2I = new Slider[4]();
CheckButton buttonTier2I(Point(480, 100), 70, 30, "Rand", set_flag, 4);
Slider sliderTier2P(Point(350, 200), 100, 30, "Persuasion", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier2P(Point(550, 200), 70, 30, "Rand", set_flag, 5);
Slider sliderTier2S(Point(350, 240), 100, 30, "Susceptibility", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier2S(Point(550, 240), 60, 30, "Rand", set_flag, 6);

Text textTier3(Point(20, 315), FL_HELVETICA_BOLD, 20, fl_rgb_color(194, 220, 203), "AGENTS TIER 3");
In_box inboxTier3(Point(180, 294), 50, 30, "", fl_rgb_color(255, 40, 40));
Slider *sliderTier3I = new Slider[4]();
CheckButton buttonTier3I(Point(150, 370), 70, 30, "Rand", set_flag, 7);
Slider sliderTier3P(Point(20, 470), 100, 30, "Persuasion", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier3P(Point(220, 470), 70, 30, "Rand", set_flag, 8);
Slider sliderTier3S(Point(20, 510), 100, 30, "Susceptibility", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier3S(Point(220, 510), 60, 30, "Rand", set_flag, 9);

Text textTier4(Point(350, 315), FL_HELVETICA_BOLD, 20, fl_rgb_color(224, 210, 233), "AGENTS TIER 4");
In_box inboxTier4(Point(510, 294), 50, 30, "", fl_rgb_color(255, 40, 40));
Slider *sliderTier4I = new Slider[4]();
CheckButton buttonTier4I(Point(480, 370), 70, 30, "Rand", set_flag, 10);
Slider sliderTier4P(Point(350, 470), 100, 30, "Persuasion", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier4P(Point(550, 470), 70, 30, "Rand", set_flag, 11);
Slider sliderTier4S(Point(350, 510), 100, 30, "Susceptibility", FL_HOR_SLIDER, FL_ALIGN_RIGHT, FL_WHITE, 0, NULL);
CheckButton buttonTier4S(Point(550, 510), 60, 30, "Rand", set_flag, 12);

//Window1 Environment 
Windows window(Point(w1 / 2, 200), w1/2, h1/3, "Simulations number", fl_rgb_color(45, 45, 45));
In_box simNum(Point(85, 120), 50, 30, "Sim Number", FL_WHITE);
Text textSM(Point(270, 140), FL_HELVETICA, 15, FL_WHITE, "0");
Text textExit(Point(75, 90), FL_HELVETICA, 15, FL_WHITE, "Click X on the console to exit/stop");
Text textFin(Point(130, 180), FL_HELVETICA, 26, FL_WHITE, "Finished");

//Dialog Window
Windows* dialog;
Text textDialog(Point(50, 50), FL_HELVETICA, 15, FL_WHITE, "Errore");
Button buttonDialog(Point(w1 / 4 - 100, h1 / 4 - 50), 100, 30, "Ok", close_dialog);

string OutputFolder = "";

bool init() {
	try {
		N = stoi(inboxN.get_value());
		if (N > 300) { open_dialog("Error: Environment dimension (N) too large"); return false; }

		agents_tiers[0] = stoi(inboxTier1.get_value());
		agents_tiers[1] = stoi(inboxTier2.get_value());
		agents_tiers[2] = stoi(inboxTier3.get_value());
		agents_tiers[3] = stoi(inboxTier4.get_value());
		NA = agents_tiers[0] + agents_tiers[1] + agents_tiers[2] + agents_tiers[3];
		if (NA > 20000 || NA > N * N - 1) { open_dialog("Error: Too many agents"); return false; }

		//radius for agents view
		radius = stoi(inboxRadius.get_value());
		if (radius < 1 || radius > N) { open_dialog("Error: Radius not allowed"); return false; }
		radius = N / radius;

		if (N < 1 || NA < 1 || NC < 1) { open_dialog("Error"); return false; }

		R = (300 / N) * (scal - abs(1.8 * log(scal)));

		END_TIME = stoi(inboxEndTime.get_value());
		if (END_TIME < 1 || NA > 100000) { open_dialog("Error End Time"); return false; }

		END_SIMS = stoi(simNum.get_value());
	}
	catch (const std::exception&) { open_dialog("Error"); return false; }

	//agents properties and ideas
	for (int i = 0; i < 4; i++) {
		agents_properties[i] = new double[2]();
		agents_ideas[i] = new double[NC]();
	}

	// properties
	agents_properties[0][0] = random_flag[1] ? -1 : sliderTier1P.get_value();
	agents_properties[0][1] = random_flag[2] ? -1 : sliderTier1S.get_value();
	agents_properties[1][0] = random_flag[4] ? -1 : sliderTier2P.get_value();
	agents_properties[1][1] = random_flag[5] ? -1 : sliderTier2S.get_value();
	agents_properties[2][0] = random_flag[7] ? -1 : sliderTier3P.get_value();
	agents_properties[2][1] = random_flag[8] ? -1 : sliderTier3S.get_value();
	agents_properties[3][0] = random_flag[10] ? -1 : sliderTier4P.get_value();
	agents_properties[3][1] = random_flag[11] ? -1 : sliderTier4S.get_value();

	// ideas
	for (int i = 0; i < NC; i++) {
		agents_ideas[0][i] = random_flag[0] ? -1 : sliderTier1I[i].get_value();
		agents_ideas[1][i] = random_flag[3] ? -1 : sliderTier2I[i].get_value();
		agents_ideas[2][i] = random_flag[6] ? -1 : sliderTier3I[i].get_value();
		agents_ideas[3][i] = random_flag[9] ? -1 : sliderTier4I[i].get_value();
	}

	// threshold
	thr = sliderThresh.get_value();

	canti_col[0] = fl_rgb_color(255, 10, 10);
	canti_col[1] = fl_rgb_color(10, 10, 255);
	canti_col[2] = fl_rgb_color(10, 255, 10);
	canti_col[3] = fl_rgb_color(255, 255, 255);

	buttonGO.hide();

	return true;
}

// main function
void callback1(Fl_Widget*, void*) {
	if (init()) {

		// CSV writing
		try {
			auto t = time(nullptr);
			auto tm = *localtime(&t);
			ostringstream oss;
			oss << std::put_time(&tm, "_%Y%m%d_%H-%M-%S");

			OutputFolder = "MultipleSimData\\Sim" + oss.str();
			if (!CreateDirectory(OutputFolder.c_str(), NULL) && !ERROR_ALREADY_EXISTS == GetLastError()) {
				throw exception();
			}
			plot = new Plot(OutputFolder, END_SIMS, END_TIME, NC, 4, agents_tiers, agents_properties, agents_ideas);
		}
		catch (const std::exception &ex) { std::cout << "Exception was thrown: " << ex.what() << std::endl; }
		
		for (int simi = 0; simi < END_SIMS; simi++) {
			textSM.set_content(to_string(simi));
			if (simi < 11 || !(simi % 25)) {
				Fl::check();
				Fl::redraw();
			}

			// create environment
			Environment env(N, NA, NC, radius, 4, agents_tiers, agents_properties, agents_ideas, id_basd, thr);
			plot->set_env(&env);
			int *numbers_shuffle = env.get_agents_shuffle();
			

			for (int time = 0; time < END_TIME && !STOP; time++) {

				//MAIN PROCEDURE
				env.init_interactions();
				random_shuffle(&numbers_shuffle[0], &numbers_shuffle[NA]);
				for (int i = 0; i < NA; i++) {
					int agent_id = numbers_shuffle[i];

					//TICK
					env.get_agent(agent_id)->tick();
					/////
				}
				plot->update_tick(time);
			}

		}
		plot->close_plot();
		window.attach(textFin);
		Fl::check();
		Fl::redraw();
		string python = "python plot.py .\\" + OutputFolder + "\\";
		system(python.c_str());
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

void plotF(Fl_Widget*, void*) {
	if (SIM_DATA_FLAG && !OutputFolder.empty()) {
		string python = "python plot.py .\\" + OutputFolder + "\\";
		system(python.c_str());
	}
}

void stop(Fl_Widget*, void*) {
	STOP = TRUE;
}

void open_dialog(string s) {
	dialog = new Windows(Point(w / 2 - w1 / 4, h / 2 - h1 / 8), w1 / 2, h1 / 4, "Error", fl_rgb_color(45, 45, 45));
	dialog->attach(textDialog);
	dialog->attach(buttonDialog);

	cout << s + "\n";
	textDialog.set_content(s);
	dialog->show();
	while (dialog->shown()) Fl::wait();
}

void close_dialog(Fl_Widget*, void*) {
	dialog->hide();
	delete dialog;
}

void slider_change(Fl_Widget*w, void *par) {
	Slider *sliders = (Slider*)par;
	int idea = 0;
	for (int i = 0; i < NC; i++)
		if (sliders[i].equals(((Fl_Value_Slider*)w)))
			idea = i;
	double value = sliders[idea].get_value();
	double newsum = 1 - value;
	double sum = 0.0;
	for (int i = 0; i < NC; i++)
		if (i != idea)
			sum += sliders[i].get_value();
	for (int i = 0; i < NC; i++)
		if (i != idea)
			sliders[i].set_value(min(max( .03/(NC - 1.0), (sliders[i].get_value()*newsum)/sum), .97) );
}

void radio_change(Fl_Widget*w, void *par) {
	RadioButton *buttons = (RadioButton*)par;
	int mainOne = 0;
	for (int i = 0; i < 3; i++)
		if (buttons[i].equals(((Fl_Round_Button*)w)))
			mainOne = i;
	for (int i = 0; i < 3; i++)
		if (i == mainOne)
			buttons[i].on();
		else
			buttons[i].off();

	int tmp = NC;
	NC = mainOne + 2;
	show_sliders_after_change(tmp);
}

void radio_thresh_change(Fl_Widget*w, void *par) {
	RadioButton *buttons = (RadioButton*)par;
	int mainOne = 0;
	for (int i = 0; i < 2; i++)
		if (buttons[i].equals(((Fl_Round_Button*)w)))
			mainOne = i;
	for (int i = 0; i < 2; i++)
		if (i == mainOne)
			buttons[i].on();
		else
			buttons[i].off();

	id_basd = mainOne ? FALSE : TRUE;
}

void set_flag(Fl_Widget*w, void* i) {
	int tmp = *(int*)i;
	if (tmp == 0)
		if (!RUNNING)
			SIM_DATA_FLAG = !SIM_DATA_FLAG;
		else 
			((Fl_Check_Button*)w)->value(1-((int)((Fl_Check_Button*)w)->value()));
	else
		random_flag[--tmp] = !random_flag[tmp];
}

void gui_start() {
	window.attach(simNum);
	simNum.set_value("100");
	window.attach(textSM);
	window.attach(textExit);

	window2.attach(inboxN);
	inboxN.set_value("25");
	window2.attach(inboxEndTime);
	inboxEndTime.set_value("1000");
	window2.attach(textN);
	window2.attach(inboxRadius);
	inboxRadius.set_value("3");
	window2.attach(buttonGO);
	radio[0] = RadioButton(Point(50, 198), 20, 20, "2", TRUE, radio_change, radio);
	radio[1] = RadioButton(Point(80, 198), 20, 20, "3", TRUE, radio_change, radio);
	radio[2] = RadioButton(Point(110, 198), 20, 20, "4", TRUE, radio_change, radio);
	window2.attach(radio[0]);
	window2.attach(radio[1]);
	window2.attach(radio[2]);
	radio[0].on();
	window2.attach(textIdeas);
	radio_thresh[0] = RadioButton(Point(50, 345), 20, 20, "Based on ideas values", FALSE, radio_thresh_change, radio_thresh);
	radio_thresh[1] = RadioButton(Point(50, 370), 20, 20, "Based on T=", FALSE, radio_thresh_change, radio_thresh);
	window2.attach(radio_thresh[0]);
	window2.attach(radio_thresh[1]);
	radio_thresh[1].on();
	window2.attach(textThresh);
	window2.attach(sliderThresh);
	sliderThresh.set_value(.3);

	window3.attach(textTier1);
	window3.attach(inboxTier1);
	window3.attach(inboxTier1);
	inboxTier1.set_value("25");
	window3.attach(sliderTier1P);
	window3.attach(buttonTier1I);
	sliderTier1P.set_value(.5);
	sliderTier1P.set_bounds(0, 1);
	window3.attach(buttonTier1P);
	window3.attach(sliderTier1S);
	sliderTier1S.set_value(.5);
	sliderTier1S.set_bounds(0, 1);
	window3.attach(buttonTier1S);

	window3.attach(textTier2);
	window3.attach(inboxTier2);
	window3.attach(inboxTier2);
	inboxTier2.set_value("25");
	window3.attach(sliderTier2P);
	window3.attach(buttonTier2I);
	sliderTier2P.set_value(.5);
	sliderTier2P.set_bounds(0, 1);
	window3.attach(buttonTier2P);
	window3.attach(sliderTier2S);
	sliderTier2S.set_value(.5);
	sliderTier2S.set_bounds(0, 1);
	window3.attach(buttonTier2S);

	window3.attach(textTier3);
	window3.attach(inboxTier3);
	window3.attach(inboxTier3);
	inboxTier3.set_value("0");
	window3.attach(sliderTier3P);
	window3.attach(buttonTier3I);
	sliderTier3P.set_value(.5);
	sliderTier3P.set_bounds(0, 1);
	window3.attach(buttonTier3P);
	window3.attach(sliderTier3S);
	sliderTier3S.set_value(.5);
	sliderTier3S.set_bounds(0, 1);
	window3.attach(buttonTier3S);

	window3.attach(textTier4);
	window3.attach(inboxTier4);
	window3.attach(inboxTier4);
	inboxTier4.set_value("0");
	window3.attach(sliderTier4P);
	window3.attach(buttonTier4I);
	sliderTier4P.set_value(.5);
	sliderTier4P.set_bounds(0, 1);
	window3.attach(buttonTier4P);
	window3.attach(sliderTier4S);
	sliderTier4S.set_value(.5);
	sliderTier4S.set_bounds(0, 1);
	window3.attach(buttonTier4S);

	show_sliders();
}

void show_sliders_after_change(int old_nc) {
	for (int i = 0; i < old_nc; i++)
		Fl::delete_widget(sliderTier1I[i].pw);
	delete[] sliderTier1I;
	sliderTier1I = new Slider[4]();
	for (int i = 0; i < old_nc; i++)
		Fl::delete_widget(sliderTier2I[i].pw);
	delete[] sliderTier2I;
	sliderTier2I = new Slider[4]();
	for (int i = 0; i < old_nc; i++)
		Fl::delete_widget(sliderTier3I[i].pw);
	delete[] sliderTier3I;
	sliderTier3I = new Slider[4]();
	for (int i = 0; i < old_nc; i++)
		Fl::delete_widget(sliderTier4I[i].pw);
	delete[] sliderTier4I;
	sliderTier4I = new Slider[4]();
	Fl::check();
	Fl::redraw();
	show_sliders();
}

void show_sliders() {
	sliderTier1I[0] = Slider(Point(20, 55), 25, 125, "1", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 40, 40), slider_change, sliderTier1I);
	window3.attach(sliderTier1I[0]);
	sliderTier1I[0].set_value(1.0 / NC);
	sliderTier1I[0].set_bounds(0.03 / (NC - 1), .97);
	sliderTier1I[0].set_param(sliderTier1I);
	sliderTier1I[1] = Slider(Point(47, 55), 25, 125, "2", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(30, 80, 255), slider_change, sliderTier1I);
	window3.attach(sliderTier1I[1]);
	sliderTier1I[1].set_value(1.0 / NC);
	sliderTier1I[1].set_bounds(0.03 / (NC - 1), .97);
	sliderTier1I[1].set_param(sliderTier1I);
	if (NC > 2) {
		sliderTier1I[2] = Slider(Point(74, 55), 25, 125, "3", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(80, 255, 70), slider_change, sliderTier1I);
		window3.attach(sliderTier1I[2]);
		sliderTier1I[2].set_value(1.0 / NC);
		sliderTier1I[2].set_bounds(0.03 / (NC - 1), .97);
		sliderTier1I[2].set_param(sliderTier1I);
	}
	if (NC > 3) {
		sliderTier1I[3] = Slider(Point(101, 55), 25, 125, "4", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 255, 255), slider_change, sliderTier1I);
		window3.attach(sliderTier1I[3]);
		sliderTier1I[3].set_value(1.0 / NC);
		sliderTier1I[3].set_bounds(0.03 / (NC - 1), .97);
		sliderTier1I[3].set_param(sliderTier1I);
	}

	sliderTier2I[0] = Slider(Point(350, 55), 25, 125, "1", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 40, 40), slider_change, sliderTier2I);
	window3.attach(sliderTier2I[0]);
	sliderTier2I[0].set_value(1.0 / NC);
	sliderTier2I[0].set_bounds(0.03 / (NC - 1), .97);
	sliderTier2I[0].set_param(sliderTier2I);
	sliderTier2I[1] = Slider(Point(377, 55), 25, 125, "2", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(30, 80, 255), slider_change, sliderTier2I);
	window3.attach(sliderTier2I[1]);
	sliderTier2I[1].set_value(1.0 / NC);
	sliderTier2I[1].set_bounds(0.03 / (NC - 1), .97);
	sliderTier2I[1].set_param(sliderTier2I);
	if (NC > 2) {
		sliderTier2I[2] = Slider(Point(404, 55), 25, 125, "3", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(80, 255, 70), slider_change, sliderTier2I);
		window3.attach(sliderTier2I[2]);
		sliderTier2I[2].set_value(1.0 / NC);
		sliderTier2I[2].set_bounds(0.03 / (NC - 1), .97);
		sliderTier2I[2].set_param(sliderTier2I);
	}
	if (NC > 3) {
		sliderTier2I[3] = Slider(Point(431, 55), 25, 125, "4", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 255, 255), slider_change, sliderTier2I);
		window3.attach(sliderTier2I[3]);
		sliderTier2I[3].set_value(1.0 / NC);
		sliderTier2I[3].set_bounds(0.03 / (NC - 1), .97);
		sliderTier2I[3].set_param(sliderTier2I);
	}

	sliderTier3I[0] = Slider(Point(20, 325), 25, 125, "1", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 40, 40), slider_change, sliderTier3I);
	window3.attach(sliderTier3I[0]);
	sliderTier3I[0].set_value(1.0 / NC);
	sliderTier3I[0].set_bounds(0.03 / (NC - 1), .97);
	sliderTier3I[0].set_param(sliderTier3I);
	sliderTier3I[1] = Slider(Point(47, 325), 25, 125, "2", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(30, 80, 255), slider_change, sliderTier3I);
	window3.attach(sliderTier3I[1]);
	sliderTier3I[1].set_value(1.0 / NC);
	sliderTier3I[1].set_bounds(0.03 / (NC - 1), .97);
	sliderTier3I[1].set_param(sliderTier3I);
	if (NC > 2) {
		sliderTier3I[2] = Slider(Point(74, 325), 25, 125, "3", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(80, 255, 70), slider_change, sliderTier3I);
		window3.attach(sliderTier3I[2]);
		sliderTier3I[2].set_value(1.0 / NC);
		sliderTier3I[2].set_bounds(0.03 / (NC - 1), .97);
		sliderTier3I[2].set_param(sliderTier3I);
	}
	if (NC > 3) {
		sliderTier3I[3] = Slider(Point(101, 325), 25, 125, "4", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 255, 255), slider_change, sliderTier3I);
		window3.attach(sliderTier3I[3]);
		sliderTier3I[3].set_value(1.0 / NC);
		sliderTier3I[3].set_bounds(0.03 / (NC - 1), .97);
		sliderTier3I[3].set_param(sliderTier3I);
	}

	sliderTier4I[0] = Slider(Point(350, 325), 25, 125, "1", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 40, 40), slider_change, sliderTier4I);
	window3.attach(sliderTier4I[0]);
	sliderTier4I[0].set_value(1.0 / NC);
	sliderTier4I[0].set_bounds(0.03 / (NC - 1), .97);
	sliderTier4I[0].set_param(sliderTier4I);
	sliderTier4I[1] = Slider(Point(377, 325), 25, 125, "2", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(30, 80, 255), slider_change, sliderTier4I);
	window3.attach(sliderTier4I[1]);
	sliderTier4I[1].set_value(1.0 / NC);
	sliderTier4I[1].set_bounds(0.03 / (NC - 1), .97);
	sliderTier4I[1].set_param(sliderTier4I);
	if (NC > 2) {
		sliderTier4I[2] = Slider(Point(404, 325), 25, 125, "3", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(80, 255, 70), slider_change, sliderTier4I);
		window3.attach(sliderTier4I[2]);
		sliderTier4I[2].set_value(1.0 / NC);
		sliderTier4I[2].set_bounds(0.03 / (NC - 1), .97);
		sliderTier4I[2].set_param(sliderTier4I);
	}
	if (NC > 3) {
		sliderTier4I[3] = Slider(Point(431, 325), 25, 125, "4", FL_VERT_SLIDER, FL_ALIGN_BOTTOM, fl_rgb_color(255, 255, 255), slider_change, sliderTier4I);
		window3.attach(sliderTier4I[3]);
		sliderTier4I[3].set_value(1.0 / NC);
		sliderTier4I[3].set_bounds(0.03 / (NC - 1), .97);
		sliderTier4I[3].set_param(sliderTier4I);
	}
}
