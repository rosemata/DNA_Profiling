//
//  driver.h
//  data processing framework that does all that score tabulating
//
//  Created by Roselle Mata
//  Copyright © 2020 Roselle Mata. All rights reserved.
//

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

// This function returns random element from set questions

Question randomQuestionFrom(set<Question>& questions) {
    if (questions.size() == 0) {
        throw runtime_error("set: empty");
    }

    Question random;    // create a question object
    random = randomElement(questions);  // call randomElement
    questions.erase(random); 
    return random;
}

// Takes map as parameters, calculate values of the map and return it.

map<char, int> scoresFrom(map<Question, int>& answers) {
    // crete a map of <char, int>
    map<char, int> scores;

    // loop through all the pairs in asnwers

    for(pair<Question, int> p : answers) {
        for (pair<char, int> q : p.first.factors) { 

            // calculation

            scores[q.first] += q.second *(p.second-3); 
        }
    }
    return scores; 
}

// This function takes map as parameters, and calculates the map value's cosine similarity

map<char, double> normalize(map<char, int>& scores) {

    map<char, double> normalizeNum;
    double total = 0;

    // for each, raise to pow 2, then add to total

    for(pair<char, int> p : scores) { 
        total = total + (pow(p.second,2)); 
    }

    total = sqrt(total); 

    if (total == 0) {
        throw runtime_error("divide by zero"); 
    }
    
    // for each divide to num = sqrt(num);

    for(pair<char, int> m : scores) {  

        double num2 = m.second / total;
        normalizeNum[m.first] = num2;
    }

    return normalizeNum;

}

// This function takes maps as parameters, and calculates the cosine similarity

double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {

    set <char> mySet;

    for(pair<char, double> p: lhs) { // loop though map lhs, then insert value to new map
        mySet.insert(p.first);
    }

    for(pair<char, double> p: rhs) { // loop though map rhs, then insert value to new map
        mySet.insert(p.first);
    }

    double similarity = 0;

    for (char x : mySet) { // calculatiorn
        if (lhs.find(x) != lhs.end() && rhs.find(x) != rhs.end()) {
            similarity = similarity + (lhs.at(x) * rhs.at(x));
        }
    }
    return similarity;
}

// Returns person scores have highest cosine similarity to user’s scores.

Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {

    if (people.size() == 0) {
        throw runtime_error("set: empty");
    }

    Person person1;
    map<char, double> userScore = normalize(scores);
    double maxSim = -1;
    double currSim = 0;

    for (Person currPerson : people) {
        map<char, double> peopleScore = normalize(currPerson.scores);
        
        currSim = cosineSimilarityOf(userScore, peopleScore);

        if (currSim > maxSim) {
            maxSim = currSim;
            person1 = currPerson;
        }
    }

    return person1;

}
