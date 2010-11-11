#pragma once
#include "turtle.h"

using std::string;
using std::vector;

class Lparser
{
private:
	double _thickness, _angle, _length;
	string _axiom;
	unsigned int _detail, _recursion, _color, _turtleType;
	
	map<char, string> rules;
 	map<char, string> homomorphism;

	stack<Turtle*> turtles;
	
	vector<vmath::Vector3d> verticesOutput;
	vector<vmath::Vector3d> normalsOutput;
	vector<Indices> indicesOutput;

	double getParameterd(std::string *word);
	unsigned int getParameterui(std::string *word);

	bool pushContourTurtle();
	bool pushPolygonTurtle();
	bool popTurtle();

	string generateWord();
	string uncomment(string text);
	
public:
	Lparser():_length(1.0){};
	Lparser(double thickness, double angle, double length, unsigned int detail, unsigned int turtleType);
	~Lparser(void);

	bool loadFromFile(string filename);	
	bool parse(string word);
	inline vector<vmath::Vector3d> getVertex() { return verticesOutput;};
	inline vector<vmath::Vector3d> getNormals() { return normalsOutput;};
	inline vector<Indices> getIndices() { return indicesOutput;};

	void setThickness(int thickness);
	void setDefaultAngle(int angle);
};

