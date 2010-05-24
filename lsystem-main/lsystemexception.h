#pragma once

#ifndef ABSTRACTLSYSTEMEX_H
#define ABSTRACTLSYSTEMEX_H

#include <exception>
#include <string>
#include <iostream>/*temporarily*/

class LSystemException : public exception
{
public:
    std::string type, message;

public:
    LSystemException()
    {
        appendType();
    };

    LSystemException( const char * description)
    {
        appendType();
        this->message.append(description);
    };

    ~LSystemException() throw() {};

    virtual const char * what() const throw()
    {  
        return this->message.c_str();
    };

    virtual void appendType()
    {
        message = "LSystem: ";
    };
};

class ParsingException : public LSystemException
{
public:
    ParsingException()
    {
        appendType();
    };

    ParsingException( const char * description )
    {
        appendType();
        this->message.append(description);
    };

    ParsingException( std::string description )
    {
        appendType();
        this->message.append(description);
    };

    virtual void appendType()
    {
        this->message.append( "parsing error: " );
    }
};

class FileException : public LSystemException
{
public:
    FileException()
    {
        appendType();
    };

    FileException( const char * description)
    {
        appendType();
        this->message.append(description);
    };

    FileException( std::string description)
    {
        appendType();
        this->message.append(description);
    };

    virtual void appendType()
    {
        type.append( "file i/o error: " );
    }
};

#endif // ABSTRACTLSYSTEMEX_H
