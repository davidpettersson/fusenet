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

#include <string>

namespace fusenet {

  /**
   * Filesystem database. The filesystem database uses a simple
   * directory structure with plain text files as its back end. The
   * directory for a working database looks like this:
   *
   * <pre>
   *   db/ --+-- 1/ ---+--- meta
   *         |         |
   *         |         +--- 1
   *         |         |
   *         |         +--- 2
   *         |
   *         +-- 2/
   *         |
   *         :
   * </pre>
   *
   * The database is contained in a db/ directory that lies in the
   * current working directory. Inside the db directory, there exists
   * one directory per newsgroup. Each group has one meta file
   * containing the name of the group, and each article is one plain
   * text file in this directory.
   */
  class FilesystemDatabase : public Database {

  public:

    /**
     * Create instance.
     */
    FilesystemDatabase(void);

    /**
     * Clear the database.
     */
    Status_t clear(void);

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

  private:

  };
}

#endif

