#ifndef MEMORY_SERVER_CREATOR_H
#define MEMORY_SERVER_CREATOR_H

/**
 * @file memory-server-creator.h
 *
 * This file contains the memory server creator interface.
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
  class MemoryServerCreator : public ProtocolCreator {

  public:
    /**
     * Creates instances of server protocols.
     */
    Protocol* create(Transport* const transport) const;
  };
}

#endif

