#pragma once

#include "interface.hpp"

#include <vector>

namespace netfin::core::network {
  class InterfaceFinder {
    public: 
      explicit InterfaceFinder() = default;
      ~InterfaceFinder() = default;

      std::vector<Interface> find() const;
  };
};