#include <gtest/gtest.h>

#include "Configure.h"



TEST(ConfigureTest, addItem)
{
    Configure* pc = new Configure();

    ASSERT_TRUE(pc != NULL);

    pc->addItem("A");
    pc->addItem("B");
    EXPECT_EQ(pc->addItem("A"), 0);
    // EXPECT_EQ(pc->addItem("A"), 1);

    EXPECT_EQ(pc->getSize(), 2);
    EXPECT_STREQ(pc->getItem(0).c_str(), "A");
    EXPECT_STREQ(pc->getItem(1).c_str(), "B");
    EXPECT_STREQ(pc->getItem(10).c_str(), "");

    delete pc;
}

TEST(ConfigureTest, getSize)
{
    Configure* pc = new Configure();

    ASSERT_TRUE(pc != NULL);

    EXPECT_EQ(pc->getSize(), 0);

    delete pc;
}