#pragma once
#include <string>
#include "FiniteStateMachine.h"
class FiniteStateMachine;
class State
{
public:
	explicit State(FiniteStateMachine& fsm, xstring name);
	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;
protected:
	FiniteStateMachine& fsm;
	xstring name;
};

