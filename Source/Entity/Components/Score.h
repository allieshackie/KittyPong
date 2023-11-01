#pragma once

class Score
{
public:
	void AddPoint();
	int GetScore() const;
	void SetScore(int score);

private:
	int mScore = 0;
};
