/**
 * @file protocol.cc
 *
 * This file contains the implementation of the base protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "protocol.h"

namespace fusenet {

  Protocol::Protocol(const Transport* transport) {
    this->transport = transport;
  }

}

