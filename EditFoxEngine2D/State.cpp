#include "stdafx.h"
#include "State.h"

State::State(FiniteStateMachine& fsm, xstring name)
	: fsm(fsm), name(name)
{
}
