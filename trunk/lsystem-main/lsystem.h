#pragma once

using namespace std;

class Lsystem
{
private:
	int angle, thickness, recursion;
	string axiom;
	map<char, string> rules;

	string uncomment(string text);
public:
	Lsystem(void);
	~Lsystem(void);
	
	void loadFromFile(string filename);
	string generateWord();
};