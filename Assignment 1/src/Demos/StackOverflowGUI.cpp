#include "StackOverflowGUI.h"
#include "StackOverflow.h"
#include "goptionpane.h"
using namespace std;

namespace {
    const string kWarningMessage =
R"(Warning! This is going to crash your program.
Make sure you ran your program using the "Debug" option,
the green triangle with the little bug icon on it.)";
}

StackOverflowGUI::StackOverflowGUI(GWindow &) {
    GOptionPane::showMessageDialog(kWarningMessage, "Warning!");
    triggerStackOverflow(137);
}

shared_ptr<ProblemHandler> makeStackOverflowGUI(GWindow& window) {
    return make_shared<StackOverflowGUI>(window);
}
