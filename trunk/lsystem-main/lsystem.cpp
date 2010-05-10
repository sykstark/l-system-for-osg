#include "precompiled.h"
#include "lsystem.h"

Lsystem::Lsystem(void)
{
}

Lsystem::~Lsystem(void)
{
}

void Lsystem::loadFromFile(string filename)
{
	ifstream file(filename.c_str());
	string line= "";
	if(file.is_open())
	{
		while(line.empty()) 
		{
			getline(file, line);
			line = uncomment(line);
		}
		istringstream strin(line);
		strin >> recursion;
		line.clear();
		strin.clear();

		while(line.empty()) 
		{
			getline(file, line);
			line = uncomment(line);
		}
		strin.str(line);
		strin >> angle;
		line.clear();
		strin.clear();

		while(line.empty()) 
		{
			getline(file, line);
			line = uncomment(line);
		}
		strin.str(line);
		strin >> thickness;
		line.clear();
		strin.clear();

		while(line.empty()) 
		{
			getline(file, line);
			line = uncomment(line);
		}
		axiom = line;
		line.clear();

		while(!file.eof())
		{
			while(line.empty()) 
			{
				getline(file, line);
				line = uncomment(line);
			}
			if(line.at(0) == '@') break;
			else 
			{
				rules.insert(make_pair<char,string>(line.at(0),line.substr(2)));
				line.clear();
			}
		}
	}
}

string Lsystem::uncomment(string text)
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

string Lsystem::generateWord()
{
	string word1, word2="";
	word1 = axiom;
	for(int j=0; j<recursion; j++)
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
	return word1;
}