#ifndef __SCAPEGOAT_H_
#define __SCAPEGOAT_H_
#define LEFT(x) x->left.load()
#define RIGHT(x) x->right.load()
#include <atomic>
#include <cmath>
#include <limits>
namespace EditFoxEngine {
	namespace ECS{
			class Entity;
		}
	}
template<typename _Kty, typename _Ty, typename _Hash = std::hash<_Kty>, typename _Comp = std::less<size_t>, typename _Alloc = stl_allocator<_Ty>>
class scapegoat
{
	struct node {
		size_t key;
		_Ty* value;
		std::atomic<node*> parent, left, right;
		scapegoat& tree;
		explicit node(scapegoat& tree) {
			this->tree = tree;
			this->value = nullptr;
			this->left.store(nullptr);
			this->right.store(nullptr);
			this->parent.store(nullptr);
		};
		node(scapegoat& tree, _Kty key, _Ty value) {
			this->tree = tree;
			this->key = _Hash()(key);
			this->value = std::allocator_traits<_Alloc>::allocate(this->allocator, 1);
			std::allocator_traits<_Alloc>::construct(this->allocator, this->value, value);
			this->left.store(nullptr);
			this->right.store(nullptr);
			this->parent.store(nullptr);
		}
		~node() {
			std::allocator_traits<_Alloc>::destroy(this->allocator, this->value);
			std::allocator_traits<_Alloc>::deallocate(this->allocator, this->value, 1);
			delete left.load();
			delete right.load();
			left.store(NULL);
			right.store(NULL);
		}
		node& operator=(node&& n) {
			if (*this != n) {
				std::allocator_traits<_Alloc>::destroy(this->allocator, this->value);
				std::allocator_traits<_Alloc>::construct(this->allocator, this->value, value);
				this->key = n.key;
				n.key = std::numeric_limits<size_t>().max();
				this->tree->remove(std::numeric_limits<size_t>().max());
			}
			return *this;
		}
		_Alloc allocator;
	};
public:
	explicit scapegoat() = default;
	virtual ~scapegoat() {
		delete root.load();
	};
	void insert(_Kty key, _Ty value) {
		node* potentialParent = nullptr;
		node* n = new node(*this ,key, value);
		if (!this->root.load())
			{ this->root.store(n);
			  return; }
		node* currentNode = this->root.load();
		while (currentNode != nullptr)
			{ potentialParent = currentNode;
			  if (_Comp(n->key, currentNode->key)) currentNode = LEFT(currentNode);
			  else currentNode = RIGHT(currentNode); }
		if (potentialParent != nullptr) n->parent.store(potentialParent);
		if (_Comp(n->key, n->parent.load()->key)) n->parent.load()->left.store(n);
		else n->parent.load()->right.store(n);
		this->_length++;
		node* scapegoat = this->findScapegoat(n);
		if (!scapegoat) return;
		node* tmp = this->rebalance(scapegoat);
		scapegoat->left.store(LEFT(tmp));
		scapegoat->right.store(RIGHT(tmp));
		scapegoat->key = tmp->key;
		scapegoat->value = tmp->value;
		LEFT(scapegoat)->parent.store(scapegoat);
		RIGHT(scapegoat)->parent.store(scapegoat);
	}
	void remove(_Kty key) {
		node* parent = nullptr;
		node* succesor;
		bool isLeftChild = true;
		auto n = this->root.load();
		while (n->key != key) {
			parent = n;
			if (_Comp(n->key, key))
				{ n = RIGHT(n);
				  isLeftChild = false; }
			else
				{ n = LEFT(n);
				  isLeftChild = true; }
		}
		if (!LEFT(n) && !RIGHT(n)) return;
		else if (!LEFT(n)) succesor = RIGHT(n);
		else if (!RIGHT(n)) succesor = LEFT(n);
		else{ succesor = this->minimum(RIGHT(n));
			  if (succesor = RIGHT(n)) succesor->left.store(LEFT(n)); }
		if (parent == nullptr) this->root.store(succesor);
		else if (isLeftChild) parent->left.store(succesor);
		else parent->right.store(succesor);
		this->_length--;
		this->root.store(this->rebalance(this->root.load()));
	}
	_Ty& operator[](_Kty at) const {
		return *search(this->root.load())->value;
	}
protected:
	node* search(_Kty at, node* root) const {
		if (!root || root->key == at) return root;
		return _Comp(RIGHT(root), at) ? search(RIGHT(root)) : search(LEFT(root));
	}
	node* findScapegoat(node* n) {
		node* tmp;
		if (n == this->root.load()) return nullptr;
		tmp = n;
		while (this->isBalancedAtNode(tmp)) 
			{ if (tmp == this->root.load()) return nullptr;
			  tmp = tmp->parent.load(); }
		return tmp;
	}
	bool isBalancedAtNode(node* key)
		{ if ((float)(this->sizeOfSubtree(LEFT(key)) - this->sizeOfSubtree(RIGHT(key))) <= 1) return true;
		  return false; }
	size_t sizeOfSubtree(node* key)
		{ if (!key) return 0;
		  return 1 + this->sizeOfSubtree(LEFT(key)) + this->sizeOfSubtree(RIGHT(key)); }
	node* buildBalancedFromArray(node** a, int i, int n) const {
		if (n == 0) return nullptr;
		int m = n >> 1;
		a[i + m]->left.store(buildBalancedFromArray(a, i, m));
		if (LEFT(a[i + m]) != nullptr) LEFT(a[i + m])->parent.store(a[i + m]);
		a[i + m]->right.store(buildBalancedFromArray(a, i + m + 1, n - m - 1));
		if (RIGHT(a[i + m]) != nullptr) RIGHT(a[i + m])->parent = a[i + m];
		return a[i + m];
	}
	int storeInArray(node* ptr, node* arr[], int i) const {
		if (!ptr) return i;
		i = storeInArray(LEFT(ptr), arr, i);
		arr[i++] = ptr;
		return storeInArray(RIGHT(ptr), arr, i);
	}
	node* rebalance(node* root) {
		node* result = root;
		int n = sizeOfSubtree(result);
		auto p = result->parent.load();
		node** arr = new node*[n];
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
	node* minimum(node* n)
		{ auto tmp = n;
		  while (LEFT(tmp) != nullptr) tmp = LEFT(tmp);
		  return tmp; }
private:
	std::atomic<node*> root;
	std::atomic_size_t _length;
	DECLARE_ALLOCATOR
};
#pragma region specializations
#include "efeid.h"
#include "Entity.h"
template<>
class scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>> {
	struct node {
		EditFoxEngine::ECS::Entity* value;
		std::atomic<node*> parent, left, right;
		scapegoat& tree;
		explicit node(scapegoat& tree);
		node(scapegoat& tree, EditFoxEngine::ECS::Entity entity);
		~node();
	};
public:
	explicit scapegoat() = default;
	virtual ~scapegoat() {
		delete root.load();
	};
	void insert(EditFoxEngine::ECS::Entity entity);
	void remove(EditFoxEngine::ECS::Entity entity);
	EditFoxEngine::ECS::Entity& operator[](EditFoxEngine::efeid at) const;
protected:
	node* search(node* root, EditFoxEngine::efeid id) const;
	node* findScapegoat(node* n);
	bool isBalancedAtNode(node* key) {
		if (abs((float)(this->sizeOfSubtree(LEFT(key)) - this->sizeOfSubtree(RIGHT(key)))) <= 1) return true;
		return false;
	}
	size_t sizeOfSubtree(node* key) {
		if (!key) return 0;
		return 1 + this->sizeOfSubtree(LEFT(key)) + this->sizeOfSubtree(RIGHT(key));
	}
	node* buildBalancedFromArray(node** a, int i, int n) const {
		if (n == 0) return nullptr;
		int m = n >> 1;
		a[i + m]->left.store(buildBalancedFromArray(a, i, m));
		if (LEFT(a[i + m]) != nullptr) LEFT(a[i + m])->parent.store(a[i + m]);
		a[i + m]->right.store(buildBalancedFromArray(a, i + m + 1, n - m - 1));
		if (RIGHT(a[i + m]) != nullptr) RIGHT(a[i + m])->parent = a[i + m];
		return a[i + m];
	}
	int storeInArray(node* ptr, node* arr[], int i) const {
		if (!ptr) return i;
		i = storeInArray(LEFT(ptr), arr, i);
		arr[i++] = ptr;
		return storeInArray(RIGHT(ptr), arr, i);
	}
	node* rebalance(node* root);
	node* minimum(node* n) {
		auto tmp = n;
		while (LEFT(tmp) != nullptr) tmp = LEFT(tmp);
		return tmp;
	}
private:
	std::atomic<node*> root;
	std::atomic_size_t _length;
	DECLARE_ALLOCATOR
};
#pragma endregion

#endif
