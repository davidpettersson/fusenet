/**
 * @file main.cc
 *
 * This file contains the application entry point. Just having one
 * main.cc simplifies the build process, since we don't have to mess
 * about with several different make targets.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "client-protocol.h"
#include "client-protocol-creator.h"
#include "network-reactor.h"
#include "protocol.h"
#include "transport.h"

#ifdef ENABLE_ECHO
#include "echo.h"
#endif

namespace fusenet {

  class TerminalTransport : public Transport {
  public:
    void send(uint8_t data) const {
      printf("%02x ", data);
    }
    ~TerminalTransport(void) {
      printf("\n");
    }
  };
}

static void serverBehaviour(void) {
#ifdef ENABLE_ECHO
  fusenet::NetworkReactor networkReactor;
  fusenet::EchoServerCreator creator;
  networkReactor.serve(4000, &creator);
#else
  // WE NEED A SERVER! :-)
#endif

}

static void clientBehaviour(void) {
  fusenet::NetworkReactor networkReactor;

#ifdef ENABLE_ECHO
  fusenet::EchoClientCreator creator;
#else
  fusenet::ClientProtocolCreator creator;
#endif

  networkReactor.initiate("localhost", 4000, &creator);
}

static void printUsage(void) {
  std::cerr << "usage: fusenet [--client|--server]" << std::endl;
}

int main(int argc, char* argv[]) {

  /*
   * This argument handling is ugly, fix it sometime. :-)
   */

  if (argc != 2) {
    printUsage();
  } else if (strcmp(argv[1], "--client") == 0) {
    clientBehaviour();
  } else if (strcmp(argv[1], "--server") == 0) {
    serverBehaviour();
  } else {
    printUsage();
  }

  return 0;
}

