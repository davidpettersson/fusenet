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

#define INVALID_NID(x) ((static_cast<size_t>(x) > mapping.size()) || (mapping[x] == NULL))
#define INVALID_AID(x, y) ((static_cast<size_t>(x) > mapping[y]->second->size()) || (mapping[y]->second->at(x) == NULL))

namespace fusenet {

  MemoryDatabase::MemoryDatabase(void) {
    // Does nothing
  }

  Status_t MemoryDatabase::getNewsgroupList(NewsgroupList_t& newsgroupList) {
    if (mapping.size() < 1)
	    return STATUS_FAILURE;

    for (size_t i = 0; i < mapping.size(); ++i)
    {
	    if (mapping[i])
		    newsgroupList.push_back(mapping[i]->first);
    }
    return STATUS_SUCCESS;
  }
  
  Status_t MemoryDatabase::createNewsgroup(std::string& newsgroupName) {
    for (size_t i = 0; i < idmap.size(); ++i)
    {
	    if (idmap[i] && idmap[i]->compare(newsgroupName))
	    	return STATUS_FAILURE_ALREADY_EXISTS;
    }
    Newsgroup_t ng = {idmap.size(), newsgroupName};
    pair_t *tmp = new pair_t;
    tmp->first = ng;
    tmp->second = NULL;
    idmap.push_back(new std::string(newsgroupName));
    mapping.push_back(tmp);
    return STATUS_SUCCESS;
  }

  Status_t MemoryDatabase::deleteNewsgroup(int newsgroupIdentifier) {
    if (INVALID_NID(newsgroupIdentifier))
	    return STATUS_FAILURE_N_DOES_NOT_EXIST;
    if (mapping[newsgroupIdentifier]->second)
	    delete[] mapping[newsgroupIdentifier]->second;
    delete idmap[newsgroupIdentifier];
    delete mapping[newsgroupIdentifier];
    idmap[newsgroupIdentifier] = NULL;
    mapping[newsgroupIdentifier] = NULL;
    return STATUS_SUCCESS;
  }

  Status_t MemoryDatabase::listArticles(int newsgroupIdentifier,
					ArticleList_t& articleList) {
   if (INVALID_NID(newsgroupIdentifier))
	    return STATUS_FAILURE_N_DOES_NOT_EXIST;
   
   for (size_t i = 0; i < mapping[newsgroupIdentifier]->second->size(); ++i)
	   articleList.push_back(*(mapping[newsgroupIdentifier]->second->at(i)));
    
    return STATUS_SUCCESS;
  }

  Status_t MemoryDatabase::createArticle(int newsgroupIdentifier,
                                         Article_t& article) {
    if (INVALID_NID(newsgroupIdentifier))
	    return STATUS_FAILURE_N_DOES_NOT_EXIST;
    if (mapping[newsgroupIdentifier]->second == NULL)
	    mapping[newsgroupIdentifier]->second = new std::vector<Article_t *>;
    article.id = mapping[newsgroupIdentifier]->second->size();
    mapping[newsgroupIdentifier]->second->push_back(new Article_t(article));
    return STATUS_SUCCESS;
  }

  Status_t MemoryDatabase::deleteArticle(int newsgroupIdentifier,
					 int articleIdentifier) {
    if (INVALID_NID(newsgroupIdentifier))
	    return STATUS_FAILURE_N_DOES_NOT_EXIST;
    
    if (INVALID_AID(articleIdentifier, newsgroupIdentifier))
	    return STATUS_FAILURE_A_DOES_NOT_EXIST;
    delete mapping[newsgroupIdentifier]->second->at(articleIdentifier);
    mapping[newsgroupIdentifier]->second->at(articleIdentifier) = NULL;
    return STATUS_SUCCESS;
  }

  Status_t MemoryDatabase::getArticle(int newsgroupIdentifier,
				      int articleIdentifier,
				      Article_t& article) {
    if (INVALID_NID(newsgroupIdentifier))
	    return STATUS_FAILURE_N_DOES_NOT_EXIST;
    if (INVALID_AID(articleIdentifier, newsgroupIdentifier))
	    return STATUS_FAILURE_A_DOES_NOT_EXIST;
    article = *(mapping[newsgroupIdentifier]->second->at(articleIdentifier));
    return STATUS_SUCCESS;
  }

  MemoryDatabase::~MemoryDatabase(void) {
  }
}
