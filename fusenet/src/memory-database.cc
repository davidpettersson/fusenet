/**
 * @file memory-database.cc
 *
 * This file contains the mostly implementation of the memory
 * database.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>

#include "memory-database.h"

namespace fusenet {

  MemoryDatabase::MemoryDatabase(void) {
    // Does nothing
  }

  Status_t MemoryDatabase::getNewsgroupList(NewsgroupList_t& newsgroupList) {
    return STATUS_FAILURE;
  }
  
  Status_t MemoryDatabase::createNewsgroup(std::string& newsgroupName) {
    return STATUS_FAILURE;
  }

  Status_t MemoryDatabase::deleteNewsgroup(int newsgroupIdentifier) {
    return STATUS_FAILURE;
  }

  Status_t MemoryDatabase::listArticles(int newsgroupIdentifier,
					ArticleList_t& articleList) {
    return STATUS_FAILURE;
  }

  Status_t MemoryDatabase::createArticle(Article_t& article) {
    return STATUS_FAILURE;
  }

  Status_t MemoryDatabase::deleteArticle(int newsgroupIdentifer,
					 int articleIdentifier) {
    return STATUS_FAILURE;
  }

  Status_t MemoryDatabase::getArticle(int newsgroupIdentifer,
				      int articleIdentifier,
				      Article_t& article) {
    return STATUS_FAILURE;
  }

  MemoryDatabase::~MemoryDatabase(void) {
    // Does nothing
  }
}
