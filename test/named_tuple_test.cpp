/**
 * @file named_tuple_test.cpp
 * @authors Giulio Romualdi
 * @copyright This software may be modified and distributed under the terms of the BSD-3-Clause
 * license.
 */

#include <string>
#include <tuple>
#include <vector>

// Catch2
#include <catch2/catch.hpp>

#include <named_tuple/named_tuple.h>

using namespace ntuple;

class DummyClass
{
public:
    using State = named_tuple<named_param<detail::sid_hash("name"), std::string>,
                              named_param<detail::sid_hash("number"), int>,
                              named_param<detail::sid_hash("floating_number"), double>,
                              named_param<detail::sid_hash("vector"), std::vector<double>>>;

    const State& getState() const
    {
        return m_state;
    }

    State& getState()
    {
        return m_state;
    }

    void setState(const State& state)
    {
        m_state = state;
    }

private:

    State m_state;
};

TEST_CASE("Test Named Parameter")
{
    using namespace ntuple::literals;
    named_param<"name"_h, std::string> param;

    param = "Olivia";
}

TEST_CASE("Test Named Tuple - Tuple operator")
{
    using namespace ntuple::literals;
    DummyClass dummyClass;

    dummyClass.setState(std::tuple<std::string, int, double, std::vector<double>> //
                        {"Elisa", 12, 1.231, {1.2, 32.13}});

    REQUIRE(std::get<0>(dummyClass.getState()) == "Elisa");
    REQUIRE(dummyClass.getState().get_from_hash<"name"_h>() == "Elisa");

    const DummyClass& dummyClassRef = dummyClass;
    REQUIRE(std::get<0>(dummyClassRef.getState()) == "Elisa");
    REQUIRE(dummyClassRef.getState().get_from_hash<"name"_h>() == "Elisa");
}

TEST_CASE("Test NamedTuple")
{
    using namespace ntuple::literals;

    auto foo = make_named_tuple(named_param<"name"_h, std::string>(),
                                named_param<"number"_h, int>(),
                                named_param<"floating_number"_h, double>(),
                                named_param<"vector"_h, std::vector<double>>());

    SECTION("Get")
    {
        auto& [a, b, c, d] = foo;
        a = "Smith";
        b = 150;
        c = 3.14;
        d = {12.34, 21.3, 235.123123};

        REQUIRE(a == foo.get_from_hash<"name"_h>());
        REQUIRE(std::get<0>(foo) == foo.get_from_hash<"name"_h>());

        REQUIRE(b == foo.get_from_hash<"number"_h>());
        REQUIRE(std::get<1>(foo) == foo.get_from_hash<"number"_h>());

        REQUIRE(c == foo.get_from_hash<"floating_number"_h>());
        REQUIRE(std::get<2>(foo) == foo.get_from_hash<"floating_number"_h>());

        REQUIRE(d == foo.get_from_hash<"vector"_h>());
        REQUIRE(std::get<3>(foo) == foo.get_from_hash<"vector"_h>());
    }

    SECTION("Set")
    {
        foo.get_from_hash<"name"_h>() = "Olivia";
        REQUIRE(std::get<0>(foo) == foo.get_from_hash<"name"_h>());

        foo.get_from_hash<"number"_h>() = 42;
        REQUIRE(std::get<1>(foo) == foo.get_from_hash<"number"_h>());

        foo.get_from_hash<"floating_number"_h>() = 8.15;
        REQUIRE(std::get<2>(foo) == foo.get_from_hash<"floating_number"_h>());

        foo.get_from_hash<"vector"_h>() = {1.3154, 24671.3, 235.1, 15641.294};
        REQUIRE(std::get<3>(foo) == foo.get_from_hash<"vector"_h>());
    }

    SECTION("From and to tuple")
    {
        auto tempTuple = foo.to_tuple();
        decltype(foo) newNamedTuple(tempTuple);

        decltype(foo) newNamedTuple2;
        newNamedTuple2 = tempTuple;
    }
}
