/* File: OnlyConnect.cpp
 *
 * TODO: Edit these comments to describe anything interesting or noteworthy in your implementation.
 *
 * TODO: Edit these comments to leave a puzzle for your section leader to solve!
 */
#include "OnlyConnect.h"
#include "Testing/OnlyConnectTests.h"
#include<cctype>
#include"strlib.h"
using namespace std;
const string CONSONANTS_LOWER = "bcdfghjklmnpqrstvwxz";
//const string CONSONANTS_UPPER = "BCDFGHJKLMNPQRSTVWXZ";
bool is_consonant(const char& c);
string onlyConnectize(string phrase) {
    /* TODO: The next few lines just exist to make sure you don't get compiler warning messages
     * when this function isn't implemented. Delete these lines, then implement this function.
     */
    /*(void) phrase;
    string result="";
    for(auto p :phrase){
        if(is_consonant(p)){
            result+=toUpperCase(p);
        }
    }*/
//    RECURSION CASE
//    BASE CASE
//    if(phrase=="")  return "";
    if(phrase.size()==1||phrase.size()==0){
        if(is_consonant(phrase[0])){
            string tmp="";
            return tmp+toUpperCase(phrase[0]);
        }
        else{
            return "";
        }
    }
//    recursion case

    return onlyConnectize(phrase.substr(0,1))+onlyConnectize(phrase.substr(1));
}
bool is_consonant(const char& c){
    if(!isalpha(c)) return false;
    auto char_lower=tolower(c);
    for(auto consonant : CONSONANTS_LOWER){
        if(char_lower==consonant){
            return true;
        }
    }
    return false;
}





/* * * * * * Test Cases * * * * * */


ADD_TEST("Test the consonat function") {
    EXPECT_EQUAL(is_consonant('!'), false);
}
ADD_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

ADD_TEST("Handles non-letter characters properly.") {
    EXPECT_EQUAL(onlyConnectize("2.718281828459045"), "");
    EXPECT_EQUAL(onlyConnectize("'Hi, Mom!'"), "HMM");
}

ADD_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("+"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}
ADD_TEST("my own extreme test.") {
    EXPECT_EQUAL(onlyConnectize(""), "");
    EXPECT_EQUAL(onlyConnectize("989237482789748278478787482974cnhw iowhuhds*&*^&*%^&%^"), "CNHWWHHDS");
}
/* TODO: You will need to add your own tests into this suite of test cases. Think about the sorts
 * of inputs we tested here, and, importantly, what sorts of inputs we *didn't* test here. Some
 * general rules of testing:
 *
 *    1. Try extreme cases. What are some very large cases to check? What are some very small cases?
 *
 *    2. Be diverse. There are a lot of possible inputs out there. Make sure you have tests that account
 *       for cases that aren't just variations of one another.
 *
 *    3. Be sneaky. Don't just try standard inputs. Try weird ones that you wouldn't expect anyone to
 *       actually enter, but which are still perfectly legal.
 *
 * Happy testing!
 */






