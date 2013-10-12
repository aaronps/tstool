/* 
 * File:   EndianStream.hpp
 * Author: admin
 *
 * Created on June 4, 2013, 2:57 PM
 */

#ifndef ENDIANSTREAM_HPP
#define	ENDIANSTREAM_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <stdint.h>

class iEndianStream
{
public:
    iEndianStream(std::istream& is) : is(is) {}
    
    uint16_t read16Lite()
    {
        uint8_t buf[2];
        is.read(reinterpret_cast<char*>(buf), 2);
        return buf[0] | (buf[1] << 8);
    }
    
    std::string readFixString(unsigned len)
    {
        char buf[len+1];
        memset(buf, 0, len+1);
        is.read(buf, len);
        return std::string(buf);
    }
    
private:
    iEndianStream(const iEndianStream&);
    void operator=(const iEndianStream&);
    std::istream& is;
};

class oEndianStream
{
public:
    oEndianStream(std::ostream& os) : os(os) {}
    
    void write16Lite(uint16_t v)
    {
        uint8_t buf[2];
        buf[0] = v & 0xff;
        buf[1] = v >> 8;
        os.write(reinterpret_cast<char*>(buf), 2);
    }
    
    void writeFixString(const std::string& s, unsigned len)
    {
        char buf[len+1];
        memset(buf, 0, len+1);
        s.copy(reinterpret_cast<char*>(buf), len, 0);
        os.write(reinterpret_cast<char*>(buf), len);
    }
    
private:
    oEndianStream(const oEndianStream&);
    void operator=(const oEndianStream&);
    std::ostream& os;
};


#endif	/* ENDIANSTREAM_HPP */

