/**
 * @file transport.cc
 *
 * This file contains the mostly empty implementation of the transport class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "transport.h"

namespace fusenet {

  Transport::Transport(void) {
    transportName = "<unknown>";
  }

  Transport::Transport(std::string& name) {
    transportName = name;
  }

  std::string& Transport::getName(void) {
    return transportName;
  }

  Transport::~Transport(void) {
    // Does nothing
  }
}
