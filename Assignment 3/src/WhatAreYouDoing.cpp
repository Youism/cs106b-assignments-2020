/*
 * File: WhatAreYouDoing.cpp
 * ---------------
 * This program will take a sentence as input, and generate all the possible combinations in which different words of the
 * sentence are capitalized.
 */

#include "WhatAreYouDoing.h"
#include "Testing/WhatAreYouDoingTests.h"
#include "strlib.h"
using namespace std;



bool isAllPunctuation(const string&s){
    auto isAll=true;
    for(auto i :s){
        if(isalpha(i)){
            isAll=false;
        }
    }
    return isAll;
}
/* Function: emphasesRec
 * -----------------------
 * emphasesRec takes as input two strings: sentence and chosen. Sentence is the part of the string that we haven't explored yet,
 * while chosen is the part of the string that has been explored in previous recursion calls. The recursive function looks at
 * a string and returns a HashSet with all combinations of words capitalized in that string.
 */
HashSet<string> emphasesRec(const string& sentence, const string& chosen) {
    HashSet<string> result={};
//    if(isAllPunctuation(sentence)) return {sentence};
    if(sentence=="") return {chosen};

    auto first=tokenize(sentence).front();
    auto choose_first=toUpperCase(first);
    auto not_choose_first=toLowerCase(first);

    auto remain=sentence.substr(first.size());
    auto left=emphasesRec(remain,chosen+choose_first);
    auto right=emphasesRec(remain,chosen+not_choose_first);

    result.unionWith(left);
    result.unionWith(right);
    return result;
}

/* Function: allEmphasesOf
 * -----------------------
 * This is a wrapper function that takes as an argument a sentence and calls emphasesRec with that sentence and with an empty string,
 * which is the first call for "chosen" in the emphasesRec function
 */
HashSet<string> allEmphasesOf(const string& sentence) {
    return emphasesRec(sentence, "");
}

/* * * * * * Test Cases * * * * * */

ADD_TEST("User test: returns an empty string if argument is empty") {
    HashSet<string> expected = {""};
    EXPECT_EQUAL(allEmphasesOf(""), expected);
}

ADD_TEST("User test: Enumerates options correctly even when starting with a non letter character") {
    HashSet<string> expected = {
        "/n stanford",
        "/n STANFORD",
        "/N stanford",
        "/N STANFORD"
    };
    EXPECT_EQUAL(allEmphasesOf("/n StanFOrd"), expected);
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

ADD_TEST("Provided Test: Enumerates all options in a simple case.") {
    HashSet<string> expected = {
        "hello",
        "HELLO",
    };

    EXPECT_EQUAL(allEmphasesOf("Hello"), expected);
}

ADD_TEST("Provided Test: Each option has the right length.") {
    string sentence = "Hello, world!";
    for (string option: allEmphasesOf(sentence)) {
        EXPECT_EQUAL(option.length(), sentence.length());
    }
}

ADD_TEST("Provided Test: Enumerates all options in a more typical case.") {
    HashSet<string> expected = {
        "you are?",
        "you ARE?",
        "YOU are?",
        "YOU ARE?"
    };

    EXPECT_EQUAL(allEmphasesOf("You Are?"), expected);
}

//ADD_TEST("Provided Test: Warning! This next test will hang if you produce duplicate capitalizations.") {
//    // Nothing to see here, folks. Move along!
//}

//ADD_TEST("Provided Test: Recursion doesn't branch on non-word symbols.") {
//    string punctuation(50, '*'); // 50 copies of *

//    /* The only emphasis is itself. */
//    HashSet<string> expected = {
//        punctuation
//    };

//    EXPECT_EQUAL(allEmphasesOf(punctuation), expected);
//}
