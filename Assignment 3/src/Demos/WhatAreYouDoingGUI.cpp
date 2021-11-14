#include "ProblemHandler.h"
#include "TemporaryComponent.h"
#include "GColorConsole.h"
#include "WhatAreYouDoing.h"
#include "set.h"
#include "ginteractors.h"
#include "tokenscanner.h"
#include <cctype>
using namespace std;

namespace {
    class WhatAreYouDoingGUI: public ProblemHandler {
    public:
        WhatAreYouDoingGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;

    private:
        Temporary<GColorConsole> mConsole;
        Temporary<GLabel>        mDescription;
        Temporary<GTextField>    mInput;
        Temporary<GButton>       mGo;
        Temporary<GButton>       mClear;
    };

    /* Toggles the case of a string. */
    string toggleCaseIn(string result) {
        for (char& ch: result) {
            if (isalpha(ch)) {
                if (isupper(ch)) ch = tolower(ch);
                else ch = toupper(ch);

            }
        }
        return result;
    }
}

WhatAreYouDoingGUI::WhatAreYouDoingGUI(GWindow& window) {
    mConsole     = Temporary<GColorConsole>(new GColorConsole(), window, "CENTER");
    mDescription = Temporary<GLabel>(new GLabel("Enter sentence: "), window, "SOUTH");
    mInput       = Temporary<GTextField>(new GTextField(""), window, "SOUTH");
    mGo          = Temporary<GButton>(new GButton("Go!"), window, "SOUTH");
    mClear       = Temporary<GButton>(new GButton("Clear"), window, "WEST");
}

void WhatAreYouDoingGUI::actionPerformed(GObservable* source) {
    if (source == mGo || source == mInput) {
        string input = mInput->getText();

        Set<string> result([](const string& lhs, const string& rhs) {
            /* Reverse the case of the strings before comparing, so that
             * lower case precedes upper-case.
             */
            return toggleCaseIn(lhs) < toggleCaseIn(rhs);
        });
        for (string option: allEmphasesOf(input)) {
            result += option;
        }

        mConsole->doWithStyle("#000080", GColorConsole::BOLD_ITALIC, [&] {
            *mConsole << "All emphases of \"" << input << "\":" << '\n';
        });
        for (string option: result) {
            *mConsole << "  " << option << '\n';
        }
        *mConsole << endl;
    } else if (source == mClear) {
        mConsole->clear();
    }
}

shared_ptr<ProblemHandler> makeWhatAreYouDoingGUI(GWindow& window) {
    return make_shared<WhatAreYouDoingGUI>(window);
}

Vector<string> tokenize(const string& str) {
    Vector<string> result;

    /* The TokenScanner does most, but not all, of what we want. Specifically, we can
     * either glob together strings that include single quotes or have them broken
     * apart. We'll include quotation marks as part of the string, and then if the
     * string appears to be something in single quotes we'll break it apart.
     */
    TokenScanner scanner(str);
    scanner.addWordCharacters("'");

    while (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        if (token.size() > 1 &&
            token.front() == '\'' &&
            token.back()  == '\'') {
            result += "'";
            result += token.substr(1, token.size() - 2);
            result += "'";
        } else {
            result += token;
        }
    }

    return result;
}
