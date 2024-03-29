/**
 * @file
 *
 * Tests the database implementations.
 */

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "fusenet-types.h"
#include "memory-database.h"
#include "filesystem-database.h"

using namespace fusenet;

static bool UseMemoryDatabase = false;

static Database* MakeDatabase(void) {
  if (UseMemoryDatabase) {
    return new MemoryDatabase();
  } else {
    FilesystemDatabase* pDatabase = new FilesystemDatabase();
    if (pDatabase != NULL) {
      pDatabase->clear();
    }
    return pDatabase;
  }
}

class NewsgroupTestFixture : public CppUnit::TestFixture {
protected:
  Database* pDatabase;
public:
  void setUp() {
    pDatabase = MakeDatabase();
    CPPUNIT_ASSERT(pDatabase != NULL);
  }
  void tearDown() {
    CPPUNIT_ASSERT(pDatabase != NULL);
    delete pDatabase;
  }
};

class ArticleTestFixture : public CppUnit::TestFixture {
protected:
  Database* pDatabase;
  Newsgroup_t newsgroup;
public:
  void setUp() {
    NewsgroupList_t newsgroupList;
    std::string name("foo");
    pDatabase = MakeDatabase();
    CPPUNIT_ASSERT(pDatabase != NULL);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
    newsgroup = newsgroupList.front();
  }
  void tearDown() {
    CPPUNIT_ASSERT(pDatabase != NULL);
    delete pDatabase;
  }
};

class CreateNewsgroupTest : public NewsgroupTestFixture {
  CPPUNIT_TEST_SUITE(CreateNewsgroupTest);
  CPPUNIT_TEST(testSingle);
  CPPUNIT_TEST(testDouble);
  CPPUNIT_TEST(testEmpty);
  CPPUNIT_TEST_SUITE_END();
public:
  void testSingle() {
    NewsgroupList_t newsgroupList;
    std::string name("foo");
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
  }
  void testDouble() {
    NewsgroupList_t newsgroupList;
    std::string name("foo");
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
    CPPUNIT_ASSERT(pDatabase->createNewsgroup(name) == STATUS_FAILURE_ALREADY_EXISTS);

    newsgroupList.clear();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);

    name = "bar";
    newsgroupList.clear();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 2);
  }
  void testEmpty() {
    NewsgroupList_t newsgroupList;
    std::string emptyName;
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(emptyName)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
  }
};

class ListNewsgroupsTest : public NewsgroupTestFixture {
  CPPUNIT_TEST_SUITE(ListNewsgroupsTest);
  CPPUNIT_TEST(testEmpty);
  CPPUNIT_TEST(testNonEmpty);
  CPPUNIT_TEST_SUITE_END();
public:
  void testEmpty() {
    NewsgroupList_t newsgroupList;
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
  }
  void testNonEmpty() {
    NewsgroupList_t newsgroupList;
    std::string name("foo");
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
  }
};

class DeleteNewsgroupTest : public NewsgroupTestFixture {
  CPPUNIT_TEST_SUITE(DeleteNewsgroupTest);
  CPPUNIT_TEST(testEmpty1);
  CPPUNIT_TEST(testEmpty2);
  CPPUNIT_TEST(testEmpty3);
  CPPUNIT_TEST(testNonEmpty);
  CPPUNIT_TEST(testSingle);
  CPPUNIT_TEST(testWithArticles);
  CPPUNIT_TEST_SUITE_END();
public:
  void testEmpty1() {
    NewsgroupList_t newsgroupList;
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(!IS_SUCCESS(pDatabase->deleteNewsgroup(0)));
  }
  void testEmpty2() {
    NewsgroupList_t newsgroupList;
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(!IS_SUCCESS(pDatabase->deleteNewsgroup(-1)));
  }
  void testEmpty3() {
    NewsgroupList_t newsgroupList;
    CPPUNIT_ASSERT(newsgroupList.size() == 0);
    CPPUNIT_ASSERT(pDatabase->deleteNewsgroup(0) == STATUS_FAILURE_N_DOES_NOT_EXIST);
  }
  void testNonEmpty() {
    std::string name("foo");
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(pDatabase->deleteNewsgroup(-1) == STATUS_FAILURE_N_DOES_NOT_EXIST);
  }
  void testSingle() {
    NewsgroupList_t newsgroupList;
    Newsgroup_t newsgroup;
    std::string name("foo");
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
    newsgroup = newsgroupList.front();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->deleteNewsgroup(newsgroup.id)));
  }
  void testWithArticles() {
    NewsgroupList_t newsgroupList;
    Newsgroup_t newsgroup;
    Article_t article;

    article.title = "1984";
    article.author = "George Orwell";
    article.text = "Big brother ...";

    std::string name("foo");
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getNewsgroupList(newsgroupList)));
    CPPUNIT_ASSERT(newsgroupList.size() == 1);
    newsgroup = newsgroupList.front();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createArticle(newsgroup.id, article)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->deleteNewsgroup(newsgroup.id)));
  }
};

