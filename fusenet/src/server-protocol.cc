/**
 * @file server-protocol.cc
 *
 * This file contains the implementation of the server protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>
#include <string>

#include "server-protocol.h"

namespace fusenet {

  void ServerProtocol::handleListNewsgroups(void) {
    receiveCommand();
    onListNewsgroups();
  }

  void ServerProtocol::onListNewsgroups(void) {
    sendCommand(ANS_LIST_NG);
    sendParameter(2);
    sendParameter(1);
    sendParameter("foo");
    sendParameter(2);
    sendParameter("bar");
    sendCommand(ANS_END);
  }

  void ServerProtocol::handleCreateNewsgroup(void) {
    std::string name;
    receiveParameter(name);
    receiveCommand();
    onCreateNewsgroup(name);
  }

  void ServerProtocol::onCreateNewsgroup(std::string& newsgroupName) {
    sendCommand(ANS_CREATE_NG);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void ServerProtocol::handleDeleteNewsgroup(void) {
    int id;
    receiveParameter(&id);
    receiveCommand();
    onDeleteNewsgroup(id);
  }

  void ServerProtocol::onDeleteNewsgroup(int newsgroupIdentifier) {
    sendCommand(ANS_DELETE_NG);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void ServerProtocol::handleListArticles(void) {
    int group;
    receiveParameter(&group);
    receiveCommand();
    onListArticles(group);
  }
    
  void ServerProtocol::onListArticles(int newsgroupIdentifier) {
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

  void ServerProtocol::handleCreateArticle(void) {
    int ngid;
    std::string title, author, text;
    receiveParameter(&ngid);
    receiveParameter(title);
    receiveParameter(author);
    receiveParameter(text);
    receiveCommand();
    onCreateArticle(ngid, title, author, text);
  }

  void ServerProtocol::onCreateArticle(int newsgroupIdentifier,
				       std::string& articleTitle,
				       std::string& articleAuthor,
				       std::string& articleText) {
    sendCommand(ANS_CREATE_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }
  
  void ServerProtocol::handleDeleteArticle(void) {
    int gid, aid;
    receiveParameter(&gid);
    receiveParameter(&aid);
    receiveCommand();
    onDeleteArticle(gid, aid);
  }

  void ServerProtocol::onDeleteArticle(int newsgroupIdentifier,
				       int articlIdentifier) {
    sendCommand(ANS_DELETE_ART);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void ServerProtocol::handleGetArticle(void) {
    int gid, aid;
    receiveParameter(&gid);
    receiveParameter(&aid);
    receiveCommand();
    onGetArticle(gid, aid);
  }

  void ServerProtocol::onGetArticle(int newsgroupIdentifier,
				    int articleIdentifier) {
    sendCommand(ANS_GET_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }

  void ServerProtocol::onConnectionMade(void) {
    // Do nothing yet
  }

  void ServerProtocol::onDataReceived(uint8_t data) {

    switch (data) {
    case COM_LIST_NG:
      handleListNewsgroups();
      break;
    case COM_CREATE_NG:
      handleCreateNewsgroup();
      break;
    case COM_DELETE_NG:
      handleDeleteNewsgroup();
      break;
    case COM_LIST_ART:
      handleListArticles();
      break;
    case COM_CREATE_ART:
      handleCreateArticle();
      break;
    case COM_DELETE_ART:
      handleDeleteArticle();
      break;
    case COM_GET_ART:
      handleGetArticle();
      break;
    default:
      std::cerr << "Error, unknown command byte: " 
		<< static_cast<int>(data) << std::endl;
      break;
    }
  }
}

