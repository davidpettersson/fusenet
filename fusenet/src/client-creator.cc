/**
 * @file client-creator.h
 *
 * This file contains the implementation of the client creator class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "client-creator.h"
#include "client.h"

namespace fusenet {

  Protocol* ClientCreator::create(Transport* const transport) const {
    return new Client(transport);
  }

}

