/**
 * @file client-protocol.cc
 *
 * This file contains the implementation of the client protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>

#include "client-protocol.h"

namespace fusenet {

  void ClientProtocol::listNewsgroups(void) {
    sendCommand(COM_LIST_NG);
    sendCommand(COM_END);
  }
  
  void ClientProtocol::receiveListNewsgroups(void) {
    size_t n;
    size_t i;
    std::vector<Newsgroup_t> newsgroupList;
    Newsgroup_t newsgroup;
    
    receiveParameter(reinterpret_cast<int*>(&n));
    
    std::cout << "Awaiting " << n << " newsgroups" << std::endl;
    
    for (i = 0; i < n; i++) {
      receiveParameter(&newsgroup.first);
      receiveParameter(newsgroup.second);
      newsgroupList.push_back(newsgroup);
    }

    assert(receiveAnswer() == ANS_END);
    
    onListNewsgroups(newsgroupList);
  }

  void ClientProtocol::onListNewsgroups(std::vector<Newsgroup_t>& newsgroupList) {
    std::vector<Newsgroup_t>::iterator i;

    for (i = newsgroupList.begin(); i != newsgroupList.end(); i++) {
      std::cout << (*i).first << " " << (*i).second << std::endl;
    }
  }

  void ClientProtocol::createNewsgroup(const std::string& name) {
    sendCommand(COM_CREATE_NG);
    sendParameter(name);
    sendCommand(COM_END);
  }

  void ClientProtocol::deleteNewsgroup(int newsgroupIdentifier) {
    sendCommand(COM_DELETE_NG);
    sendParameter(newsgroupIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::listArticles(int newsgroupIdentifier) {
    sendCommand(COM_LIST_ART);
    sendParameter(newsgroupIdentifier);
    sendCommand(COM_END);    
  }

  void ClientProtocol::createArticle(int newsgroupIdentifier,
				     const std::string& title,
				     const std::string& author,
				     const std::string& text) {
    sendCommand(COM_CREATE_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(title);
    sendParameter(author);
    sendParameter(text);
    sendCommand(COM_END);
  }
  
  void ClientProtocol::deleteArticle(int newsgroupIdentifier,
				     int articleIdentifier) {
    sendCommand(COM_DELETE_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(articleIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::getArticle(int newsgroupIdentifier,
				  int articleIdentifier) {
    sendCommand(COM_GET_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(articleIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::onConnectionMade(void) {
    interact();
  }

  void ClientProtocol::interact(void) {
    char command;

    std::cout << "Enter command: ";
    std::cin >> command;

    std::cout << "Got " << command << ", hehe..." << std::endl;

    switch (command) {

    case 'l': {
      listNewsgroups();
      break;
    }

    case 'c': {
      std::string newsgroupName;
      std::cout << "Enter newgroup name: ";
      std::cin >> newsgroupName;
      createNewsgroup(newsgroupName);
      break;
    }

    default: {
      std::cout << "Huh?" << std::endl;
      break;
    }
  }

  void ClientProtocol::onDataReceived(uint8_t data) {

    switch (data) {
    case ANS_LIST_NG:
      receiveListNewsgroups();
      break;
    default:
      break;
    }
  }
}

