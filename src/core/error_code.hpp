#pragma once

namespace netfin::core {
  enum ErrorCode {
    Success                  = 0,
    PlatformNotSupported     = 1,
    NoNetworkInterfacesFound = 2,
    CommandNotFound          = 3,
    InvalidOption            = 4,
    UnknownError             = 9999,
  };
}