/**
 * @file memory-server.cc
 *
 * This file contains the implementation of the memory server class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>
#include <string>

#include "memory-server.h"

namespace fusenet {

  void MemoryServer::onListNewsgroups(void) {
    sendCommand(ANS_LIST_NG);
    sendParameter(2);
    sendParameter(1);
    sendParameter("foo");
    sendParameter(2);
    sendParameter("bar");
    sendCommand(ANS_END);
  }

  void MemoryServer::onCreateNewsgroup(std::string& newsgroupName) {
    sendCommand(ANS_CREATE_NG);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void MemoryServer::onDeleteNewsgroup(int newsgroupIdentifier) {
    sendCommand(ANS_DELETE_NG);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void MemoryServer::onListArticles(int newsgroupIdentifier) {
    sendCommand(ANS_LIST_ART);

    if (newsgroupIdentifier == 1) {
      sendCommand(ANS_ACK);
      sendParameter(2);
      sendParameter(1);
      sendParameter("haXX0r");
      sendParameter(2);
      sendParameter("eller ngt");
    } else {
      sendCommand(ANS_NAK);
      sendCommand(ERR_NG_DOES_NOT_EXIST);
    }

    sendCommand(ANS_END);
  }

  void MemoryServer::onCreateArticle(int newsgroupIdentifier,
				       Article_t& article) {
    sendCommand(ANS_CREATE_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }
  
  void MemoryServer::onDeleteArticle(int newsgroupIdentifier,
				       int articlIdentifier) {
    sendCommand(ANS_DELETE_ART);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void MemoryServer::onGetArticle(int newsgroupIdentifier,
				    int articleIdentifier) {
    sendCommand(ANS_GET_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }

  void MemoryServer::onConnectionMade(void) {
    // Do nothing yet
  }

  void MemoryServer::onConnectionLost(void) {
    // Do nothing yet
  }

}

