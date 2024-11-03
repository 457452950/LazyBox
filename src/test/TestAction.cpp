#include "lazybox/test/TestAction.hpp"

#include "lazybox/test/TestEngine.hpp"

namespace lbox {
namespace test {

PreAction::PreAction() { TestEngine::GetInstance()->AddPreAction(this); }

EndAction::EndAction() { TestEngine::GetInstance()->AddEndAction(this); }

} // namespace test
} // namespace lbox
