#include "precompiled.h"

#include "abstractgenerator.h"
#include "d0lsystem.h"
#include "parstoch0lsystem.h"
#include "par2lsystem.h"

using namespace AP_LSystem;

AbstractLSystem * AbstractGenerator::createLSystem( AbstractFile * file )
{
    AbstractLSystem * lsystem;
    if( D0LSystem::isCapable( file->type() ) )
        lsystem = new D0LSystem( file );
    else if( ParStoch0LSystem::isCapable( file->type() ) )
        lsystem = new ParStoch0LSystem( file );
    else if( Par2LSystem::isCapable( file->type() ) )
        lsystem = new Par2LSystem( file );
    else
        throw ParsingException("non of L-systems fulfils the conditions");

    return lsystem;
}
