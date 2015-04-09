// Copyright Ralf W. Grosse-Kunstleve 2002-2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef EXTENDING_H_
#define EXTENDING_H_

#include <iostream>
#include <string>

namespace { // Avoid cluttering the global namespace.

  // A friendly class.
  class hello
  {
    public:
      hello(const std::string& country);
      std::string greet() const;
    private:
      std::string country;
  };
}

#endif