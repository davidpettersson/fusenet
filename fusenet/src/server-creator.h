#ifndef SERVER_CREATOR_H
#define SERVER_CREATOR_H

/**
 * @file server-creator.h
 *
 * This file contains the server creator interface.
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
  class ServerCreator : public ProtocolCreator {

  public:
    /**
     * Creates instances of server protocols.
     */
    Protocol* create(Transport* const transport) const;
  };
}

#endif

