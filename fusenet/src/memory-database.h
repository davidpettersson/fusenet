#ifndef MEMORY_DATABASE_H
#define MEMORY_DATABASE_H

/**
 * @file memory-database.h
 *
 * This file contains the base database class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <map>

#include "fusenet-types.h"
#include "database.h"

typedef struct {
	fusenet::Newsgroup_t first;
	std::vector<fusenet::Article_t*> *second;
} pair_t;

typedef std::vector<std::string*> idmap_t;
typedef std::vector<pair_t*> mapping_t;

namespace fusenet {

  /**
   * Memory database.
   */
  class MemoryDatabase : public Database {

  public:

    /**
     * Create instance.
     */
    MemoryDatabase(void);

    /**
     * Get all newsgroups.
     */
    Status_t getNewsgroupList(NewsgroupList_t& newsgroupList);

    /**
     * Create newsgroups.
     */
    Status_t createNewsgroup(std::string& newsgroupName);

    /**
     * Delete newsgroup.
     */
    Status_t deleteNewsgroup(int newsgroupIdentifier);

    /**
     * List articles.
     */
    Status_t listArticles(int newsgroupIdentifier,
			  ArticleList_t& articleList);

    /**
     * Create article.
     */
    Status_t createArticle(int newsgroupIdentifier,
			   Article_t& article);

    /**
     * Delete article.
     */
    Status_t deleteArticle(int newsgroupIdentifer,
			   int articleIdentifier);

    /**
     * Get article.
     */
    Status_t getArticle(int newsgroupIdentifer,
			int articleIdentifier,
			Article_t& article);

    /**
     * Destroy instance.
     */
    virtual ~MemoryDatabase(void);

  private:
    idmap_t idmap;
    mapping_t mapping;
  };
}

#endif

