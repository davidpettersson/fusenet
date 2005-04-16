#ifndef DATABASE_H
#define DATABASE_H

/**
 * @file
 *
 * This file contains the database interface.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <string>

#include "fusenet-types.h"

namespace fusenet {

  /**
   * Base class for all databases. This base class is supposed to be
   * inherited from by all different kinds of databases, in this
   * project a memory based and a disk based version.
   */
  class Database {

  public:

    /**
     * Create instance.
     */
    Database(void);

    /**
     * Get all newsgroups.
     */
    virtual Status_t getNewsgroupList(NewsgroupList_t& newsgroupList) = 0;

    /**
     * Create newsgroups.
     */
    virtual Status_t createNewsgroup(std::string& newsgroupName) = 0;

    /**
     * Delete newsgroup.
     */
    virtual Status_t deleteNewsgroup(int newsgroupIdentifier) = 0;

    /**
     * List articles.
     */
    virtual Status_t listArticles(int newsgroupIdentifier,
				  ArticleList_t& articleList) = 0;

    /**
     * Create article.
     */
    virtual Status_t createArticle(int newsgroupIdentifier,
				   Article_t& article) = 0;

    /**
     * Delete article.
     */
    virtual Status_t deleteArticle(int newsgroupIdentifer,
				   int articleIdentifier) = 0;

    /**
     * Get article.
     */
    virtual Status_t getArticle(int newsgroupIdentifer,
				int articleIdentifier,
				Article_t& article) = 0;

    /**
     * Destroy instance.
     */
    virtual ~Database(void);
  };
}

#endif

