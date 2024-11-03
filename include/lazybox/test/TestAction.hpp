#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTACTION_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTACTION_HPP_


namespace lbox {
namespace test {

class TestAction {
public:
    virtual void Do() = 0;

             TestAction() = default;
    virtual ~TestAction() = default;
};


class PreAction : public TestAction {
public:
     PreAction();
    ~PreAction() override = default;
};

class EndAction : public TestAction {
public:
     EndAction();
    ~EndAction() override = default;
};

} // namespace test
} // namespace lbox


#endif // LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTACTION_HPP_
