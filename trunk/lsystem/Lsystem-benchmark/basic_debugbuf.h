#pragma once

template <class CharT, class TraitsT = std::char_traits<CharT> >
class basic_debugbuf : 
    public std::basic_stringbuf<CharT, TraitsT>
{
public:

    virtual ~basic_debugbuf()
    {
        sync();
    }

protected:

    int sync()
    {
        output_debug_string(str().c_str());
        str(std::basic_string<CharT>());    // Clear the string buffer


        return 0;
    }

    void output_debug_string(const CharT *text) {}
};
