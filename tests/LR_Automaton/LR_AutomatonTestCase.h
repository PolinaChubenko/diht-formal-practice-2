#pragma once

#include <gtest/gtest.h>
#include "LR_AutomatonMock.h"


class LR_AutomatonTestCase: public ::testing::Test {
public:
    LR_AutomatonMock mock_obj;
};
