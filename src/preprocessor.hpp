#pragma once

#include <string>
#include <vector>
#include <utility>

class Compiler;
class Initializer;
class PPTokenizer;
class PPDirectiveTokenizer;
class PPDirectiveProcessor;
class PPCharacterConverter;

class Preprocessor
{
public:
    friend Compiler;
    friend Initializer;
    friend PPTokenizer;
    friend PPDirectiveTokenizer;
    friend PPDirectiveProcessor;
    friend PPCharacterConverter;

    enum ESearch
    {
        CURRENT_ONLY,
        CURRENT_AND_SYSTEM,
        SYSTEM_ONLY
    };

    Preprocessor();
    ~Preprocessor();

    bool execute(const std::string& file,
                 const std::string& dir,
                 ESearch eSearch = CURRENT_ONLY);
    bool execute(std::string&& file,
                 std::string&& dir = std::string(),
                 ESearch eSearch = CURRENT_ONLY);

    bool retokenize(std::string& src);

    bool characterConvert();

    const std::vector<class PreprocessingToken*>& getPPTokens() const {return mProcessedPPTokens;}

    static void includePredefinedMacro();

private:
    void initialize();
    void ppTokenize();
    void ppDirectiveTokenize();
    void ppDirectiveProcess();
    void ppCharacterConvert();

    void error(const char* message) const;

    static std::vector<class ControlLine*> MACROS; 

    Initializer* mInitializer;
    PPTokenizer* mPPTokenizer; 
    PPDirectiveTokenizer* mPPDirectiveTokenizer;
    PPDirectiveProcessor* mPPDirectiveProcessor;
    PPCharacterConverter* mPPCharacterConverter;

    std::string mFile;
    std::string mDir;
    std::string mSrc;
    ESearch mESearch;

    std::vector<std::pair<class PreprocessingToken*, std::string::size_type>> mPPTokens;

    class PreprocessingFile* mPreprocessingFile;

    std::vector<class PreprocessingToken*> mProcessedPPTokens;

    bool mIsValid;
};