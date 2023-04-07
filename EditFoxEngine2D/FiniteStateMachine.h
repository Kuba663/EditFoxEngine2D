#pragma once
#include <memory>
#include <cassert>
#include "State.h"
class State;
class FiniteStateMachine
{
	xmap<xstring, std::unique_ptr<State>> states;
	State* currentState;
public:
	FiniteStateMachine();
	template<class S>
	State& addState(xstring name);
	State& getState(xstring name);
	State& getCurrentState() const;
	void setCurrentState(xstring name);
	void update();
protected:
	void setCurrentState(State* state);
};

template<class S>
inline State& FiniteStateMachine::addState(xstring name)
{
	static_assert(!std::is_same<State, S>());
	states[name] = std::make_unique<S>(*this, name);
	return *states[name];
}
