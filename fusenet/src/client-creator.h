#ifndef CLIENT_CREATOR_H
#define CLIENT_CREATOR_H

/**
 * @file client-creator.h
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
   * Class for creating instances of the client protocol.
   */
  class ClientCreator : public ProtocolCreator {

  public:
    /**
     * Creates instances of client protocols.
     */
    Protocol* create(Transport* const transport) const;
  };
}

#endif

