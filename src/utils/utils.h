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
    void HintNotFoundWays();

    void ErrorWhileParsingConfig();
    void ErrorAbnormal();
    void ErrorBadConfig();
    void ErrorFewArgs();
    void ErrorWhileFindRoutes();
    void ErrorIllJsonAnswer();
}

namespace Errors {
    struct Error {
        virtual std::string what() const = 0;
        virtual ~Error() = default;
    };

    struct CantOpenFile : public Error {
        std::string what() const override;
    };
    struct BadJson : public Error {
        std::string what() const override;
    };
    struct ErrorParseJson : public Error {
        std::string what() const override;
    };
    struct ErrorInvalidDate : public Error {
        std::string what() const override;
    };
    struct ErrorRequestBadStatus : public Error {
        ErrorRequestBadStatus(long status_code = 0) : status_code_(status_code) {}
        std::string what() const override;
        long status_code_;
    };
    struct ErrorApiCode : public Error {
        std::string what() const override;
    };
    struct ErrorApiCityNotFound : public Error {
        ErrorApiCityNotFound(const std::string& city) : city_(city) {}
        std::string what() const override;
        std::string city_;
    };
}