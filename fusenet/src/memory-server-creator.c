/**
 * @file memory-server-creator.h
 *
 * This file contains the implementation of the memory server creator
 * class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "memory-server-creator.h"
#include "memory-server.h"

namespace fusenet {

  Protocol* MemoryServerCreator::create(Transport* const transport) const {
    return new MemoryServer(transport);
  }

}

