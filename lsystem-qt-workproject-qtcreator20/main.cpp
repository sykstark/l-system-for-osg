#include <QtCore/QCoreApplication>
#include <iostream>
#include <fstream>
#include "lsystemexception.h"
#include <exception>
#include <boost/program_options.hpp>
#include <iterator>
#include "log.h"
#include "lsystemgenerator.h"

using namespace AP_LSystem;
using namespace boost::program_options;
using namespace std;

template<class T>
ostream& operator<<(ostream& os, const std::vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

int main(int argc, char *argv[])
{


    std::string filename = "c:\\Documents and Settings\\mpa\\Dokumenty\\lsystem\\ls\\loadingTest12.ls";
    std::string lsw = "test.lsw";
//    std::string cfg = "c:\\Documents and Settings\\mpa\\Dokumenty\\lsystem\\lsconfig01.cfg";

 /*   FunctionParser f;
    f.Parse("x+2", "x", false);

    double * p = new double[1];
    p[0] = 2.0;
    double d = f.Eval(p);

    cout << d << endl;*/
/*
    options_description config_file_options;
    variables_map vm, vm2;
    std::ifstream ifs(cfg.c_str());
    config_file_options.add_options()
            ("vec1",value<double>())
            ("vec2",value<double>());

    std::stringstream stream;
    stream << "vec1=2.3" << endl;
    stream << "vec2=3.5" << endl;

    if(stream)
    {
        store(parse_config_file(stream, config_file_options), vm);
        notify(vm);
    }


    std::stringstream stream2;
    stream2 << "vec1=1.0" << endl;
    //stream2 << "vec2=1.0" << endl;

    if(stream2)
    {
        store(parse_config_file(stream2, config_file_options), vm2);
        notify(vm2);

        //variable_value val = vm2.begin()->second;
        //vm[vm2.begin()->first] = val;

        if(vm.count(vm2.begin()->first))
        {
            vm.erase(vm2.begin()->first);
            vm.insert( *vm2.begin() );
        }
    }

//    config_file_options.add_options()("vec1", value<double>()->default_value(3.0));

    if (vm.count("vec1"))
    {
        cout << "vec1: "
             << vm["vec1"].as< double >() << "\n";
    }
    if (vm.count("vec2"))
    {
        cout << "vec2: "
             << vm["vec2"].as< double >() << "\n";
    }
*/
   AbstractGenerator * generator;
    LongString k,* l;
    l = new LongString;
    string w = "FAFAF";
    l->convertFromString(&w);
    k = *l;
    delete l;
    Log::get()->write(k.toString());

    try
    {
        generator = new LSystemGenerator();
        generator->loadFile(filename);

        for( int i = 0; i < 3 ; i++)
        {
            Log::get()->write("nextIteration");
            generator->nextIteration();
        }
        generator->getWord();

//      generator->saveWordToFile( lsw );


    }
    catch( LSystemException &ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}
