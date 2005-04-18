#ifndef MESSAGE_PROTOCOL_H
#define MESSAGE_PROTOCOL_H

/**
 * @file
 *
 * This file contains the message protocol interface.
 */

#include <string>

#include "fusenet-types.h"
#include "protocol.h"

namespace fusenet {

  /**
   * Base class for all message protocols. This class provides some
   * convenience methods that are useful when implementing the message
   * based protocols as defined by Holm et al.
   *
   * Creating classes that inherit this support simplifies the
   * implementation and hides the low-level bit and byte juggling of
   * the message protocol primitives.
   */
  class MessageProtocol : public Protocol {

  public:

    /**
     * Creates a message protocol instance.
     *
     * @param transport the transport
     */
    MessageProtocol(Transport* transport) : Protocol(transport) { }

    /**
     * Called on made connection.
     */
    virtual void onConnectionMade(void);

    /**
     * Send a command.
     *
     * @param commandIdentifier the command identifier to send
     */
    void sendCommand(MessageIdentifier_t commandIdentifier);

    /**
     * Send a string parameter.
     *
     * @param parameter the string parameter to send
     */
    void sendParameter(const std::string& parameter);

    /**
     * Send a number parameter.
     *
     * @param parameter the string parameter to send
     */
    void sendParameter(int parameter);

    /**
     * Receive an command.
     *
     * @return the command
     */
    MessageIdentifier_t receiveCommand(void);

    /**
     * Expect a command. This call consumes a command byte and throws
     * an exception if the actual command received is not the expected
     * one.
     */
    void expectCommand(MessageIdentifier_t expected);
    
    /**
     * Receive a string parameter.
     *
     * @param parameter the parameter.
     */
    void receiveParameter(std::string& parameter);

    /**
     * Receive a number parameter.
     *
     * @param parameter the parameter.
     */
    void receiveParameter(int* const parameter);

    /**
     * Called on lost connection.
     */
    virtual void onConnectionLost(void);

  private:

    /**
     * Unpack to a byte array.
     *
     * @param integer the integer to unpack
     * @param array the array to place it in
     */
    void unpack(uint32_t integer, uint8_t* const array);

    /**
     * Pack to a byte array.
     *
     * @param array the array to pack
     * @param integer the packed integer
     */
    void pack(const uint8_t* const array, size_t* const integer);
  };

}

#endif

