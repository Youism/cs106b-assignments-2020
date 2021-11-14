#include "YouGotHufflepuff.h"
#include "Testing/YouGotHufflepuffTests.h"
#include"hashset.h"
#include<cmath>
using namespace std;
/*
 * return a score for each option
 * param factor
 * param strengh
*/
int get_score(int factor,int strengh);
/*
*  return the cofficient
*/
double get_close(const HashMap<char,int>& user_score,
                 const HashMap<char,int>& people);

void administerQuiz(const HashSet<Question>& questions,
                    int numQuestions,
                    const HashSet<Person>& people) {
    HashMap<char,int> score{{'O',0},{'C',0},{'E',0},{'A',0},{'N',0}};
    HashSet<Question> questions_copy=questions;
    while(numQuestions--){
        auto question=randomElement(questions_copy);
        auto strength=askPersonalityQuestion(question);
        auto factor=question.factors;
        for(auto key:factor){
            score[key]+=get_score(factor[key],strength);
        }
        questions_copy.remove(question);
    }
    displayScores(score);
    HashMap<double,string> dict;
    double maximum=0;
    for(auto p:people){
        auto similarity=get_close(score,p.scores);
        dict[similarity]=p.name;
    }
    for(auto v:dict.keys())  maximum=maximum>v?maximum:v;
    string msg="你和"+dict[maximum]+"最相似！";
    displayMessage(msg);


}


int get_score(int factor,int strengh){
    if(factor==-1){
        return -(strengh-3);
    }
    else if(factor==1){
        return strengh-3;
    }
    else
        return 0;
}

double get_close(const HashMap<char,int>& user_score,
                 const HashMap<char,int>& people){
    auto user_values=user_score.values();
    auto meaningful=false;
    for(auto v :user_values){
        if(v!=0) meaningful=true;
    }
    if(!meaningful) {displayMessage("the user input is meaningless!"); return 0;}
// step 1 standardize
// step 2 calculate the result
    auto people_values=people.values();
    double user_squre=0.0;
    double people_squre=0.0;
    for(int i=0;i<5;++i){
        user_squre+=user_values[i]*user_values[i];
        people_squre+=people_values[i]*people_values[i];
    }
    auto sqrt_result_user=sqrt(user_squre);
    auto sqrt_result_people=sqrt(people_squre);
    double similarity=0.0;
    for(int i=0;i<5;++i){
        user_values[i]/=sqrt_result_user;
        people_values[i]/=sqrt_result_people;
        similarity+=user_values[i]*people_values[i];
    }
    return similarity;

}
ADD_TEST("Get score test"){
    EXPECT_EQUAL(get_score(-1,5),-2);
}
/* * * * *   Test Cases Below This Point   * * * * */

/* Because administerQuiz is a function that requires interaction with the user,
 * you won't be able to directly test that administerQuiz returns the correct
 * values. Similarly, you can't call the askPersonalityQuestion, displayScores,
 * or displayMessage functions, as those are wired into the demo and don't
 * work in testing.
 *
 * However, you are welcome to add test cases to test any helper functions you
 * write! To do so, use the ADD_TEST macro to define tests, just as you did in
 * Assignment 1 and for Rising Tides.
 *
 * You aren't required to include test cases in this part of the assignment,
 * though we definitely would encourage you to do so! This is a multistep
 * assignment that's going to require each individual function to work
 * correctly, and adding tests for the functions you write is a great way to
 * find and fix bugs early on.
 */

