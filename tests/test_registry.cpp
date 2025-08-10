#include <cstdint>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <SECSY/ECS/Registry.hpp>

// Small helper component types
struct Position {
  int x, y;
};
struct Velocity {
  float dx, dy;
};
struct Tag {
  std::string name;
};

// Move-only component
struct MoveOnly {
  MoveOnly(int v) : v(v) {}
  MoveOnly(MoveOnly &&o) noexcept : v(o.v) {
    o.v = -1;
  }
  MoveOnly &operator=(MoveOnly &&o) noexcept {
    v   = o.v;
    o.v = -1;
    return *this;
  }
  MoveOnly(const MoveOnly &)            = delete;
  MoveOnly &operator=(const MoveOnly &) = delete;
  int v;
};

TEST(Entity_Basics, DefaultInitAndValidity) {
  SECSY::Entity e;
  // ---- EXPECTED: default ctor should zero-initialize fields ----
  // If this fails, ensure Entity::id and Entity::ver are initialized to 0.
  EXPECT_EQ(e.id, 0u);
  EXPECT_EQ(e.ver, 0u);
  EXPECT_FALSE(e.IsValid());
}

TEST(Entity_Basics, ParameterizedCtorEquality) {
  SECSY::Entity a{1, 1}, b{1, 1}, c{1, 2}, d{2, 1};
  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
  EXPECT_NE(a, d);
}

TEST(Entity_StdHashLess, WorksWithStdContainers) {
  SECSY::Entity a{1, 1}, b{2, 1}, c{1, 2};
  std::unordered_set<SECSY::Entity> s;
  s.insert(a);
  s.insert(b);
  s.insert(c);
  EXPECT_EQ(s.size(), 3u);

  std::set<SECSY::Entity> ordered;
  ordered.insert(c);
  ordered.insert(a);
  ordered.insert(b);
  // Should be ordered and not crash
  EXPECT_EQ(ordered.size(), 3u);
}

class RegistryFixture : public ::testing::Test {
 protected:
  SECSY::Registry reg;
};

TEST_F(RegistryFixture, CreateDestroyAndLiveness) {
  auto e = reg.Create();
  EXPECT_TRUE(reg.IsAlive(e));

  reg.Destroy(e);
  EXPECT_FALSE(reg.IsAlive(e));
}

TEST_F(RegistryFixture, ReuseIdIncrementsVersionAndNonZero) {
  // Create and destroy to push id into free list
  auto e1   = reg.Create();
  auto id   = e1.id;
  auto ver1 = e1.ver;
  reg.Destroy(e1);

  // Re-create -> should reuse id and increment version (non-zero)
  auto e2 = reg.Create();
  EXPECT_EQ(e2.id, id);

  // ---- EXPECTED: version increases and is non-zero ----
  // If this fails, the Create() reuse path has a bug (see ver initialization /
  // overflow handling).
  EXPECT_NE(e2.ver, ver1);
  EXPECT_NE(e2.ver, 0)
      << "version wrapped to zero; overflow-handling is incorrect";
}

TEST_F(RegistryFixture, EmplaceGetHasRemoveBasic) {
  auto e  = reg.Create();
  auto &p = reg.Emplace<Position>(e, 42, 84);
  EXPECT_EQ(p.x, 42);
  EXPECT_EQ(p.y, 84);
  EXPECT_TRUE(reg.Has<Position>(e));

  auto &p2 = reg.Get<Position>(e);
  EXPECT_EQ(&p, &p2);

  reg.Remove<Position>(e);
  EXPECT_FALSE(reg.Has<Position>(e));
  EXPECT_THROW(reg.Get<Position>(e), std::out_of_range);
}

TEST_F(RegistryFixture, GetThrowsWhenNoStorage) {
  auto e = reg.Create();
  // We never emplaced a Velocity; Get should throw because there is no storage
  EXPECT_THROW(reg.Get<Velocity>(e), std::out_of_range);
}

