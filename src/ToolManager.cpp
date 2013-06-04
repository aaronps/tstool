#include "ToolManager.hpp"
#include <iostream>

#include <map>

using std::map;
using std::string;
using std::vector;

struct ToolData
{
    string name;
    string help;
    TSTool * (*factory)();
    
    ToolData(const string& name, const string& help, TSTool*(*factory)())
        : name(name), help(help), factory(factory)
    {}
};

typedef map<string, ToolData> ToolMap;

static ToolMap * toolMap = 0;

static void ensureMap()
{
    if ( ! toolMap )
    {
        toolMap = new ToolMap();
    }
}

void ToolManager::registerTool( const std::string& name,
                                const std::string& help,
                                TSTool* (*factory)() )
{
    ensureMap();
    toolMap->insert(std::make_pair(name, ToolData(name, help, factory)));
}

bool ToolManager::hasTool(const std::string& name)
{
    ensureMap();
    ToolMap::iterator i = toolMap->find(name);
    return i != toolMap->end();
}

void ToolManager::showHelp(std::ostream& out)
{
    ensureMap();
    if ( toolMap->empty() )
    {
        out << "No tools defined\n";
    }
    else
    {
        ToolMap::iterator i = toolMap->begin();
        while ( i != toolMap->end() )
        {
            out << i->second.name << ":\n\t"
                << i->second.help << "\n";
            i++;
        }
    }
}

bool ToolManager::runTool(const std::string& name,
                          const std::vector<std::string> params)
{
    ensureMap();
    
    ToolMap::iterator i = toolMap->find(name);
    if ( i != toolMap->end() )
    {
        TSTool * tool = i->second.factory();
        
        tool->run(params);
        
        delete tool;
        return true;
    }
    
    return false;
}
