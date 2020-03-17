#include "pp_directive_processor.hpp"
#include "preprocessor.hpp"
#include "macro.hpp"
#include <iostream>

PPDirectiveProcessor::PPDirectiveProcessor(Preprocessor* pp):
    mPP(pp),
    mIsValid(true)
{
}

bool PPDirectiveProcessor::execute()
{
    process(mPP->mPreprocessingFile);

    return mIsValid;
}

void PPDirectiveProcessor::process(PreprocessingFile* preprocessingFile)
{
    if(preprocessingFile->group)
        process(preprocessingFile->group);
}