#ifndef CLIENT_PROTOCOL_CREATOR_H
#define CLIENT_PROTOCOL_CREATOR_H

/**
 * @file client-protocol-creator.h
 *
 * This file contains the client protocol creator interface.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  /**
   * Class for creating instances of the client protocol.
   */
  class ClientProtocolCreator : public ProtocolCreator {

  public:
    /**
     * Creates instances of client protocols.
     */
    Protocol* create(const Transport* transport) const;
  };
}

#endif

