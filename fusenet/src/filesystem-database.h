#ifndef FILESYSTEM_DATABASE_H
#define FILESYSTEM_DATABASE_H

/**
 * @file filesystem-database.h
 *
 * This file contains the base database class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "fusenet-types.h"
#include "database.h"

namespace fusenet {

  /**
   * Filesystem database.
   */
  class FilesystemDatabase : public Database {

  public:

    /**
     * Create instance.
     */
    FilesystemDatabase(void);

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
    virtual ~FilesystemDatabase(void);
  };
}

#endif

