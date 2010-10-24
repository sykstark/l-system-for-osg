#include "precompiled.h"
#include "lparser.h"
#include "turtle.h"
#include "polygonturtle.h"
#include "jointedturtle.h"
#include "contourturtle.h"
#include "LSFileGrammar.h"
#include "configuration.h"


Lparser::Lparser(double thickness, double angle, double length, unsigned int detail, unsigned int turtleType):
	  _thickness(thickness),_angle(angle),_length(length), _detail(detail), _color(0), _turtleType(turtleType)
{	
	
}

Lparser::~Lparser(void)
{
}

void Lparser::setDefaultAngle(int angle)
{
	_angle = angle;
}

void Lparser::setThickness(int thickness)
{
	_thickness = thickness;
}

bool Lparser::parse(string word)
{
	Turtle *t;
	if(_turtleType)	t = new ContourTurtle(_thickness,_angle,_length,_detail);
	else t = new JointedTurtle(_thickness,_angle,_length,_detail);
	turtles.push(t);

	//cerr << endl << "Processing: ";
	char ch;
	while(!word.empty())
	{
		ch = word.at(0);
		//cerr << ch;
		word.erase(word.begin());
		switch(ch)
		{
		case '[':
			pushContourTurtle();
			break;
		case ']':
			popTurtle();
			break;
		case '{':
			pushPolygonTurtle();
			break;
		case '}':
			popTurtle();
			break;
/*		case '.':  //record vertex
			vertexAndNormals = turtles.top()->recordVertex();
			vertexStack.top().push_back(vmath::Vector3d(vertexAndNormals.at(0).x,vertexAndNormals.at(0).y,vertexAndNormals.at(0).z));
			normalsStack.top().push_back(vmath::Vector3d(vertexAndNormals.at(1).x,vertexAndNormals.at(1).y,vertexAndNormals.at(1).z));
			break;
*/
		case 'f':  //krok dopredu - vertex
		case 'z':  //pulkrok dopredu - vertex
		case 'F': // krok dopredu s kreslenim
		case 'Z': // polovicni krok dopredu s kreslenim
			if((!word.empty()) && (word.at(0) == '(')) turtles.top()->forwardDrawMove(getParameterd(&word));
			else if(ch='F') turtles.top()->fullForwardDrawMove();
			else turtles.top()->halfForwardDrawMove();
			break;
		case '+':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->turnLeft(getParameterd(&word));
			}
			else
			{
				turtles.top()->turnLeft();
			}
			break;
		case '-':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->turnRight(getParameterd(&word));
			}
			else
			{
				turtles.top()->turnRight();
			}
			break;
		case '|':
			turtles.top()->turnArround();
			break;
		case '&':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->pitchDown(getParameterd(&word));
			}
			else
			{
				turtles.top()->pitchDown();
			}
			break;
		case '^':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->pitchUp(getParameterd(&word));
			}
			else
			{
				turtles.top()->pitchUp();
			}
			break;
		case '<':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->rollLeft(getParameterd(&word));
			}
			else
			{
				turtles.top()->rollLeft();
			}
			break;
		case '>':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->rollRight(getParameterd(&word));
			}
			else
			{
				turtles.top()->rollRight();
			}
			break;
		case '~':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->randomTurnPitchRoll(getParameterui(&word));
			}
			else
			{
				turtles.top()->randomTurnPitchRoll();
			}
			break;
		case '%':
			turtles.top()->rollArround();
			break;
		case '$':
			turtles.top()->rollUntilHorizontal();
			break;
		case '\"':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->multiplyLength(getParameterd(&word));
			}
			else
			{
				turtles.top()->multiplyLength(1.1);
			}
			break;
		case '\'':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->multiplyLength(getParameterd(&word));
			}
			else
			{
				turtles.top()->multiplyLength(0.9);
			}
			break;
		case ';':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->multiplyAngle(getParameterd(&word));
			}
			else
			{
				turtles.top()->multiplyAngle(1.1);
			}
			break;
		case ':':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->multiplyAngle(getParameterd(&word));
			}
			else
			{
				turtles.top()->multiplyAngle(0.9);
			}
			break;
		case '?':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->multiplyThickness(getParameterd(&word));
			}
			else
			{
				turtles.top()->multiplyThickness(1.4);
			}
			break;
		case '!':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->multiplyThickness(getParameterd(&word));
			}
			else
			{
				turtles.top()->multiplyThickness(0.7);
			}
			break;
		case '*':
			turtles.top()->resetValues();
			break;
		case 'c':
			if((!word.empty()) && (word.at(0) == '('))
			{
				turtles.top()->setColorIndex(getParameterui(&word));
			}
			else
			{
				turtles.top()->increaseColorIndex();
			}
			break;
		case '(':
			getParameterd(&word);
			break;
		default:
			continue;
		}
	}
	popTurtle();
	return true;
}
		
