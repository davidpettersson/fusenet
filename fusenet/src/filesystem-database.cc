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

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "filesystem-database.h"

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
   * Forward declaration of the walker function.
   */
  class Visitor;
  bool Walk(const std::string directoryPath, Visitor& visitor);
  
  /**
   * Base class for visitors.
   */
  class Visitor {
  public:
    virtual void visit(const std::string& directory, 
		       const std::string& filename) = 0;
  };

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
      if (filename != metaFilename) {
	int current = atoi(filename.c_str());
	
	if (current >= max) {
	  max = current + 1;
	}
      }
    }
    
    int getMax(void) {
      return max;
    }       
  };

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
      std::ifstream metaStream;
      std::string path = directory + filename + "/" + metaFilename;
      std::istringstream identifierString(filename);

      if (filename != metaFilename) {
	identifierString >> newsgroup.id;
	metaStream.open(path.c_str());
	metaStream >> newsgroup.name;
	metaStream.close();
	newsgroupList->push_back(newsgroup);
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

  /**
   * Walk function for the visitors. This is not a method in the
   * FilesystemDatabase class since it need to be access outside the
   * class (e.g. by the visitors).
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

    // Figure out largest number and create following group
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

  Status_t FilesystemDatabase::deleteNewsgroup(int newsgroupIdentifier) {
    Status_t status = STATUS_FAILURE;
    std::string newsgroupPath;
    std::ostringstream numberString;
    ClearVisitor clearVisitor;

    numberString << newsgroupIdentifier;
    newsgroupPath = baseDirectory + numberString.str();

    if (pathAvailable(newsgroupPath)) {
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
    return STATUS_FAILURE;
  }

  Status_t FilesystemDatabase::createArticle(int newsgroupIdentifier,
					     Article_t& article) {
    return STATUS_FAILURE;
  }

  Status_t FilesystemDatabase::deleteArticle(int newsgroupIdentifer,
					     int articleIdentifier) {
    return STATUS_FAILURE;
  }

  Status_t FilesystemDatabase::getArticle(int newsgroupIdentifer,
					  int articleIdentifier,
					  Article_t& article) {
    return STATUS_FAILURE;
  }

  bool FilesystemDatabase::pathAvailable(const std::string& path) {
    int rv = access(path.c_str(), 0);
    return (rv == 0);
  }

  FilesystemDatabase::~FilesystemDatabase(void) {
    // Does nothing
  }
}
