#ifndef ABSTRACTLSYSTEMEX_H
#define ABSTRACTLSYSTEMEX_H

#include <exception>
#include <string>

class LSystemException : public exception
{
public:
    std::string type, message;

    const char * toString() throw()
    {
        if( message.empty() )
        {
            return string( this->type + "Unspecified error" ).c_str();
        }
        else
        {
            return string( this->type + this->message ).c_str();
        }
    };

public:
    LSystemException()
    {
        type = "LSystem: ";
    };

    LSystemException( const char * description)
    {
        LSystemException();
        this->message = description;
    };

    ~LSystemException() throw() {};

    virtual const char * what() const throw()
    {
        return this->toString();
    };
};

class ParsingException : public LSystemException
{
public:
    ParsingException()
    {
        type.append( "parsing error: " );
    };

    ParsingException( const char * description)
    {
        ParsingException();
        this->message = description;
    };
};

#endif // ABSTRACTLSYSTEMEX_H
