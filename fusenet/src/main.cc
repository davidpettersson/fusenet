/**
 * @file main.cc
 *
 * This file contains the application entry point. Just having one
 * main.cc simplifies the build process, since we don't have to mess
 * about with several different make targets.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "protocol.h"
#include "transport.h"

namespace fusenet {

  class EchoServer : public Protocol {
  public:
    EchoServer(Transport* transport) : Protocol(transport) {
    }

    void onDataReceived(uint8_t* data, size_t size) {
      std::cout << "Got some data!" << std::endl;
      transport->send(data, size);
    }

    void onConnectionLost(void) {
    }
  };

  class TerminalTransport : public Transport {
  public:
    void send(uint8_t* data, size_t size) const {
      size_t i;
      for (i = 0; i < size; i++) {
	std::cout << data[i] << std::endl;
      }
    }
  };
}

int main(int argc, char* argv[]) {
  uint8_t* data;

  data = new uint8_t[5];
  memcpy(data, "hello", 5);

  fusenet::TerminalTransport terminalTransport;
  fusenet::EchoServer echoServer(&terminalTransport);
  echoServer.onDataReceived(data, 5);

  return 0;
}

