/**
 * @file
 *
 * This file contains the client implementation.
 */

#include <cassert>

#include "client.h"

namespace fusenet {

  static void PrintStatus(Status_t status) {
    switch (status) {
    case STATUS_SUCCESS:
      std::cout << "Operation successful" << std::endl;
      break;
    case STATUS_FAILURE:
      std::cout << "Operation failed" << std::endl;
      break;
    case STATUS_FAILURE_ALREADY_EXISTS:
      std::cout << "Operation failed, item does not exist" << std::endl;
      break;
    case STATUS_FAILURE_N_DOES_NOT_EXIST:
      std::cout << "Operation failed, no such newsgroup" << std::endl;
      break;
    case STATUS_FAILURE_A_DOES_NOT_EXIST:
      std::cout << "Operation failed, no such article" << std::endl;
      break;
    default:
      assert(0);
      break;
    }
  }

  void Client::onListNewsgroups(Status_t status, NewsgroupList_t& newsgroupList) {
    NewsgroupList_t::iterator i;

    if (IS_SUCCESS(status)) {
      for (i = newsgroupList.begin(); i != newsgroupList.end(); i++) {
	Newsgroup_t& n = *i;
	std::cout << n.id << " " << n.name << std::endl;
      }
    } else {
      PrintStatus(status);
    }

    interact();
  }

  void Client::onCreateNewsgroup(Status_t status) {
    PrintStatus(status);
    interact();
  }
  
  void Client::onDeleteNewsgroup(Status_t status) {
    PrintStatus(status);
    interact();
  }

  void Client::onListArticles(Status_t status,
			      ArticleList_t& articleList) {
    ArticleList_t::iterator i;

    if (IS_SUCCESS(status)) {
      for (i = articleList.begin(); i != articleList.end(); i++) {
	Article_t& a = *i;
	std::cout << a.id << " " << a.title << " " << std::endl;
      }
    } else {
      PrintStatus(status);
    }

    interact();
  }

  void Client::onCreateArticle(Status_t status) {
    PrintStatus(status);
    interact();
  }
  
  void Client::onDeleteArticle(Status_t status) {
    PrintStatus(status);
    interact();
  }

  void Client::onGetArticle(Status_t status, Article_t& article) {
    if (IS_SUCCESS(status)) {
      std::cout << "Title   : " << article.title << std::endl;
      std::cout << "Author  : " << article.author << std::endl;
      std::cout << std::endl<< article.text << std::endl;
    } else {
      PrintStatus(status);
    }

    interact();
  }
  
  void Client::onConnectionMade(void) {
    std::cout << "Connection established" << std::endl;
    interact();
  }

  void Client::onConnectionLost(void) {
    std::cout << "Connection lost" << std::endl;
  }
  
  std::string Client::askString(const std::string& question) {
    std::string answer;

    std::cout << question << ": ";

    while (answer == "") {
      getline(std::cin, answer);
    }

    return answer;
  }
  
  int Client::askInteger(const std::string& question) {
    int answer;
    
    std::cout << question << ": ";
    std::cin >> answer;

    return answer;
  }
  
  void Client::printHelpMsg(void) {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  a  list articles" << std::endl;
    std::cout << "  c  create newsgroup" << std::endl;
    std::cout << "  d  delete article" << std::endl;
    std::cout << "  g  get article" << std::endl;
    std::cout << "  h  this help message" << std::endl;
    std::cout << "  k  delete newsgroup" << std::endl;
    std::cout << "  l  list newsgroups" << std::endl;
    std::cout << "  n  create article" << std::endl;
    interact();
  }
  
  void Client::interact(void) {
    std::string command;
    
    command = askString("Command (h for help)");
    std::cout << "Got '" << command << "'... excellent..." << std::endl;
    
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
}

