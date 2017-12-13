#ifndef DRAWAGENTS_HPP
#define DRAWAGENTS_HPP

static void Agent_RD(void *userdata);

class DrawAgents : public Fl_Box {

public:
	DrawAgents(int X, int Y, int W, int H, const char*L);

private:
	void draw();

};

#endif 