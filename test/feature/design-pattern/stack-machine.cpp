/**
 *
 */
#include <gtest/gtest.h>

#include <patterns/stack-machine.hpp>

/**
 * @class state_fixture 
 * @brief a fixture for state related tests
 *
 */
class state_fixture : public ::testing::Test {
protected:

    void SetUp() override {
        // Optional setup code before each test
    }

    void TearDown() override {
        // Optional teardown code after each test
    }
};

// Test case to check the add function
TEST_F(state_fixture, validate_null_state ) {
    using namespace softjenga::patterns::stack_machine;
    null_state state;
    auto expected_error_code = std::make_error_code(errc::null_state);
    EXPECT_EQ(state.on_enter(), expected_error_code);
}

// Test fixture for testing the add function
class Fixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Optional setup code before each test
    }

    void TearDown() override {
        // Optional teardown code after each test
    }
};

// Test case to check the add function

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

