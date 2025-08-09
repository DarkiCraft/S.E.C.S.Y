#include <cassert>
#include <string>
#include <vector>
#include "FlatSet.hpp"

using namespace SECSY;

void test_initially_empty() {
  FlatSet<int, std::string> set;
  assert(set.empty());
  assert(set.size() == 0);
}

void test_emplace_inserts_and_returns_reference() {
  FlatSet<int, std::string> set;
  auto& ref = set.Emplace(5, "hello");
  assert(ref == "hello");
  assert(set.size() == 1);
  assert(set.At(5) == "hello");
}

void test_emplace_does_not_overwrite_existing() {
  FlatSet<int, std::string> set;
  set.Emplace(1, "first");
  auto& ref = set.Emplace(1, "second");
  assert(ref == "first");  // Should not overwrite
  assert(set.size() == 1);
}

void test_insert_behavior() {
  FlatSet<int, std::string> set;
  assert(set.Insert(1, "one"));
  assert(set.size() == 1);
  assert(!set.Insert(1, "new"));  // duplicate
  assert(set.At(1) == "one");
}

void test_erase_behavior() {
  FlatSet<int, std::string> set;
  set.Insert(1, "one");
  assert(set.Erase(1));
  assert(set.empty());
  assert(!set.Erase(42));  // not found
}

void test_find_and_contains() {
  FlatSet<int, std::string> set;
  set.Insert(1, "one");
  assert(set.Find(1) != nullptr);
  assert(*set.Find(1) == "one");
  assert(set.Find(42) == nullptr);
  assert(set.Contains(1));
  assert(!set.Contains(42));
}

void test_operator_bracket() {
  FlatSet<int, std::string> set;
  auto& val = set[5];
  assert(val == "");  // default-inserted
  assert(set.Contains(5));

  set[5] = "updated";
  assert(set[5] == "updated");
}

void test_at_throws() {
  FlatSet<int, std::string> set;
  bool threw = false;
  try {
    set.At(42);
  } catch (const std::out_of_range&) {
    threw = true;
  }
  assert(threw);
}

void test_iteration_sorted() {
  FlatSet<int, int> set;
  set.Insert(3, 30);
  set.Insert(1, 10);
  set.Insert(2, 20);

  std::vector<int> keys;
  for (auto& p : set) keys.push_back(p.first);

  assert((keys == std::vector<int>{1, 2, 3}));
}

void test_clear() {
  FlatSet<int, int> set;
  set.Insert(1, 100);
  set.Insert(2, 200);
  set.Clear();
  assert(set.empty());
  assert(set.size() == 0);
}

int main() {
  test_initially_empty();
  test_emplace_inserts_and_returns_reference();
  test_emplace_does_not_overwrite_existing();
  test_insert_behavior();
  test_erase_behavior();
  test_find_and_contains();
  test_operator_bracket();
  test_at_throws();
  test_iteration_sorted();
  test_clear();
  return 0;  // success
}
