/**
 * @file test-database.cc
 *
 * Tests the database implementations.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "fusenet-types.h"
#include "memory-database.h"

using namespace fusenet;

class DatabaseTestFixture : public CppUnit::TestFixture {
protected:
  Database* pDatabase;
public:
  void setUp() {
    pDatabase = new MemoryDatabase();
    CPPUNIT_ASSERT(pDatabase != NULL);
  }
  void tearDown() {
    CPPUNIT_ASSERT(pDatabase != NULL);
    delete pDatabase;
  }
};

class CreateNewsgroupTest : public DatabaseTestFixture {
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

class ListNewsgroupsTest : public DatabaseTestFixture {
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

class DeleteNewsgroupTest : public DatabaseTestFixture {
  CPPUNIT_TEST_SUITE(DeleteNewsgroupTest);
  CPPUNIT_TEST(testEmpty1);
  CPPUNIT_TEST(testEmpty2);
  CPPUNIT_TEST(testSingle);
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
  void testNonEmpty() {
    std::string name("foo");
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->createNewsgroup(name)));
    CPPUNIT_ASSERT(IS_SUCCESS(pDatabase->deleteNewsgroup(-1)));
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
};

int main(void)
{
  CppUnit::TestResult result;
  CppUnit::TextTestRunner runner;
  bool success;

  CPPUNIT_TEST_SUITE_REGISTRATION(CreateNewsgroupTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(ListNewsgroupsTest);
  CPPUNIT_TEST_SUITE_REGISTRATION(DeleteNewsgroupTest);
  
  CppUnit::Test* test =
    CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  runner.addTest(test);
  success = runner.run();

  return (success) ? 0 : 1;
}

