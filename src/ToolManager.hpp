/* 
 * File:   ToolManager.hpp
 * Author: admin
 *
 * Created on June 3, 2013, 4:37 PM
 */

#ifndef TOOLMANAGER_HPP
#define	TOOLMANAGER_HPP

#include <iosfwd>
#include <string>
#include <vector>

#include "TSTool.hpp"

class ToolManager
{
public:
    static void registerTool(const std::string& name,
                             const std::string& help,
                             TSTool*(*factory)());
        
    static bool hasTool(const std::string& name);
    
    static bool runTool(const std::string& name,
                        const std::vector<std::string> params);
    
    static void showHelp(std::ostream& out);
    
private:
    ToolManager();
    ToolManager(const ToolManager&);
    void operator=(const ToolManager&);
};

template<class T>
struct ToolManagerAutoRegister
{
    ToolManagerAutoRegister(const std::string& name, const std::string& help)
    {
        ToolManager::registerTool(name, help, &create);
    }
    
    static TSTool* create()
    {
        return static_cast<TSTool*>(new T());
    }
};

#endif	/* TOOLMANAGER_HPP */

