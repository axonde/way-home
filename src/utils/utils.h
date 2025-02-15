#pragma once
#include <cstring>
#include <iostream>

/// @name Proceeed<...> ask the user yes / no answer
/// @name Hint<...> inform the user
/// @name Error<...> print the happend error
namespace Warnings {
    bool ProceedAlreadyCreatedConfig();

    void HintCorrectnessCityNaming();
    void HintFirstCreateConfigFile();

    void ErrorWhileParsingConfig();
    void ErrorAbnormal();
}

namespace Errors {
    struct CantOpenFile {};
}