/**
 * @file client-protocol.cc
 *
 * This file contains the implementation of the client class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>

#include "client.h"

#define PRINT_ERR_NG  std::cout << "Newsgroup does not exist" << std::endl
#define PRINT_ERR_ART std::cout << "Article does not exist" << std::endl
#define PRINT_ERR_UNK std::cout << "Unknown error, error message not defined." << std::endl

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
    case STATUS_FAILURE_DOES_NOT_EXIST:
      std::cout << "Operation failed, no such item" << std::endl;
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
    std::cin >> answer;
    
    return answer;
  }
  
  int Client::askInteger(const std::string& question) {
    int answer;
    
    std::cout << question << ": ";
    std::cin >> answer;
    
    return answer;
  }
  
  void Client::printHelpMsg(void) {
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
  
  void Client::interact(void) {
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
}

