#include "preprocessor.hpp"
#include "initializer.hpp"
#include "pp_tokenizer.hpp"
#include "pp_directive_tokenizer.hpp"
#include "pp_directive_processor.hpp"
#include <iostream>
#include <utility>

std::vector<class ControlLine*> Preprocessor::MACROS;

void Preprocessor::includePredefinedMacro()
{
    Preprocessor p;
    p.execute("predefined_macro.h");
}

Preprocessor::Preprocessor():
    mInitializer(nullptr),
    mPPTokenizer(nullptr),
    mPPDirectiveTokenizer(nullptr),
    mPPDirectiveProcessor(nullptr),
    mFile(),
    mDir(),
    mSrc(),
    mESearch(CURRENT_ONLY),
    mPPTokens(),
    mPreprocessingFile(nullptr),
    mProcessedPPTokens(),
    mIsValid(true)
{
    mInitializer = new Initializer(this);
    mPPTokenizer = new PPTokenizer(this);
    mPPDirectiveTokenizer = new PPDirectiveTokenizer(this);
    mPPDirectiveProcessor = new PPDirectiveProcessor(this);
}

Preprocessor::~Preprocessor()
{
    delete mInitializer;
    delete mPPTokenizer;
    delete mPPDirectiveTokenizer;
    delete mPPDirectiveProcessor;
}

bool Preprocessor::execute(const std::string& file,
                           const std::string& dir,
                           ESearch eSearch)
{
    mFile = file,
    mDir = dir;
    mESearch = eSearch;

    initialize();

    if(mIsValid)
        ppTokenize();
    
    if(mIsValid)
        ppDirectiveTokenize();

    if(mIsValid)
        ppDirectiveProcess();

    return mIsValid;
}

bool Preprocessor::execute(std::string&& file,
                           std::string&& dir,
                           ESearch eSearch)
{
    std::string f(std::forward<std::string>(file));
    std::string d(std::forward<std::string>(dir));

    return execute(f, d, eSearch);
}

bool Preprocessor::retokenize(std::string& src)
{
    mSrc = src;
    
    ppTokenize();
    
    return mIsValid;
}

void Preprocessor::initialize()
{
    mIsValid = mInitializer->execute();

    if(!mIsValid)
        error("failed to initialize source code.");
}

void Preprocessor::ppTokenize()
{
    mIsValid = mPPTokenizer->execute();

    if(!mIsValid)
        error("failed to tokenize source code.");
}

void Preprocessor::ppDirectiveTokenize()
{
    mIsValid = mPPDirectiveTokenizer->execute();
    
    if(!mIsValid)
        error("failed to tokenize preprocessing token.");
}

void Preprocessor::ppDirectiveProcess()
{
    mIsValid = mPPDirectiveProcessor->execute();

    if(!mIsValid)
        error("failed to process preprocessing directive.");
}

void Preprocessor::error(const char* message) const
{
    std::cerr << "preprocessor-class-error:\n    "
              << message
              << std::endl;
}