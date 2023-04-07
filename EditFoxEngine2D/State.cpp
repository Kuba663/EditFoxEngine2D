#include "State.h"

State::State(FiniteStateMachine& fsm, std::string name)
	: fsm(fsm), name(name)
{
}
