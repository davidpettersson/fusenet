/**
 * @file filesystem-database.cc
 *
 * This file contains the implementation of the filesystem database.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "filesystem-database.h"

#define THIS_CANNOT_HAPPEN (0 == "This cannot happen")

namespace fusenet {

  /**
   * Base directory.
   */
  const std::string baseDirectory = "db/";

  /**
   * Meta filename for newsgroups.
   */
  const std::string metaFilename = "meta";
  
  /**
   * Standard directory mode.
   */
  const int directoryMode = 0755;
  
  /**
   * Standard file mode.
   */
  const int fileMode = 0644;

  /*
   * Utility functions
   */

  /**
   * Base class for visitors.
   */
  class Visitor {
  public:
    virtual void visit(const std::string& directory, 
		       const std::string& filename) = 0;
  };

  /**
   * Walk directory.
   */
  bool Walk(const std::string directoryPath,
	    Visitor& visitor) {
    DIR* directory = NULL;
    struct dirent* entity = NULL;
    std::string filename;
    
    directory = opendir(directoryPath.c_str());
    
    if (directory == NULL) {
      return false;
    }

    entity = readdir(directory);
    
    while (entity != NULL) {
      filename = entity->d_name;

      if (filename != "." && filename != "..") {
	visitor.visit(directoryPath, filename);
      }

      entity = readdir(directory);
    }

    closedir(directory);
    return true;
  }

  /**
   * Get newsgroup path.
   */
  std::string GetNewsgroupPath(int newsgroupIdentifier) {
    std::ostringstream numberString;
    numberString << newsgroupIdentifier;
    return baseDirectory + numberString.str() + "/";
  }

  /**
   * Get article path.
   */
  std::string GetArticlePath(int newsgroupIdentifier,
			     int articleIdentifier) {
    std::ostringstream numberString;
    numberString << articleIdentifier;
    return GetNewsgroupPath(newsgroupIdentifier) + numberString.str();
  }

  /**
   * Read article from path.
   */
  bool ReadArticle(std::string& path, Article_t& article) {
    (void) path;
    (void) article;
    return false;
  }

  /**
   * Write article from path.
   */
  bool WriteArticle(std::string& path, Article_t& article) {
    (void) path;
    (void) article;
    return false;
  }

  /**
   * Read newsgroup name.
   */
  bool ReadNewsgroupName(std::string& newsgroupPath,
			 std::string& newsgroupName) {
    std::ifstream metaStream;
    std::string path;

    path = newsgroupPath + metaFilename;

    // FIXME: Error handling
    metaStream.open(path.c_str());
    metaStream >> newsgroupName;
    metaStream.close();

    return true;
  }

  /**
   * Write newsgroup metadata.
   */
  bool WriteNewsgroupName(std::string& newsgroupPath,
			  std::string& newsgroupName) {
    std::ofstream metaStream;
    std::string path;

    path = newsgroupPath + metaFilename;

    // FIXME: Error handling
    metaStream.open(path.c_str());
    metaStream << newsgroupName;
    metaStream.close();

    return true;
  }

  /**
   * Check if path is available.
   */
  bool PathAvailable(const std::string& path) {
    return access(path.c_str(), 0) == 0;
  }

  /**
   * Fetches the largest entity.
   */
  class MaxVisitor : public Visitor {
  private:
    int max;
  public:
    MaxVisitor(void) {
      max = 0;
    }
    
    void visit(const std::string& directory, 
	       const std::string& filename) {
      (void) directory;

      if (filename != metaFilename) {
	int current = atoi(filename.c_str());
	
	if (current > max) {
	  max = current;
	}
      }
    }
    
    int getMax(void) {
      return max;
    }       
  };

  /**
   * Get next filename number in directory.
   */
  int GetNextNumber(const std::string& directory) {
    MaxVisitor maxVisitor;

    if (Walk(directory, maxVisitor)) {
      if (maxVisitor.getMax() >= 0) {
	return maxVisitor.getMax() + 1;
      }
    }

    assert(0 == "This cannot happen");
    return -1;
  }

  /**
   * Fetches all newsgroups.
   */
  class NewsgroupListVisitor : public Visitor {
  private:
    NewsgroupList_t* newsgroupList;
  public:
    NewsgroupListVisitor(NewsgroupList_t& newsgroupList) {
      this->newsgroupList = &newsgroupList;
    }

    void visit(const std::string& directory, 
	       const std::string& filename) {
      Newsgroup_t newsgroup;
      std::string path = directory + filename + "/" + metaFilename;

      if (ReadNewsgroupName(path, newsgroup.name)) {
	newsgroupList->push_back(newsgroup);
      }

      /*
      std::istringstream identifierString(filename);


      std::ifstream metaStream;

      if (filename != metaFilename) {
	identifierString >> newsgroup.id;
	metaStream.open(path.c_str());
	metaStream >> newsgroup.name;
	metaStream.close();
	newsgroupList->push_back(newsgroup);
      }
      */
    }
  };

  /**
   * Fetches all articles.
   */
  class ArticleListVisitor : public Visitor {
  private:
    ArticleList_t* articleList;
  public:
    ArticleListVisitor(ArticleList_t& articleList) {
      this->articleList = &articleList;
    }

    void visit(const std::string& directory, 
	       const std::string& filename) {
      Article_t article;
      std::string path = directory + filename;

      if (ReadArticle(path, article)) {
	articleList->push_back(article);
      }
    }
  };

  /**
   * Clearing visitor. Removes the entire database.
   */
  class ClearVisitor : public Visitor {
  public:
    void visit(const std::string& directory, 
	       const std::string& filename) {
      std::string path = directory + filename;
      struct stat s;

      if (stat(path.c_str(), &s) == 0) {
	if (S_ISDIR(s.st_mode)) {
	  Walk(path + "/", *this);
	  rmdir(path.c_str());
	} else {
	  unlink(path.c_str());
	}
      }
    }
  };

  /*
   * FilesystemDatabase implementation
   */
      
  FilesystemDatabase::FilesystemDatabase(void) {
    // Ignore error code, we cannot do anything anyway
    mkdir(baseDirectory.c_str(), directoryMode);
  }

  Status_t FilesystemDatabase::clear(void) {
    Status_t status = STATUS_FAILURE;
    ClearVisitor clearVisitor;

    if (Walk(baseDirectory, clearVisitor)) {
      // FIXME: What goes here?
    }

    return status;
  }

  Status_t FilesystemDatabase::getNewsgroupList(NewsgroupList_t& newsgroupList) {
    Status_t status = STATUS_FAILURE;
    NewsgroupListVisitor listVisitor(newsgroupList);

    if (Walk(baseDirectory, listVisitor)) {
      status = STATUS_SUCCESS;
    }

    return status;
  }
  
  Status_t FilesystemDatabase::createNewsgroup(std::string& newsgroupName) {
    Status_t status = STATUS_FAILURE;
    NewsgroupList_t newsgroupList;
    NewsgroupList_t::iterator i;
    std::string path;
    
    if (IS_SUCCESS(getNewsgroupList(newsgroupList))) {
      for (i = newsgroupList.begin(); i != newsgroupList.end(); i++) {
	if (newsgroupName == (*i).name) {
	  return STATUS_FAILURE_ALREADY_EXISTS;
	}
      }
    }
    
    // FIXME: Error handling
    path = GetNewsgroupPath(GetNextNumber(baseDirectory));
    mkdir(path.c_str(), directoryMode);
    WriteNewsgroupName(path, newsgroupName);
    status = STATUS_SUCCESS;

    return status;
  }
      

    /*
    NewsgroupList_t::iterator listIterator;
    MaxVisitor maxVisitor;
    NewsgroupListVisitor listVisitor(newsgroupList);
    std::ostringstream maxString;
    std::ofstream metaStream;
    std::string newDirectory;
    std::string path;
    int rv;

    // Check if this name already exists
    if (Walk(baseDirectory, listVisitor)) {
      for (listIterator = newsgroupList.begin();
	   listIterator != newsgroupList.end();
	   listIterator++) {
	if (newsgroupName == (*listIterator).name) {
	  return STATUS_FAILURE_ALREADY_EXISTS;
	}
      }
    }

    path = getNewsgroupPath(getNextNumber(baseDirectory));
    mkdir(path.c_str(), directoryMode);
    // FIXME: write metadata
    */

    /*
    if (Walk(baseDirectory, maxVisitor)) {
      if (maxVisitor.getMax() >= 0) {
	maxString << maxVisitor.getMax();
	newDirectory = maxString.str() + "/";
	path = baseDirectory + newDirectory;

	rv = mkdir(path.c_str(), directoryMode);
	if (rv == 0) {
	  path += metaFilename;
	  metaStream.open(path.c_str());
	  metaStream << newsgroupName;
	  metaStream.close();
	  status = STATUS_SUCCESS;
	} else {
	  if (errno == EEXIST) {
	    status = STATUS_FAILURE_ALREADY_EXISTS;
	  }
	}
      }
    }

    return status;
  }
    */

  Status_t FilesystemDatabase::deleteNewsgroup(int newsgroupIdentifier) {
    Status_t status = STATUS_FAILURE;
    std::string newsgroupPath;
    ClearVisitor clearVisitor;

    newsgroupPath = GetNewsgroupPath(newsgroupIdentifier);

    if (PathAvailable(newsgroupPath)) {
      if (Walk(newsgroupPath, clearVisitor)) {
	status = STATUS_SUCCESS;
      }
    } else {
      status = STATUS_FAILURE_N_DOES_NOT_EXIST;
    }
      
    return status;;
  }

  Status_t FilesystemDatabase::listArticles(int newsgroupIdentifier,
					    ArticleList_t& articleList) {
    Status_t status = STATUS_FAILURE;
    ArticleListVisitor listVisitor(articleList);

    if (Walk(GetNewsgroupPath(newsgroupIdentifier), listVisitor)) {
      status = STATUS_SUCCESS;
    }

    return status;
  }

  Status_t FilesystemDatabase::createArticle(int newsgroupIdentifier,
					     Article_t& article) {
    Status_t status = STATUS_FAILURE;
    std::string path;

    path = GetNewsgroupPath(newsgroupIdentifier);
    
    if (PathAvailable(path)) {
      path = GetArticlePath(newsgroupIdentifier, GetNextNumber(path));

      if (!PathAvailable(path)) {
	if (WriteArticle(path, article)) {
	  status = STATUS_SUCCESS;
	} else {
	  status = STATUS_FAILURE;
	}
      } else {
	status = STATUS_FAILURE_ALREADY_EXISTS;
      }
    } else {
      status = STATUS_FAILURE_N_DOES_NOT_EXIST;
    }

    return status;
  }

  Status_t FilesystemDatabase::deleteArticle(int newsgroupIdentifier,
					     int articleIdentifier) {
    Status_t status = STATUS_FAILURE;
    std::string path;

    path = GetNewsgroupPath(newsgroupIdentifier);
    
    if (PathAvailable(path)) {
      path = GetArticlePath(newsgroupIdentifier, articleIdentifier);

      if (PathAvailable(path)) {
	unlink(path.c_str());
	status = STATUS_SUCCESS;
      } else {
	status = STATUS_FAILURE_A_DOES_NOT_EXIST;
      }
    } else {
      status = STATUS_FAILURE_N_DOES_NOT_EXIST;
    }

    return status;
  }

  Status_t FilesystemDatabase::getArticle(int newsgroupIdentifier,
					  int articleIdentifier,
					  Article_t& article) {
    Status_t status = STATUS_FAILURE;
    std::string path;

    path = GetNewsgroupPath(newsgroupIdentifier);
    
    if (PathAvailable(path)) {
      path = GetArticlePath(newsgroupIdentifier, articleIdentifier);

      if (PathAvailable(path)) {
	if (ReadArticle(path, article)) {
	  status = STATUS_SUCCESS;
	} else {
	  status = STATUS_FAILURE_A_DOES_NOT_EXIST;
	}
      } else {
	status = STATUS_FAILURE_A_DOES_NOT_EXIST;
      }
    } else {
      status = STATUS_FAILURE_N_DOES_NOT_EXIST;
    }

    return status;
  }

  FilesystemDatabase::~FilesystemDatabase(void) {
    // Does nothing
  }
}
