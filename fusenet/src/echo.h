/**
 * @file echo.h
 *
 * This file contains the echo demo. This is a bit ugly, but on the
 * other hand it is nothing more than a demo.
 *
 * @author David Pettersson <david@shebang.nu>
 */

namespace fusenet {

  class EchoServer : public Protocol {
  public:
    EchoServer(const Transport* transport) : Protocol(transport) {
      std::cout << "[" << this << "] Created" << std::endl;
    }

    void onDataReceived(uint8_t data) {
      std::cout << "[" << this << "] Echoing '" << data << "'" << std::endl;
      transport->send(data);
    }

    void onConnectionLost(void) {
      std::cout << "[" << this << "] Connection lost" << std::endl;
    }

    void onConnectionMade(void) {
      std::cout << "[" << this << "] Connection made" << std::endl;
    }

    ~EchoServer(void) {
      std::cout << "[" << this << "] Destroyed" << std::endl;
    }
  };

  class EchoServerCreator : public ProtocolCreator {
  public:
    Protocol* create(const Transport* transport) const {
      return new EchoServer(transport);
    }
  };

  class EchoClient : public Protocol {
  public:
    EchoClient(const Transport* transport) : Protocol(transport) {
      std::cout << "Created!" << std::endl;
    }

    void onDataReceived(uint8_t data) {
      std::cout << "Server said: " << data << std::endl;
      std::cout << "You say: ";
      std::cin >> data;
      transport->send(data);
    }

    void onConnectionLost(void) {
    }

    void onConnectionMade(void) {
      uint8_t data;
      std::cout << "You say: ";
      std::cin >> data;
      transport->send(data);
    }

    ~EchoClient(void) {
      std::cout << "Destroyed!" << std::endl;
    }
  };

  class EchoClientCreator : public ProtocolCreator {
  public:
    Protocol* create(const Transport* transport) const {
      return new EchoClient(transport);
    }
  };
}

