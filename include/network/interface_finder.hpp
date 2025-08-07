#pragma once

#include <vector>
#include "interface.hpp"

namespace netfin::network {
  class InterfaceFinder {
    public: 
      explicit InterfaceFinder() = default;
      ~InterfaceFinder() = default;

      std::vector<Interface> find() const;
  };
};