#include "stdafx.h"
#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine()
	: currentState(nullptr)
{
}
State& FiniteStateMachine::getState(xstring name)
{
	return *states[name];
}

State& FiniteStateMachine::getCurrentState() const
{
	return *currentState;
}

void FiniteStateMachine::setCurrentState(xstring name)
{
	setCurrentState(states[name].get());
}

void FiniteStateMachine::update()
{
	if (currentState != nullptr)
		currentState->update();
}

void FiniteStateMachine::setCurrentState(State* state)
{
	if (currentState == state)
		return;
	if (currentState != nullptr)
		currentState->exit();
	currentState = state;
	if (currentState != nullptr)
		currentState->enter();
}