// zjisti parametr pohybu zelvy
double Lparser::getParameterd(std::string *word)
{
	word->erase(word->begin());
	char ch;
	string paramString;
	ch = word->at(0);
	word->erase(word->begin());
	while(ch != ')')
	{
		paramString.append(1,ch);
		ch = word->at(0);
		word->erase(word->begin());
	}
	stringstream ss(paramString);
	double param;
	ss >> param;
	return param;
}

unsigned int Lparser::getParameterui(std::string *word)
{
	word->erase(word->begin());
	char ch;
	string paramString;
	ch = word->at(0);
	word->erase(word->begin());
	while(ch != ')')
	{
		paramString.append(1,ch);
		ch = word->at(0);
		word->erase(word->begin());
	}
	stringstream ss(paramString);
	unsigned int param;
	ss >> param;
	return param;
}

// pri konci vetve se davaji vertexy a normaly do hlavniho vystupu
bool Lparser::pushContourTurtle()
{
	Turtle *t;
	if(_turtleType) t = new ContourTurtle(turtles.top(),_detail);
	else t = new JointedTurtle(turtles.top(),_detail);
	turtles.push(t);
	turtles.top()->initialize();
	return true;
}

bool Lparser::pushPolygonTurtle()
{
	PolygonTurtle *pt = new PolygonTurtle(turtles.top());
	turtles.push(pt);
	turtles.top()->initialize();
	return true;
}

bool Lparser::popTurtle()
{
	turtles.top()->finalize();
	vector<vmath::Vector3d> turtleVertices = turtles.top()->getVertices();
	vector<vmath::Vector3d> turtleNormals = turtles.top()->getNormals();
	vector<Indices> turtleIndices = turtles.top()->getIndices();
	verticesOutput.insert(verticesOutput.end(),turtleVertices.begin(),turtleVertices.end());
	normalsOutput.insert(normalsOutput.end(),turtleNormals.begin(),turtleNormals.end());
	indicesOutput.insert(indicesOutput.end(),turtleIndices.begin(),turtleIndices.end());
	turtles.pop();
	return true;
}

bool Lparser::loadFromFile(string filename)
{
	ifstream file(filename.c_str());
	string line= "";
	if(!file.is_open()) return false;
	
	while(line.empty()) 
	{
		getline(file, line);
		line = uncomment(line);
	}
	istringstream strin(line);
	strin >> _recursion;
	line.clear();
	strin.clear();

	while(line.empty()) 
	{
		getline(file, line);
		line = uncomment(line);
	}
	strin.str(line);
	strin >> _angle;
	line.clear();
	strin.clear();

	while(line.empty()) 
	{
		getline(file, line);
		line = uncomment(line);
	}
	strin.str(line);
	strin >> _thickness;
	_thickness = _length*_thickness/100.0;
	line.clear();
	strin.clear();

	while(line.empty()) 
	{
		getline(file, line);
		line = uncomment(line);
	}
	_axiom = line;
	line.clear();

	while(!file.eof())
	{
		while(line.empty()) 
		{
			getline(file, line);
			line = uncomment(line);
		}
		if(line.at(0) == '@') break;
		else if(line.at(1) == '=')
		{
			rules.insert(make_pair<char,string>(line.at(0),line.substr(2)));
			line.clear();
		}
		else if(line.at(1) == ':')
		{
			homomorphism.insert(make_pair<char,string>(line.at(0),line.substr(2)));
			line.clear();
		}
		else
		{
			cerr << "Chyba syntaxe" <<endl;
		}

	}
	return parse(generateWord());
}

string Lparser::generateWord()
{
	string word1, word2="";
	word1 = _axiom;
	for(unsigned int j=0; j<_recursion; j++)
	{
		for(unsigned int i=0; i<word1.length(); i++)
		{
			if(rules.count(word1.at(i)))
			{
				word2.append(rules[word1.at(i)]);
			}
			else
			{
				word2.insert(word2.end(),word1.at(i));
			}
		}
		word1 = word2;
		word2.clear();
	}
	for(unsigned int i=0; i<word1.length(); i++)
	{
		if(homomorphism.count(word1.at(i)))
		{
			word2.append(homomorphism[word1.at(i)]);
		}
		else
		{
			word2.insert(word2.end(),word1.at(i));
		}
	}
	word1 = word2;
	word2.clear();
	return word1;
}

string Lparser::uncomment(string text)
{
	size_t i=0;
	i = text.find_first_of('#',0);
	if(i!=string::npos) text.erase(i,text.size());

	i = text.find(" ", 0);
	while (i!=string::npos)
	{
		text.erase(i);
		i = text.find(" ", i);
	}
	return text;
}