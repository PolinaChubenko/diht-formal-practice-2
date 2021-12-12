#pragma once

#include <gtest/gtest.h>
#include "EarleyMock.h"

class EarleyTestCase: public ::testing::Test {
public:
    EarleyMock mock_obj;
};