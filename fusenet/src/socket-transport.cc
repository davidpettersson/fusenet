/**
 * @file socket-transport.cc
 *
 * This file contains the implementation of the socket transport class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <cassert>

#include "socket-transport.h"

#define PREFIX "[SocketTransport] "

namespace fusenet {
  
  SocketTransport::SocketTransport(int d) {
    descriptor = d;
  }

  void SocketTransport::send(uint8_t data) {
    if (isClosed()) {
      return;
    }

    int n = ::send(descriptor, &data, 1, 0);

    if (n == -1) {
      close();
    } else if (n == 0) {
      close();
    } else {
      // All is well
    }

#ifdef ENABLE_DEBUG
    std::cout << PREFIX "send(" << static_cast<int>(data) << ")" << std::endl;
#endif
  }

  uint8_t SocketTransport::receive(void)  {
    if (isClosed()) {
      return 0;
    }

    uint8_t data;
    int n = recv(descriptor, &data, 1, 0);

    if (n == -1) {
      close();
    } else if (n == 0) {
      close();
    } else {
      // All is well
    }

#ifdef ENABLE_DEBUG
    std::cout << PREFIX "receive() -> " << static_cast<int>(data) << std::endl;
#endif

    return data;
  }

  int SocketTransport::getDescriptor(void) {
    return descriptor;
  }

  void SocketTransport::close(void) {
    if (descriptor != -1) {
      ::close(descriptor);
      descriptor = -1;
    }
  }

  bool SocketTransport::isClosed(void) const {
    return descriptor == -1;
  }

  SocketTransport::~SocketTransport(void) {
    if (descriptor != -1) {
      ::close(descriptor);
    }
  }
}


