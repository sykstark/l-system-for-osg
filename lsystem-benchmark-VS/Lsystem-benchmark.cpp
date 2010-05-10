// Lsystem-benchmark.cpp : Defines the entry point for the console application.
//

#include "precompiled.h"
#include "LSFileGrammar.h"
#include "basic_dostream.h"

using namespace AP_LSystem;

int _tmain(int argc, _TCHAR* argv[])
{
	AbstractGrammar * grammar;
	dostream debug;

	std::string filename = "d:\\HCI\\VRECKO\\bin\\data\\ls\\loadingTest02.ls";

	if( filename.empty() )
	{
		return 0;
	}

	unsigned int pos = filename.rfind( '.' );
	if( pos == std::string::npos )
	{
		return 0;
	}

	std::string ext = filename.substr( pos + 1, std::string::npos );

	if( ext == "ls" )
	{
		grammar = new LSFileGrammar( &filename );
		for( int i = 0; i < 17 ; i++)
		{
			grammar->nextIteration();
		}

//		cout << dynamic_cast<LSFileGrammar *>(grammar)->wordLength();
	}

//	int i;
//	cin >> i;

	return 0;
}

