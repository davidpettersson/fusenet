/**
 * @file client-protocol-creator.h
 *
 * This file contains the implementation of the client protocol
 * creator class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "client-protocol-creator.h"
#include "client-protocol.h"

namespace fusenet {

  Protocol* ClientProtocolCreator::create(Transport* const transport) const {
    return new ClientProtocol(transport);
  }

}

