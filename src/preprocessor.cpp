#include "preprocessor.hpp"
#include "pp_initializer.hpp"
#include "pp_tokenizer.hpp"
#include "pp_directive_tokenizer.hpp"
#include "pp_directive_processor.hpp"
#include "pp_character_converter.hpp"
#include <iostream>
#include <utility>

std::vector<class ControlLine*> Preprocessor::MACROS;

void Preprocessor::includePredefinedMacro()
{
    Preprocessor p;
    p.execute("predefined_macro.h");
}

Preprocessor::Preprocessor():
    mPPInitializer(nullptr),
    mPPTokenizer(nullptr),
    mPPDirectiveTokenizer(nullptr),
    mPPDirectiveProcessor(nullptr),
    mPPCharacterConverter(nullptr),
    mFile(),
    mDir(),
    mSrc(),
    mESearch(CURRENT_ONLY),
    mPPTokens(),
    mPreprocessingFile(nullptr),
    mProcessedPPTokens(),
    mIsValid(true)
{
    mPPInitializer = new PPInitializer(this);
    mPPTokenizer = new PPTokenizer(this);
    mPPDirectiveTokenizer = new PPDirectiveTokenizer(this);
    mPPDirectiveProcessor = new PPDirectiveProcessor(this);
    mPPCharacterConverter = new PPCharacterConverter(this);
}

Preprocessor::~Preprocessor()
{
    delete mPPInitializer;
    delete mPPTokenizer;
    delete mPPDirectiveTokenizer;
    delete mPPDirectiveProcessor;
    delete mPPCharacterConverter;
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

bool Preprocessor::characterConvert()
{
    ppCharacterConvert();

    return mIsValid;
}

void Preprocessor::initialize()
{
    mIsValid = mPPInitializer->execute();

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

void Preprocessor::ppCharacterConvert()
{
    mIsValid = mPPCharacterConverter->execute();

    if(!mIsValid)
        error("failed to convert from character escape sequence to character.");
}

void Preprocessor::error(const char* message) const
{
    std::cerr << "Preprocessor error:\n"
              << "    what: " << message
              << "\n    file: " << mDir << mFile
              << std::endl;
}