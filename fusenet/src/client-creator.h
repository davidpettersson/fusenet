#ifndef CLIENT_CREATOR_H
#define CLIENT_CREATOR_H

/**
 * @file
 *
 * This file contains the client creator interface.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  /**
   * Class for creating instances of the client.
   */
  class ClientCreator : public ProtocolCreator {

  public:

    /**
     * Creates instances of client protocols.
     *
     * @param transport the transport to give the protocol
     */
    Protocol* create(Transport* const transport) const;
  };
}

#endif

