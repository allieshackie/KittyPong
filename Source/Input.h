#pragma once


class Input {
public:
	Input(): up(false), down(false) {}

	void Update(bool new1, bool new2) {
		up = new1;
		down = new2;
	}

	bool up;
	bool down;
};