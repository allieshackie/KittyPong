#pragma once


class Input {
public:
	Input() {}

	void Update(bool new1, bool new2) {
		up = new1;
		down = new2;
	}

	bool up;
	bool down;
};