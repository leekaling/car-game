#ifndef SCORE_H
#define SCORE_H


#include <iostream>
#include <string>

using namespace std;

class Score
{
    string name;
    int score;


public:

    Score();
    Score(string, int);
    int getScore() const;
    string getName() const;

    void setScore(int);
    void setName(string);
    
    // Overloaded operator functions
    bool operator > (const Score &) const;     // Overloaded >
    bool operator < (const Score &) const;     // Overloaded <
    bool operator == (const Score &) const;    // Overloaded ==

};

std::ostream& operator<<(std::ostream& out, const Score&);

#endif // SCORE_H
