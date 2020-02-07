#pragma once

#include <string>

class Config
{
private:
    constexpr static const char* const CONFIG_FILENAME = "config";

    constexpr static const char* const GEN_RESULT_PATHNAME = "gen_result_pathname";
    
    constexpr static const char* const PP_RESULT_FILENAME = "pp_result_filename";
    constexpr static const char* const PP_IS_REPLACED_TRIGRAPH = "pp_replace_trigraph";
    constexpr static const char* const PP_IS_IGNORED_SPACE = "pp_ignore_space";

public:
    Config();

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