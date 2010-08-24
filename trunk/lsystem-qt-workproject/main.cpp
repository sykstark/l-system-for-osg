#include <QtCore/QCoreApplication>
#include <iostream>
#include <fstream>
#include "LSFileGrammar.h"
#include "lsystemexception.h"
#include <boost/program_options.hpp>
#include <iterator>

using namespace AP_LSystem;
using namespace boost::program_options;

template<class T>
ostream& operator<<(ostream& os, const std::vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

int main(int argc, char *argv[])
{


    std::string filename = "c:\\Documents and Settings\\mpa\\Dokumenty\\lsystem\\ls\\loadingTest01.ls";
    std::string cfg = "c:\\Documents and Settings\\mpa\\Dokumenty\\lsystem\\config01.cfg";

 /*   FunctionParser f;
    f.Parse("x+2", "x", false);

    double * p = new double[1];
    p[0] = 2.0;
    double d = f.Eval(p);

    cout << d << endl;*/

 /*   options_description config_file_options;
    variables_map vm;
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
    }*/


    AbstractGrammar * grammar;

    try
    {
        grammar = new LSFileGrammar( &filename );
        for( int i = 0; i < 2 ; i++)
        {
            grammar->nextIteration();
        }

        //cout << grammar->translate() << endl;

        cout << dynamic_cast<LSFileGrammar *>(grammar)->wordLength() << endl;

        if (Configuration::get()->getProperty( "TEST01", "texture" ) != NULL )
        {
            cout << Configuration::get()->getProperty( "TEST01", "texture" )->as<string>() << endl;
        }
    }
    catch( LSystemException &ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}
