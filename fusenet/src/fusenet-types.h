#ifndef FUSENET_TYPES_H
#define FUSENET_TYPES_H

/**
 * @file fusenet-types.h
 *
 * This file contains all the fusenet types.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>
#include <vector>

#include "message-identifiers.h"

namespace fusenet {

  /**
   * Newsgroup type.
   */
  typedef struct {
    int id;           //!< Identifier
    std::string name; //!< Name
  } Newsgroup_t;

  /**
   * Article type.
   */
  typedef struct {
    int id;             //!< Identifier
    std::string title;  //!< Title
    std::string author; //!< Author
    std::string text;   //!< Text
  } Article_t;

  /**
   * Newsgroup list.
   */
  typedef std::vector<Newsgroup_t> NewsgroupList_t;

  /**
   * Article list.
   */
  typedef std::vector<Article_t> ArticleList_t;
}

#endif

