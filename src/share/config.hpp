#pragma once

#include <string>
#include <vector>

class Config
{
private:
    constexpr static const char* const CONFIG_FILENAME = "config";
    
    constexpr static const char* const SYS_INCLUDE_PATHNAME_0 = "sys_include_pathname_0";
    constexpr static const char* const SYS_INCLUDE_PATHNAME_1 = "sys_include_pathname_1";
    constexpr static const char* const SYS_INCLUDE_PATHNAME_2 = "sys_include_pathname_2";
    constexpr static const char* const SYS_INCLUDE_PATHNAME_3 = "sys_include_pathname_3";
    constexpr static const char* const SYS_INCLUDE_PATHNAME_4 = "sys_include_pathname_4";

    constexpr static const char* const GEN_RESULT_PATHNAME = "gen_result_pathname";
    
    constexpr static const char* const PP_RESULT_FILENAME = "pp_result_filename";
    constexpr static const char* const PP_IS_REPLACED_TRIGRAPH = "pp_replace_trigraph";
    constexpr static const char* const PP_IS_IGNORED_SPACE = "pp_ignore_space";

public:
    Config();

    std::vector<std::string> sys_include_pathname_vec;

    std::string gen_result_pathname;
    
    std::string pp_result_filename;
    bool pp_is_replaced_trigraph;
    bool pp_is_ignored_space;

private:
    void load();
    void outputCoution(const char* const attr,
                       const char* const value) const;
};

const Config& config();