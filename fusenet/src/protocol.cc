/**
 * @file
 *
 * This file contains the protocol implementation.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "protocol.h"

namespace fusenet {

  Protocol::Protocol(Transport* transport) {
    this->transport = transport;
  }

  Protocol::~Protocol(void) {
    // Does nothing
  }

}

