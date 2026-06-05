#include "status.h"
#include <string_view>

namespace STATUS
{

    constexpr std::string_view StatusCodeToStringView(StatusCode code)
    {
        switch (code)
        {
        case StatusCode::kOk:
            return "OK";
        case StatusCode::kAborted:
            return "ABORTED";
        case StatusCode::kDealineExceeded:
            return "DEADLINE_EXCEEDED";
        case StatusCode::kFailedPrecondition:
            return "FAILED_PRECONDITION";
        case StatusCode::kNotFound:
            return "NOT_FOUND";
        case StatusCode::kInvalidArgument:
            return "INVALID_REQUEST";

        default:
            return "";
        }
    }

    bool IsAborted(const Status &status)
    {
        return status.code() == StatusCode::kAborted;
    }

    bool IsDeadlineExceeded(const Status &status)
    {
        return status.code() == StatusCode::kDealineExceeded;
    }

    bool IsFailedPrecondition(const Status &status)
    {
        return status.code() == StatusCode::kFailedPrecondition;
    }

    bool IsInvalidArgument(const Status &status)
    {
        return status.code() == StatusCode::kInvalidArgument;
    }

    bool IsNotFound(const Status &status)
    {
        return status.code() == StatusCode::kNotFound;
    }

    bool IsUnknown(const Status &status)
    {
        return status.code() == StatusCode::kUnknown;
    }
} // namespace STATUS