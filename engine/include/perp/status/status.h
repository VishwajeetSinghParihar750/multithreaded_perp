#pragma once
#include <string_view>

namespace STATUS
{

    enum class StatusCode : int
    {
        kOk = 0, // no error

        kInvalidArgument = 1,    // invalid requests
        kDealineExceeded = 2,    // timeouts
        kNotFound = 3,           // generic not found
        kFailedPrecondition = 4, // some condition reqiured to perform op failed
        kAborted = 5,            // op aborted for some reason, your infra issues or whatever
        kUnknown = 6,            // unknown

    };

    class Status
    {
        StatusCode statusCode;
        std::string_view message;
        std::string_view location;

    public:
        StatusCode
        code() const
        {
            return statusCode;
        }

        bool operator==(const Status &other) const
        {
            return statusCode == other.statusCode;
        }

        bool operator!=(const Status &other) const
        {
            return statusCode != other.statusCode;
        }

        Status() : statusCode(StatusCode::kOk) {}
        Status(const StatusCode &code) : statusCode(code) {}
        Status(const StatusCode &code, const std::string_view &message) : statusCode(code), message(message) {}
        Status(const StatusCode &code, const std::string_view &message, std::string_view location) : statusCode(code), message(message), location(location) {}

        // copy and move are kept default
    };

    // here when you get status it would error only
    template <typename T>
    using StatusOr = std::expected<T, Status>;

    constexpr std::string_view StatusCodeToStringView(StatusCode code);

    bool IsAborted(const Status &status);
    bool IsDeadlineExceeded(const Status &status);
    bool IsFailedPrecondition(const Status &status);
    bool IsInvalidArgument(const Status &status);
    bool IsNotFound(const Status &status);
    bool IsUnknown(const Status &status);

} // namespace STATUS
