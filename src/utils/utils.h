#pragma once
#include <cstring>
#include <iostream>
#include <string>

/// @name Proceeed<...> ask the user yes / no answer
/// @name Hint<...> inform the user
/// @name Error<...> print the happend error
namespace Warnings {
    bool ProceedAlreadyCreatedConfig();

    void HintCorrectnessCityNaming();
    void HintFirstCreateConfigFile();

    void ErrorWhileParsingConfig();
    void ErrorAbnormal();
    void ErrorBadConfig();
    void ErrorFewArgs();
}

namespace Errors {
    struct Error {
        virtual std::string what() const = 0;
        virtual ~Error() = default;
    };

    struct CantOpenFile : public Error {
        virtual std::string what() const override;
    };
    struct BadJson : public Error {
        virtual std::string what() const override;
    };
    struct ErrorParseJson : public Error {
        virtual std::string what() const override;
    };
    struct ErrorInvalidDate : public Error {
        virtual std::string what() const override;
    };
}