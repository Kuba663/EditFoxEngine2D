#pragma once
#include <memory>
#include <map>
#include <string>
#include <cassert>
#include "State.h"
class State;
class FiniteStateMachine
{
	std::map<std::string, std::unique_ptr<State>> states;
	State* currentState;
public:
	FiniteStateMachine();
	template<class S>
	State& addState(std::string name);
	State& getState(std::string name);
	State& getCurrentState() const;
	void setCurrentState(std::string name);
	void update();
protected:
	void setCurrentState(State* state);
};

template<class S>
inline State& FiniteStateMachine::addState(std::string name)
{
	static_assert(!std::is_same<State, S>());
	states[name] = std::make_unique<S>(*this, name);
	return *states[name];
}
