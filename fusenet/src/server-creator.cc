/**
 * @file server-creator.h
 *
 * This file contains the implementation of the server creator
 * class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "server-creator.h"
#include "server.h"

namespace fusenet {
  
  ServerCreator::ServerCreator(Database* database) {
    this->database = database;
  }

  Protocol* ServerCreator::create(Transport* const transport) const {
    return new Server(transport);
  }

}

