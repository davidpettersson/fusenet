/**
 * @file filesystem-database.cc
 *
 * This file contains the mostly implementation of the filesystem
 * database.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>

#include "filesystem-database.h"

namespace fusenet {

  FilesystemDatabase::FilesystemDatabase(void) {
    // Does nothing
  }

  Status_t FilesystemDatabase::getNewsgroupList(NewsgroupList_t& newsgroupList) {
    return STATUS_FAILURE;
  }
  
  Status_t FilesystemDatabase::createNewsgroup(std::string& newsgroupName) {
    return STATUS_FAILURE;
  }

  Status_t FilesystemDatabase::deleteNewsgroup(int newsgroupIdentifier) {
    return STATUS_FAILURE;
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

  FilesystemDatabase::~FilesystemDatabase(void) {
    // Does nothing
  }
}
