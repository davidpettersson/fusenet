/**
 * @file client-protocol.cc
 *
 * This file contains the implementation of the client protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>

#include "client-protocol.h"

#define PRINT_ERR_NG  std::cout << "Newsgroup does not exist" << std::endl
#define PRINT_ERR_ART std::cout << "Article does not exist" << std::endl
#define PRINT_ERR_UNK std::cout << "Unknown error, error message not defined." << std::endl

namespace fusenet {

  void ClientProtocol::listNewsgroups(void) {
    sendCommand(COM_LIST_NG);
    sendCommand(COM_END);
  }
  
  void ClientProtocol::receiveListNewsgroups(void) {
    size_t n, i;
    std::vector<Newsgroup_t> newsgroupList;
    Newsgroup_t newsgroup;
    
    receiveParameter(reinterpret_cast<int*>(&n));
    
    for (i = 0; i < n; i++) {
      receiveParameter(&newsgroup.first);
      receiveParameter(newsgroup.second);
      newsgroupList.push_back(newsgroup);
    }

    expectCommand(ANS_END);
    onListNewsgroups(newsgroupList);
  }

  void ClientProtocol::onListNewsgroups(std::vector<Newsgroup_t>& newsgroupList) {
    std::vector<Newsgroup_t>::iterator i;

    for (i = newsgroupList.begin(); i != newsgroupList.end(); i++) {
      std::cout << (*i).first << " " << (*i).second << std::endl;
    }

    interact();
  }

  void ClientProtocol::createNewsgroup(const std::string& name) {
    sendCommand(COM_CREATE_NG);
    sendParameter(name);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveCreateNewsgroup(void) {
    MessageIdentifier_t status = receiveCommand();

    if (status == ANS_ACK) {
      std::cout << "Newsgroup created" << std::endl;
    } else {
      status = receiveCommand();

      if (status == ERR_NG_ALREADY_EXISTS) {
	std::cout << "Newsgroup already exists" << std::endl;
      } else {
	PRINT_ERR_UNK;
      }
    }

    expectCommand(ANS_END);
    interact();
  }

  void ClientProtocol::deleteNewsgroup(int newsgroupIdentifier) {
    sendCommand(COM_DELETE_NG);
    sendParameter(newsgroupIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveDeleteNewsgroup(void) {
    MessageIdentifier_t status = receiveCommand();

    if (status == ANS_ACK) {
      std::cout << "Newsgroup deleted" << std::endl;
    } else {
      std::cout << "Error, newsgroup not deleted: ";
      status = receiveCommand();

      if (status == ERR_NG_DOES_NOT_EXIST) {
	PRINT_ERR_NG;
      } else {
	PRINT_ERR_UNK;
      }
    }

    expectCommand(ANS_END);
    interact();
  }

  void ClientProtocol::listArticles(int newsgroupIdentifier) {
    sendCommand(COM_LIST_ART);
    sendParameter(newsgroupIdentifier);
    sendCommand(COM_END);    
  }

  void ClientProtocol::receiveListArticles(void) {
    MessageIdentifier_t status = receiveCommand();
    std::vector<Article_t> articleList;
    Article_t article;
    int n;
    int i;
    bool success;

    if (status == ANS_ACK) {
      receiveParameter(&n);

      for (i = 0; i < n; i++) {
	receiveParameter(&article.first);
	receiveParameter(article.second);
	articleList.push_back(article);
      }

      success = true;
    } else {
      success = false;
    }

    expectCommand(ANS_END);
    onListArticles(success, articleList);
  }

  void ClientProtocol::onListArticles(bool success,
				      std::vector<Article_t> articleList) {
    std::vector<Newsgroup_t>::iterator i;

    if (success) {
      for (i = articleList.begin(); i != articleList.end(); i++) {
	std::cout << (*i).first << " " << (*i).second << std::endl;
      }
    } else {
      std::cout << "No such newsgroup" << std::endl;
    }

    interact();
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

  void ClientProtocol::receiveCreateArticle(void) {
    MessageIdentifier_t status;
    status = receiveCommand();

    if (status == ANS_ACK) {
      std::cout << "Article created" << std::endl;
    } else {
      std::cout << "Error, article not created: ";
      status = receiveCommand();

      if (status == ERR_NG_DOES_NOT_EXIST) {
	PRINT_ERR_NG;
      } else {
	PRINT_ERR_UNK;
      }
    }

    expectCommand(ANS_END);
    interact();
  }
  
  void ClientProtocol::deleteArticle(int newsgroupIdentifier,
				     int articleIdentifier) {
    sendCommand(COM_DELETE_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(articleIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveDeleteArticle(void) {
    MessageIdentifier_t status;
    status = receiveCommand();

    if (status == ANS_ACK) {
      std::cout << "Article deleted" << std::endl;
    } else {
      std::cout << "Error, article not deleted: ";
      status = receiveCommand();

      if (status == ERR_NG_DOES_NOT_EXIST) {
        PRINT_ERR_NG;
      } else if (status == ERR_ART_DOES_NOT_EXIST) {
        PRINT_ERR_ART;
      } else {
	PRINT_ERR_UNK;
      }
    }

    expectCommand(ANS_END);
    interact();
  }

  void ClientProtocol::getArticle(int newsgroupIdentifier,
				  int articleIdentifier) {
    sendCommand(COM_GET_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(articleIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveGetArticle(void) {
    MessageIdentifier_t status;
    status = receiveCommand();
    std::string articleTitle;
    std::string articleAuthor;
    std::string articleText;

    if (status == ANS_ACK) {
      receiveParameter(articleTitle);
      receiveParameter(articleAuthor);
      receiveParameter(articleText);

      std::cout << "Title  : " << articleTitle << std::endl;
      std::cout << "Author : " << articleAuthor << std::endl;
      std::cout << "-------------------------------------------------------------------" << std:: endl;
      std::cout << articleText << std::endl;
    } else {
      std::cout << "Error, article not sent: ";
      status = receiveCommand();

      if (status == ERR_NG_DOES_NOT_EXIST) {
        PRINT_ERR_NG;
      } else if (status == ERR_ART_DOES_NOT_EXIST) {
        PRINT_ERR_ART;
      } else {
	PRINT_ERR_UNK;
      }
    }

    expectCommand(ANS_END);
    interact();
  }
  
  void ClientProtocol::onConnectionMade(void) {
    interact();
  }
  
  std::string ClientProtocol::askString(const std::string& question) {
    std::string answer;
    
    std::cout << question << ": ";
    std::cin >> answer;
    
    return answer;
  }
  
  int ClientProtocol::askInteger(const std::string& question) {
    int answer;
    
    std::cout << question << ": ";
    std::cin >> answer;
    
    return answer;
  }
  
  void ClientProtocol::printHelpMsg(void) {
    std::cout << "l\t-\tlist newsgroups" << std::endl;
    std::cout << "c\t-\tcreate newsgroup" << std::endl;
    std::cout << "k\t-\tdelete newsgroup" << std::endl;
    std::cout << "a\t-\tlist articles" << std::endl;
    std::cout << "n\t-\tcreate article" << std::endl;
    std::cout << "d\t-\tdelete article" << std::endl;
    std::cout << "g\t-\tget article" << std::endl;
    std::cout << "h\t-\tthis help message" << std::endl;
    interact();
  }
  
  void ClientProtocol::interact(void) {
    std::string command;
    
    command = askString("Enter command");
    std::cout << "Got " << command << ", hehe..." << std::endl;
    
    switch (command[0]) {
      
    case 'l': 
      {
	listNewsgroups();
	break;
      }
      
    case 'c': 
      {
	std::string newsgroupName = askString("Enter newsgroup name");
	createNewsgroup(newsgroupName);
	break;
      }
      
    case 'k': 
      {
	int newsgroupIdentifier = askInteger("Enter newsgroup identifier");
	deleteNewsgroup(newsgroupIdentifier);
	break;
      }
      
    case 'a': 
      {
	int newsgroupIdentifier = askInteger("Enter newsgroup identifier");
	listArticles(newsgroupIdentifier);
	break;
      }
      
    case 'n': 
      {
	int newsgroupIdentifier = askInteger("Enter newsgroup identifier");
	std::string articleTitle = askString("Enter article title");
	std::string articleAuthor = askString("Enter article author");
	std::string articleText = askString("Enter article text");
	createArticle(newsgroupIdentifier, articleTitle, 
			articleAuthor, articleText);
	break;
      }
   
    case 'd':
      {
	int newsgroupIdentifier = askInteger("Enter newsgroup identifier");
	int articleIdentifier = askInteger("Enter article identifier");
	deleteArticle(newsgroupIdentifier, articleIdentifier);
	break;
      }

    case 'g':
      {
	int newsgroupIdentifier = askInteger("Enter newsgroup identifier");
	int articleIdentifier = askInteger("Enter article identifier");
	getArticle(newsgroupIdentifier, articleIdentifier);
	break;
      }

    case 'q':
      {
	exit(0);
	break;
      }

    default: 
      {
	printHelpMsg();
	break;
      }
    }
  }
  
  void ClientProtocol::onDataReceived(uint8_t data) {
    
    switch (data) {
    case ANS_LIST_NG:
      receiveListNewsgroups();
      break;
    case ANS_CREATE_NG:
      receiveCreateNewsgroup();
      break;
    case ANS_DELETE_NG:
      receiveDeleteNewsgroup();
      break;
    case ANS_LIST_ART:
      receiveListArticles();
      break;
    case ANS_CREATE_ART:
      receiveCreateArticle();
      break;
    case ANS_DELETE_ART:
      receiveDeleteArticle();
      break;
    case ANS_GET_ART:
      receiveGetArticle();
      break;
    default:
      std::cout << "Throwing away data " << static_cast<int>(data)
		<< " (this is a bad thing)" << std::endl;
      break;
    }
  }
}

