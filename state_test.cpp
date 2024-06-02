#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "state.h"

// Test default constructor
TEST_CASE("Default constructor") {
    State s;
    CHECK(s.getStateName() == " ");
    CHECK(s.isFinal() == false);
    CHECK(s.isStart() == false);
}

// Test parameterized constructor
TEST_CASE("Parameterized constructor") {
    State s("TestState", true, false);
    CHECK(s.getStateName() == "TestState");
    CHECK(s.isFinal() == true);
    CHECK(s.isStart() == false);
}

// Test copy constructor
TEST_CASE("Copy constructor") {
    State s1("TestState", true, false);
    State s2(s1);
    CHECK(s2.getStateName() == "TestState");
    CHECK(s2.isFinal() == true);
    CHECK(s2.isStart() == false);
}

// Test move constructor
TEST_CASE("Move constructor") {
    State s1("TestState", true, false);
    State s2(std::move(s1));
    CHECK(s2.getStateName() == "TestState");
    CHECK(s2.isFinal() == true);
    CHECK(s2.isStart() == false);
    CHECK(s1.getStateName() == ""); // Moved-from state should be empty or invalid state
}

// Test copy assignment operator
TEST_CASE("Copy assignment operator") {
    State s1("TestState", true, false);
    State s2;
    s2 = s1;
    CHECK(s2.getStateName() == "TestState");
    CHECK(s2.isFinal() == true);
    CHECK(s2.isStart() == false);
}

// Test move assignment operator
TEST_CASE("Move assignment operator") {
    State s1("TestState", true, false);
    State s2;
    s2 = std::move(s1);
    CHECK(s2.getStateName() == "TestState");
    CHECK(s2.isFinal() == true);
    CHECK(s2.isStart() == false);
    CHECK(s1.getStateName() == ""); // Moved-from state should be empty or invalid state
}

// Test setStateName with valid name
TEST_CASE("Set valid state name") {
    State s;
    s.setStateName("ValidState");
    CHECK(s.getStateName() == "ValidState");
}

// Test setStateName with invalid name
TEST_CASE("Set invalid state name") {
    State s;
    CHECK_THROWS_AS(s.setStateName("Invalid Name!"), const char*);
}

// Test equality operator
TEST_CASE("Equality operator") {
    State s1("TestState", true, false);
    State s2("TestState", true, false);
    CHECK(s1 == s2);
}

// Test input operator
TEST_CASE("Input operator") {
    std::istringstream iss("TestState [1,0]");
    State s;
    iss >> s;
    CHECK(s.getStateName() == "TestState");
    CHECK(s.isFinal() == true);
    CHECK(s.isStart() == false);
}

// Test output operator
TEST_CASE("Output operator") {
    State s("TestState", true, false);
    std::ostringstream oss;
    oss << s;
    CHECK(oss.str() == "TestState [1,0]");
}