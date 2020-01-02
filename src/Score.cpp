#include "Score.h"

#include <cstring>
#include <ostream>

Score::Score()
{
    name="";
    score=0;

}

Score::Score(string inpName, int inpScore){
    name = inpName;
    score=inpScore;
}

int Score::getScore() const{
    return score;
}
string Score::getName() const{
    return name;
}

void Score::setScore(int inpScore){
    score=inpScore;
}

void Score::setName(string inpName){
    name = inpName;
}

//overload operator
bool Score :: operator > (const Score & right) const
{
    return getScore() > right.getScore();
}

bool Score :: operator < (const Score & right) const
{
    return getScore() < right.getScore();
}

bool Score :: operator == (const Score & right) const
{
    return getScore() == right.getScore();
}

std::ostream& operator<<(std::ostream& out, const Score& obj)
{
    out<<obj.getName()<<","<<obj.getScore();
    return out;
}
