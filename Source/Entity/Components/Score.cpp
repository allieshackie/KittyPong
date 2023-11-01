#include "Score.h"

void Score::AddPoint()
{
	mScore++;
}

int Score::GetScore() const
{
	return mScore;
}

void Score::SetScore(int score)
{
	mScore = score;
}
