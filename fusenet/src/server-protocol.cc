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

  void ServerProtocol::listNewsgroups(void) {
    sendCommand(COM_LIST_NG);
    sendCommand(COM_END);
  }
  
  void ServerProtocol::receiveListNewsgroups(void) {
    receiveCommand();
    sendCommand(ANS_LIST_NG);
    sendParameter(2);
    sendParameter(1);
    sendParameter("foo");
    sendParameter(2);
    sendParameter("bar");
    sendCommand(ANS_END);
  }

  void ServerProtocol::onListNewsgroups(std::vector<Newsgroup_t>& newsgroupList) {
    std::vector<Newsgroup_t>::iterator i;

    for (i = newsgroupList.begin(); i != newsgroupList.end(); i++) {
      std::cout << (*i).first << " " << (*i).second << std::endl;
    }
  }

  void ServerProtocol::createNewsgroup(void) {
    std::string name;
    receiveParameter(name);
    receiveCommand();
    sendCommand(ANS_CREATE_NG);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void ServerProtocol::deleteNewsgroup(void) {
    int id;
    receiveParameter(&id);
    receiveCommand();
    sendCommand(ANS_DELETE_NG);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void ServerProtocol::listArticles(void) {
    int group;
    receiveParameter(&group);
    receiveCommand();
    sendCommand(ANS_LIST_ART);
    if (group == 1)
    {
    	sendCommand(ANS_ACK);
	sendParameter(2);
	sendParameter(1);
	sendParameter("haXX0r");
	sendParameter(2);
	sendParameter("eller ngt");
    }
    else
    {
	sendCommand(ANS_NAK);
	sendCommand(ERR_NG_DOES_NOT_EXIST);
    }
    sendCommand(ANS_END);
  }

  void ServerProtocol::createArticle(void) {
    int ngid;
    std::string title, author, text;
    receiveParameter(&ngid);
    receiveParameter(title);
    receiveParameter(author);
    receiveParameter(text);
    receiveCommand();
    sendCommand(ANS_CREATE_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }
  
  void ServerProtocol::deleteArticle(void) {
    int gid, aid;
    receiveParameter(&gid);
    receiveParameter(&aid);
    receiveCommand();
    sendCommand(ANS_DELETE_ART);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void ServerProtocol::getArticle(void) {
    int gid, aid;
    receiveParameter(&gid);
    receiveParameter(&aid);
    receiveCommand();
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
      receiveListNewsgroups();
      break;
    case COM_CREATE_NG:
      createNewsgroup();
      break;
    case COM_DELETE_NG:
      deleteNewsgroup();
      break;
    case COM_LIST_ART:
      listArticles();
      break;
    case COM_CREATE_ART:
      createArticle();
      break;
    case COM_DELETE_ART:
      deleteArticle();
      break;
    case COM_GET_ART:
      getArticle();
      break;
    default:
      std::cerr << "Error, unknown command byte: " 
		<< static_cast<int>(data) << std::endl;
      break;
    }
  }
}

