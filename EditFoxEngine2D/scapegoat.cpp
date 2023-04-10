#include "stdafx.h"
#include "scapegoat.h"
scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::node::node(scapegoat& tree, EditFoxEngine::ECS::Entity entity)
	: tree(tree), value(new EditFoxEngine::ECS::Entity(entity))
{
	this->left.store(nullptr);
	this->right.store(nullptr);
	this->parent.store(nullptr);
}
scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::node::~node() {
	delete this->value;
	delete this->parent.load();
	delete this->right.load();
	delete this->left.load();
}
void scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::insert(EditFoxEngine::ECS::Entity entity)
{
	node* potentialParent = nullptr;
	node* n = new node(*this, entity);
	if (!this->root.load())
		{ this->root.store(n);
		  return; }
	node* currentNode = this->root.load();
	while (currentNode != nullptr){
		potentialParent = currentNode;
		if (n->value->id < currentNode->value->id) currentNode = LEFT(currentNode);
		else currentNode = RIGHT(currentNode);
	}
	if (potentialParent != nullptr) n->parent.store(potentialParent);
	if (n->value->id < n->parent.load()->value->id) n->parent.load()->left.store(n);
	else n->parent.load()->right.store(n);
	this->_length++;
	node* scapegoat = this->findScapegoat(n);
	if (!scapegoat) return;
	node* tmp = this->rebalance(scapegoat);
	scapegoat->left.store(LEFT(tmp));
	scapegoat->right.store(RIGHT(tmp));
	scapegoat->value = tmp->value;
	LEFT(scapegoat)->parent = scapegoat;
	RIGHT(scapegoat)->parent = scapegoat;
}
void scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::remove(EditFoxEngine::ECS::Entity entity)
{
	node* parent = nullptr;
	node* succesor;
	bool isLeftChild = true;
	auto n = this->root.load();
	while (n->value->id != entity.id) {
		parent = n;
		if (n->value->id < entity.id)
		{
			n = RIGHT(n);
			isLeftChild = false;
		}
		else
		{
			n = LEFT(n);
			isLeftChild = true;
		}
	}
	if (!LEFT(n) && !RIGHT(n)) return;
	else if (!LEFT(n)) succesor = RIGHT(n);
	else if (!RIGHT(n)) succesor = LEFT(n);
	else {
		succesor = this->minimum(RIGHT(n));
		if (succesor == RIGHT(n)) succesor->left.store(LEFT(n));
	}
	if (parent == nullptr) this->root.store(succesor);
	else if (isLeftChild) parent->left.store(succesor);
	else parent->right.store(succesor);
	this->_length--;
	this->root.store(this->rebalance(this->root.load()));
}

EditFoxEngine::ECS::Entity& scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::operator[](EditFoxEngine::efeid at) const
{
	return *search(this->root.load(),at)->value;
}

scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::node* scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::search(node* satrt, EditFoxEngine::efeid id) const
{
	if (!satrt || satrt->value->id == id) return satrt;
	return RIGHT(satrt)->value->id < id ? search(RIGHT(satrt),id) : search(LEFT(satrt),id);
}

scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::node* scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::findScapegoat(node* n)
{
	node* tmp;
	if (n == this->root.load()) return nullptr;
	tmp = n;
	while (this->isBalancedAtNode(tmp))
	{
		if (tmp == this->root.load()) return nullptr;
		tmp = tmp->parent.load();
	}
	return tmp;
}

scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::node* scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>::rebalance(node* satrt) {
	node* result = satrt;
	int n = sizeOfSubtree(result);
	auto p = result->parent.load();
	node** arr = new node * [n];
	storeInArray(root, arr, 0);
	if (!p)
	{
		result = buildBalancedFromArray(arr, 0, n);
		result->parent.store(nullptr);
	}
	else if (RIGHT(p) == result)
	{
		p->right.store(buildBalancedFromArray(arr, 0, n));
		RIGHT(p)->parent.store(p);
	}
	else {
		p->left.store(buildBalancedFromArray(arr, 0, n));
		LEFT(p)->parent.store(p);
	}
	return result;
}