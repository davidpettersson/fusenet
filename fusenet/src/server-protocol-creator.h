#ifndef SERVER_PROTOCOL_CREATOR_H
#define SERVER_PROTOCOL_CREATOR_H

/**
 * @file server-protocol-creator.h
 *
 * This file contains the server protocol creator interface.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  /**
   * Class for creating instances of the server protocol.
   */
  class ServerProtocolCreator : public ProtocolCreator {

  public:
    /**
     * Creates instances of server protocols.
     */
    Protocol* create(const Transport* transport) const;
  };
}

#endif

