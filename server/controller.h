
#ifndef CONTROLLER_H_
#define CONTROLLER_H_


class Controller
{
public:
	enum MESSAGES {
		on_ = 0, off, toggle, flash, up_level, down_level,
		refresh_node, send_state, feedback, trigger, new_node
	};

public:
	Controller();
	void reset_();
	void control_thing(MESSAGES, unsigned char thing_address,
		unsigned char extra_byte);
protected:
	void send_word(unsigned char);
};

#endif
