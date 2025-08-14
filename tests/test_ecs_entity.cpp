#include <functional>

#include <gtest/gtest.h>

#include <SECSY/ECS/Entity.hpp>

using namespace SECSY;

class EntityTest : public ::testing::Test {
 protected:
  Entity m_default_entity{};
  Entity m_valid_entity{42, 3};

  void SetUp() override {
    m_default_entity = Entity{};
    m_valid_entity   = Entity{42, 3};
  }

  void TearDown() override {
    // Called after each test
  }
};

TEST_F(EntityTest, DefaultEntityIsInvalid) {
  EXPECT_FALSE(m_default_entity.IsValid());
}

TEST_F(EntityTest, ValidEntityIsValid) {
  EXPECT_TRUE(m_valid_entity.IsValid());
}

TEST_F(EntityTest, EqualityOperatorsWork) {
  Entity e1{10, 2};
  Entity e2{10, 2};
  Entity e3{10, 3};

  EXPECT_EQ(e1, e2);
  EXPECT_NE(e1, e3);
}

TEST_F(EntityTest, HashMatchesEqualEntities) {
  Entity e1{100, 5};
  Entity e2{100, 5};

  auto h1 = std::hash<Entity>{}(e1);
  auto h2 = std::hash<Entity>{}(e2);

  EXPECT_EQ(h1, h2);
}

TEST_F(EntityTest, LessOperatorWorks) {
  Entity e1{1, 1};
  Entity e2{2, 1};

  EXPECT_TRUE(std::less<Entity>{}(e1, e2));
}