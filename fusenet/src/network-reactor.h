#ifndef NETWORK_REACTOR_H
#define NETWORK_REACTOR_H

/**
 * @file network-reactor.h
 *
 * This file contains the network reactor interface. The network
 * reactor reacts to network events such as incoming connections,
 * received data and lost connections. It forwards these events to
 * event handlers, which in turn relay the messages to protocol
 * instances.
 *
 * This design is based upon a the Reactor pattern by
 * Schmidt. Although modified a bit, the idea is the same.
 *
 * @author David Pettersson <david@shebang.nu>
 */

namespace fusenet {

  /**
   * Reacts to network events. This class reacts to different kinds of
   * network events, and creates new handler instances when
   * necessary.
   *
   * This reactor is custom made for BSD sockets, but could be adapted
   * to any case where events are delivered synchronously and can be
   * demultiplexed with functionality similar to Unix' select() call.
   */
  class NetworkReactor {
  public:

    /**
     * Creates a new network reactor instance. Takes a socket handler
     * factory pointer as an argument, which is later used when new
     * handlers are required.
     *
     * @param portNumber the port number to react on
     * @param handlerFactory the handler factory
     *
     * @todo Add handler factory parameter
     */
    NetworkReactor(int portNumber);
  };

}

#endif

