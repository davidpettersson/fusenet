/**
 * @file server-protocol-creator.h
 *
 * This file contains the implementation of the server protocol
 * creator class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "server-protocol-creator.h"
#include "server-protocol.h"

namespace fusenet {

  Protocol* ServerProtocolCreator::create(Transport* const transport) const {
    return new ServerProtocol(transport);
  }

}

