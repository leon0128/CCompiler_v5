#include "config.hpp"
#include "file_manager.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

const Config conf;
const Config& config(){return conf;}

Config::Config():
    sys_include_pathname("/usr/include"),
    gen_result_pathname("data"),
    pp_result_filename("result.pp"),
    pp_is_replaced_trigraph(true),
    pp_is_ignored_space(true)
{
    load();
}

void Config::load()
{
    std::string stream;
    if(!FileManager::read(CONFIG_FILENAME, stream))
    {
        std::cerr << "coution: cannot open the config file. "
                     "thus use default value.";
        return;
    }

    std::vector<std::string> lines;
    std::string::size_type begPos = 0;
    for(std::string::size_type pos = 0;
        pos < stream.size();
        pos++)
    {
        if(stream.at(pos) == ' ')
        {
            stream.replace(pos, 1, "");
            pos--;
        }
        else if(stream.at(pos) == '\n')
        {
            lines.emplace_back(stream.substr(begPos, pos - begPos));
            begPos = pos + 1;
        }
        else if(pos == stream.size() - 1)
            lines.emplace_back(stream.substr(begPos, stream.size() - begPos));
    }

    std::unordered_map<std::string, std::string> attributeMap;
    for(auto&& str : lines)
    {
        // delete after hash(#)
        auto hashPos = str.find('#');
        if(hashPos != std::string::npos)
            str = str.substr(0, hashPos);

        auto equalPos = str.find('=');
        if(equalPos != std::string::npos)
            attributeMap.emplace(str.substr(0, equalPos),
                                 str.substr(equalPos + 1, str.size()));
    }
    
    bool value = true;
    auto isValid = [&value](const std::string& node)
    {
        if(node == "true" ||
           node == "t" ||
           node == "yes" ||
           node == "1")
            value = true;
        else if(node == "false" ||
                node == "f" ||
                node == "no" ||
                node == "0")
            value = false;
        else
            return false;

        return true;
    };

    // sys_include_pathname
    auto iter = attributeMap.find(SYS_INCLUDE_PATHNAME);
    if(iter != attributeMap.end())
        sys_include_pathname = iter->second;
    else
        outputCoution(SYS_INCLUDE_PATHNAME,
                      sys_include_pathname.c_str());
    // gen_result_pathname
    iter = attributeMap.find(GEN_RESULT_PATHNAME);
    if(iter != attributeMap.end())
        gen_result_pathname = iter->second;
    else
        outputCoution(GEN_RESULT_PATHNAME,
                      gen_result_pathname.c_str());
    // pp_result_filename
    iter = attributeMap.find(PP_RESULT_FILENAME);
    if(iter != attributeMap.end())
        pp_result_filename = iter->second;
    else
        outputCoution(PP_RESULT_FILENAME,
                      pp_result_filename.c_str());
    // pp_is_replaced_trigraph
    iter = attributeMap.find(PP_IS_REPLACED_TRIGRAPH);
    if(iter != attributeMap.end())
    {
        if(isValid(iter->second))
            pp_is_replaced_trigraph = value;
        else
            outputCoution(PP_IS_REPLACED_TRIGRAPH,
                          pp_is_replaced_trigraph ? "true" : "false");
    }
    else
        outputCoution(PP_IS_REPLACED_TRIGRAPH,
                      pp_is_replaced_trigraph ? "true" : "false");
    // pp_is_ignored_space
    iter = attributeMap.find(PP_IS_IGNORED_SPACE);
    if(iter != attributeMap.end())
    {
        if(isValid(iter->second))
            pp_is_ignored_space = value;
        else
            outputCoution(PP_IS_IGNORED_SPACE,
                          pp_is_ignored_space ? "true" : "false");
    }
    else
        outputCoution(PP_IS_IGNORED_SPACE,
                      pp_is_ignored_space ? "true" : "false");
}

void Config::outputCoution(const char* const attr,
                           const char* const value) const
{
    std::cerr << "caution: \""
              << attr
              << "\" does not exist. Use defalut value (\""
              << value
              << "\")."
              << std::endl;
}