TEST_F(RegistryFixture, OverwriteComponentWithEmplace) {
  auto e   = reg.Create();
  auto &p1 = reg.Emplace<Position>(e, 1, 2);
  EXPECT_EQ(p1.x, 1);
  EXPECT_EQ(p1.y, 2);

  auto &p2 = reg.Emplace<Position>(e, 5, 6);  // overwrite path
  EXPECT_EQ(&p1, &p2);
  EXPECT_EQ(p2.x, 5);
  EXPECT_EQ(p2.y, 6);
}

TEST_F(RegistryFixture, MultipleComponentTypesIndependence) {
  auto e = reg.Create();
  reg.Emplace<Position>(e, 10, 20);
  reg.Emplace<Velocity>(e, 0.1f, 0.2f);
  reg.Emplace<Tag>(e, "player");

  EXPECT_TRUE(reg.Has<Position>(e));
  EXPECT_TRUE(reg.Has<Velocity>(e));
  EXPECT_TRUE(reg.Has<Tag>(e));

  auto &pos = reg.Get<Position>(e);
  auto &vel = reg.Get<Velocity>(e);
  auto &tag = reg.Get<Tag>(e);

  EXPECT_EQ(pos.x, 10);
  EXPECT_EQ(tag.name, "player");
  EXPECT_FLOAT_EQ(vel.dx, 0.1f);
}

TEST_F(RegistryFixture, MoveOnlyComponentWorks) {
  auto e = reg.Create();
  // Should support move-only components in Emplace
  auto &m = reg.Emplace<MoveOnly>(e, 123);
  EXPECT_EQ(m.v, 123);
}

TEST_F(RegistryFixture, ComponentStorageIsolationBetweenTypes) {
  auto e1 = reg.Create();
  auto e2 = reg.Create();

  reg.Emplace<Position>(e1, 1, 1);
  reg.Emplace<Velocity>(e2, 2.0f, 3.0f);

  EXPECT_TRUE(reg.Has<Position>(e1));
  EXPECT_FALSE(reg.Has<Position>(e2));
  EXPECT_TRUE(reg.Has<Velocity>(e2));
  EXPECT_FALSE(reg.Has<Velocity>(e1));
}

TEST_F(RegistryFixture, DestroyDoesNotImplicitlyRemoveComponents) {
  auto e = reg.Create();
  reg.Emplace<Position>(e, 9, 9);
  reg.Destroy(e);

  // Current implementation leaves components in storage (design choice).
  // This test codifies that current behavior: Get/Has still succeed.
  // If you instead want components to be removed on Destroy, change test to
  // EXPECT_FALSE / EXPECT_THROW.
  EXPECT_TRUE(reg.Has<Position>(e))
      << "components were removed on destroy; was that intended?";
  EXPECT_NO_THROW({
    auto &p = reg.Get<Position>(e);
    (void)p;
  });
}

TEST_F(RegistryFixture, TypeIdUniqueness) {
  // TypeId<T> must be unique per-type.
  auto idPos = Internal::TypeId<Position>();
  auto idVel = Internal::TypeId<Velocity>();
  auto idTag = Internal::TypeId<Tag>();

  EXPECT_NE(idPos, idVel);
  EXPECT_NE(idPos, idTag);
  EXPECT_NE(idVel, idTag);
}

TEST_F(RegistryFixture, EnsureMultipleStoragesCoexist) {
  // Ensure storing multiple component types creates separate storages
  auto e = reg.Create();
  reg.Emplace<Position>(e, 7, 8);
  reg.Emplace<Velocity>(e, 1.5f, -2.5f);

  // mutate Position, ensure Velocity unaffected
  reg.Get<Position>(e).x = 33;
  EXPECT_EQ(reg.Get<Position>(e).x, 33);
  EXPECT_FLOAT_EQ(reg.Get<Velocity>(e).dx, 1.5f);
}

TEST_F(RegistryFixture, RemovingMissingComponentIsNoop) {
  auto e = reg.Create();
  // Should not throw or crash
  EXPECT_NO_THROW(reg.Remove<Position>(e));
}
