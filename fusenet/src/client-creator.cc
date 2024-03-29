/**
 * @file
 *
 * This file contains the client creator implementation.
 */

#include "client-creator.h"
#include "client.h"

namespace fusenet {

  Protocol* ClientCreator::create(Transport* const transport) const {
    return new Client(transport);
  }

}

