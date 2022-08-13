//  Project Title: Personality Quiz
//  Personality Quiz user interface
//
//  Created by Roselle Mata
//  Copyright © 2020 Roselle Mata. All rights reserved.
//
#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;

// this function splits the question text by question, by factors

void loadQuestion(set<Question> &questions) {
    ifstream infile("questions.txt");
    string line;

    while (getline(infile, line)) {
        
        // get question up to the "."

        string s = line.substr(0, line.find(".")+1);

        // get factors from after ".", to the end

        string m = line.substr(line.find(".")+2);

        Question q;
        q.questionText = s;

        stringstream sName(m);

        // separate sName by space

        while (sName >> m) {
            q.factors[m[0]] = stoi(m.substr(2));

        }

        questions.insert(q);
    }
}

// this function reads from .people file, split lines by person, by factors

void (loadPeople(vector<set<Person>> &person)) {

    vector<string> filenames = {"BabyAnimals.people", "Brooklyn99.people", "Disney.people",
                "Hogwarts.people", "MyersBriggs.people", "SesameStreet.people",
                            "StarWars.people", "Vegetables.people", "mine.people"};

    for (string filename : filenames) {

        ifstream infile(filename);
        string line;

        set<Person> persons;

        while(getline(infile, line)) {

            string name = line.substr(0,line.find("."));
            string factors = line.substr(line.find(".")+2);

            Person p;
            p.name = name;

            stringstream sName(factors);

            while(sName >> factors) { 
                p.scores[factors[0]] = stoi(factors.substr(2));
            }
            persons.insert(p);
        }
        person.push_back(persons);
    }
}

int main() {
    
    set<Question> questions;
    loadQuestion(questions);

    vector<set<Person>> people;
    loadPeople(people);

    map<Question, int> q;

    int val, answer, testNum;
    cout << "Welcome to the Personality Quiz!" << endl;
    cout << "Choose number of questions: ";
    cin >> val;
    cout << endl;   
    while (val != 0) {
        cout << "How much do you agree with this statement?" << endl;
        Question p = randomQuestionFrom(questions);
        cout << '\"' + p.questionText + '\"' << endl;
        cout << endl; 
        cout << "1. Strongly disagree" << endl;
        cout << "2. Disagree" << endl;
        cout << "3. Neutral" << endl;
        cout << "4. Agree" << endl;
        cout << "5. Strongly agree" << endl;  
        cout << endl;
        cout << "Enter your answer here (1-5): " ;
        cin >> answer;
        cout << endl; 
        val--;

        // store answer to  map q

        q[p] = answer;

    }   

    // scoresFrom(q);

    map<char, int> userScore = scoresFrom(q);

    while (true) {  
        cout << "1. BabyAnimals" << endl;
        cout << "2. Brooklyn99" << endl;
        cout << "3. Disney" << endl;
        cout << "4. Hogwarts" << endl;
        cout << "5. MyersBriggs" << endl;
        cout << "6. SesameStreet" << endl;
        cout << "7. StarWars" << endl;
        cout << "8. Vegetables" << endl;
        cout << "0. To end program." << endl;   
        cout << endl;
        cout << "Choose test number (1-8, or 0 to end): ";
        cin >> testNum; 
        
        if (testNum == 0) {
            break;
        }   

        Person m = mostSimilarTo(userScore, people[testNum -1]);

        cout << "You got " << m.name << "!" << endl;
        cout << endl;
    }   

    cout << "Goodbye!" << endl;
    return 0;
}