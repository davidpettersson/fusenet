/**
 * @file main.cc
 *
 * This file contains the application entry point. Just having one
 * main.cc simplifies the build process, since we don't have to mess
 * about with several different make targets.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "network-reactor.h"
#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  class EchoServer : public Protocol {
  public:
    EchoServer(const Transport* transport) : Protocol(transport) {
      std::cout << "Created!" << std::endl;
    }

    void onDataReceived(uint8_t data) {
      std::cout << "Got some data!" << std::endl;
      transport->send(data);
    }

    void onConnectionLost(void) {
    }

    ~EchoServer(void) {
      std::cout << "Destroyed!" << std::endl;
    }
  };

  class EchoServerCreator : public ProtocolCreator {
  public:
    Protocol* create(const Transport* transport) const {
      return new EchoServer(transport);
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

static void serverBehaviour(void) {
  fusenet::EchoServerCreator creator;
  fusenet::NetworkReactor networkReactor;
  networkReactor.serve(4000, &creator);
}

static void clientBehaviour(void) {

}

int main(int argc, char* argv[]) {
  (void) argc;
  (void) argv;

  serverBehaviour();
  return 0;
}

