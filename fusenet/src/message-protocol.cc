/**
 * @file message-protocol.cc
 *
 * This file contains the implementation of the message protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "message-protocol.h"

namespace fusenet {

  void MessageProtocol::onConnectionMade(void) {
    // Ignore for now
  }

  void MessageProtocol::sendCommand(MessageIdentifier_t commandIdentifier) {
    transport->send(commandIdentifier);
  }

  void MessageProtocol::sendParameter(const std::string& parameter) {
    uint8_t number[4];
    size_t i;

    unpack(parameter.length(), number);
    transport->send(PAR_STRING);

    for (i = 0; i < 4; i++) {
      transport->send(number[i]);
    }

    for (i = 0; i < parameter.length(); i++) {
      transport->send(parameter[i]);
    }
  }

  void MessageProtocol::sendParameter(int parameter) {
    uint8_t number[4];
    size_t i;

    unpack(parameter, number);
    transport->send(PAR_NUM);

    for (i = 0; i < 4; i++) {
      transport->send(number[i]);
    }
  }

  void MessageProtocol::onConnectionLost(void) {
    // Ignore for now
  }

  void MessageProtocol::onDataReceived(uint8_t data) {
    (void) data;

    // Ignore for now
  }

  void MessageProtocol::unpack(uint32_t integer, 
			       uint8_t* const array) {
    array[0] = (integer >> 24) & 0xff;
    array[1] = (integer >> 16) & 0xff;
    array[2] = (integer >>  8) & 0xff;
    array[3] = (integer >>  0) & 0xff;
  }

  void MessageProtocol::pack(const uint8_t* const array, 
			     uint32_t* const integer) {
    *integer  = (array[0] << 24);
    *integer |= (array[1] << 16);
    *integer |= (array[2] <<  8);
    *integer |= (array[3] <<  0);
  }
}

