#ifndef DASH__TEST__DART_LOCALITY_TEST_H_
#define DASH__TEST__DART_LOCALITY_TEST_H_

#include "TestBase.h"

#include <gtest/gtest.h>
#include <libdash.h>

/**
 * Test fixture for onesided operations provided by DART.
 */
class DARTLocalityTest : public ::testing::Test {
protected:
  size_t _dash_id;
  size_t _dash_size;

  DARTLocalityTest()
  : _dash_id(0),
    _dash_size(0) {
    LOG_MESSAGE(">>> Test suite: DARTLocalityTest");
  }

  virtual ~DARTLocalityTest() {
    LOG_MESSAGE("<<< Closing test suite: DARTLocalityTest");
  }

  virtual void SetUp() {
    dash::init(&TESTENV.argc, &TESTENV.argv);
    _dash_id   = dash::myid();
    _dash_size = dash::size();
    LOG_MESSAGE("===> Running test case with %ld units ...",
                _dash_size);
  }

  virtual void TearDown() {
    dash::Team::All().barrier();
    LOG_MESSAGE("<=== Finished test case with %ld units",
                _dash_size);
    dash::finalize();
  }
};

#endif // DASH__TEST__DART_LOCALITY_TEST_H_
