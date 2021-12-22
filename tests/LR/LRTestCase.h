#pragma once

#include <gtest/gtest.h>
#include "LRMock.h"


class LRTestCase: public ::testing::Test {
public:
    LRMock mock_obj;
};

