/* 
 * File:   TSTool.hpp
 * Author: admin
 *
 * Created on June 3, 2013, 4:16 PM
 */

#ifndef TSTOOL_HPP
#define	TSTOOL_HPP

#include <string>
#include <vector>

class TSTool
{
public:
    virtual ~TSTool(){}
    
    virtual void run(const std::vector<std::string>& params) = 0;
};

#endif	/* TSTOOL_HPP */

