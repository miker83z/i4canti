#ifndef AGENT_HPP
#define AGENT_HPP

class Agent{
public:
	Agent();
	~Agent();
	void move();

private:
	int position[2];
};

#endif // AGENT_HPP