class CreateArticleTest : public ArticleTestFixture {
  CPPUNIT_TEST_SUITE(CreateArticleTest);
  CPPUNIT_TEST(testWrongNewsgroup);
  CPPUNIT_TEST(testRightNewsgroup);
  CPPUNIT_TEST(testIncrement);
  CPPUNIT_TEST_SUITE_END();
public:
  void testWrongNewsgroup() {
    Article_t article;
    ArticleList_t articleList;
    article.title = "1984";
    article.author = "George Orwell";
    article.text = "Big brother ...";
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 0);
    CPPUNIT_ASSERT(pDatabase->createArticle(newsgroup.id + 1, article) == STATUS_FAILURE_N_DOES_NOT_EXIST);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 0);
  }
  void testRightNewsgroup() {
    Article_t article;
    ArticleList_t articleList;
    article.title = "1984";
    article.author = "George Orwell";
    article.text = "Big brother ...";
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createArticle(newsgroup.id, article)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 1);
  }
  void testIncrement() {
    Article_t article;
    ArticleList_t articleList;
    int increment = -1;
    article.title = "1984";
    article.author = "George Orwell";
    article.text = "Big brother ...";
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createArticle(newsgroup.id, article)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 1);
    increment = articleList[0].id;
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->deleteArticle(newsgroup.id, increment)));
    articleList.clear();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 0);
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createArticle(newsgroup.id, article)));
    articleList.clear();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 1);
    CPPUNIT_ASSERT(articleList[0].id == increment + 1);
  }
};

class ListArticlesTest : public ArticleTestFixture {
  CPPUNIT_TEST_SUITE(ListArticlesTest);
  CPPUNIT_TEST(testNonExistant);
  CPPUNIT_TEST(testEmpty);
  CPPUNIT_TEST_SUITE_END();
public:
  void testNonExistant() {
    ArticleList_t articleList;
    CPPUNIT_ASSERT(pDatabase->listArticles(newsgroup.id + 1, articleList) == STATUS_FAILURE_N_DOES_NOT_EXIST);
  }
  void testEmpty() {
    ArticleList_t articleList;
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
  }
};

class DeleteArticleTest : public ArticleTestFixture {
  CPPUNIT_TEST_SUITE(DeleteArticleTest);
  CPPUNIT_TEST(testWrongNewsgroup);
  CPPUNIT_TEST(testWrongArticle);
  CPPUNIT_TEST(testRight);
  CPPUNIT_TEST_SUITE_END();
public:
  void testWrongNewsgroup() {
    CPPUNIT_ASSERT(pDatabase->deleteArticle(newsgroup.id + 1, 0) == STATUS_FAILURE_N_DOES_NOT_EXIST);
  }
  void testWrongArticle() {
    CPPUNIT_ASSERT(pDatabase->deleteArticle(newsgroup.id, 0) == STATUS_FAILURE_A_DOES_NOT_EXIST);
  }
  void testRight() {
    Article_t article;
    ArticleList_t articleList;
    article.title = "1984";
    article.author = "George Orwell";
    article.text = "Big brother ...";
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createArticle(newsgroup.id, article)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 1);
    article = articleList.front();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->deleteArticle(newsgroup.id, article.id)));
  }
};

class GetArticleTest : public ArticleTestFixture {
  CPPUNIT_TEST_SUITE(GetArticleTest);
  CPPUNIT_TEST(testWrongNewsgroup);
  CPPUNIT_TEST(testWrongArticle);
  CPPUNIT_TEST(testRight);
  CPPUNIT_TEST_SUITE_END();
public:
  void testWrongNewsgroup() {
    Article_t article;
    CPPUNIT_ASSERT(pDatabase->getArticle(newsgroup.id + 1, 0, article) == STATUS_FAILURE_N_DOES_NOT_EXIST);
  }
  void testWrongArticle() {
    Article_t article;
    CPPUNIT_ASSERT(pDatabase->getArticle(newsgroup.id, 0, article) == STATUS_FAILURE_A_DOES_NOT_EXIST);
  }
  void testRight() {
    Article_t article;
    ArticleList_t articleList;
    article.title = "1984";
    article.author = "George Orwell";
    article.text = "Big brother ...";
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createArticle(newsgroup.id, article)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->listArticles(newsgroup.id, articleList)));
    CPPUNIT_ASSERT(articleList.size() == 1);
    article = articleList.front();
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->getArticle(newsgroup.id, article.id, article)));
    CPPUNIT_ASSERT(article.title == "1984");
    CPPUNIT_ASSERT(article.author == "George Orwell");
    CPPUNIT_ASSERT(article.text == "Big brother ...");
  }
};

int main(int argc, char* argv[])
{
  CppUnit::TestResult result;
  CppUnit::TextTestRunner runner;
  bool success;
  
  if (argc == 2) {
    if (strcmp("mem", argv[1]) == 0) {
      std::cout << "Using memory database" << std::endl;
      UseMemoryDatabase = true;
    } else if (strcmp("fs", argv[1]) == 0) {
      std::cout << "Using filesystem database" << std::endl;
      UseMemoryDatabase = false;
    } else {
      std::cerr << "usage: test-database [ mem | fs ]" << std::endl;
      return 1;
    }
  } else {
    std::cerr << "usage: test-database [ mem | fs ]" << std::endl;
    return 1;
  }

  CPPUNIT_TEST_SUITE_REGISTRATION(CreateNewsgroupTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(ListNewsgroupsTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(DeleteNewsgroupTest);

  CPPUNIT_TEST_SUITE_REGISTRATION(CreateArticleTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(ListArticlesTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(DeleteArticleTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(GetArticleTest);

  CppUnit::Test* test =
    CppUnit::TestFactoryRegistry::getRegistry().makeTest();
  
  runner.addTest(test);
  success = runner.run();
  
  return (success) ? 0 : 1;
}
