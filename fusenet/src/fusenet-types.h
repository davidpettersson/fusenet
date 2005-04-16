#ifndef FUSENET_TYPES_H
#define FUSENET_TYPES_H

/**
 * @file
 *
 * This file contains all the common types.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>
#include <vector>

#include "message-identifiers.h"

#define IS_SUCCESS(s) ((s) == STATUS_SUCCESS)

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

  /**
   * Status type.
   */
  typedef enum {
    STATUS_SUCCESS,
    STATUS_FAILURE,
    STATUS_FAILURE_ALREADY_EXISTS,
    STATUS_FAILURE_N_DOES_NOT_EXIST,
    STATUS_FAILURE_A_DOES_NOT_EXIST
  }
  Status_t;
    
}

#endif

