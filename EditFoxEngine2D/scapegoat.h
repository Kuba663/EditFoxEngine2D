#ifndef __SCAPEGOAT_H_
#define __SCAPEGOAT_H_
#define LEFT(x) x->left.load()
#define RIGHT(x) x->right.load()
#include <memory>
#include <stl_allocator.h>
#include <atomic>
#include <cmath>
#include <limits>
template<typename _Kty, typename _Ty, typename _Hash = std::hash<_Kty>, typename _Comp = std::less<size_t>, typename _Alloc = stl_allocator<_Ty>>
class scapegoat
{
	struct node {
		size_t key;
		_Ty* value;
		std::atomic<std::shared_ptr<node>> parent, left, right;
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
	virtual ~scapegoat() {};
	void insert(_Kty key, _Ty value) {
		std::shared_ptr<node> potentialParent;
		std::shared_ptr<node> n = std::make_shared<node>(*this ,key, value);
		if (!this->root.load())
			{ this->root.store(n);
			  return; }
		std::shared_ptr<node> currentNode = this->root.load();
		while (currentNode != nullptr)
			{ potentialParent = currentNode;
			  if (_Comp(n->key, currentNode->key)) currentNode = LEFT(currentNode);
			  else currentNode = RIGHT(currentNode); }
		n->parent.store(potentialParent);
		if (_Comp(n->key, n->parent.load()->key)) n->parent.load()->left.store(n);
		else n->parent.load()->right.store(n);
		this->_length++;
		std::shared_ptr<node> scapegoat = this->findScapegoat(n);
		if (!scapegoat) return;
		std::shared_ptr<node> tmp = this->rebalance(scapegoat);
		scapegoat->left.store(LEFT(tmp));
		scapegoat->right.store(RIGHT(tmp));
		scapegoat->key = tmp->key;
		LEFT(scapegoat)->parent = scapegoat;
		RIGHT(scapegoat)->parent = scapegoat;
	}
	void remove(_Kty key) {
		std::shared_ptr<node> parent, succesor;
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
		if (!parent) this->root.store(succesor);
		else if (isLeftChild) parent->left.store(succesor);
		else parent->right.store(succesor);
		this->_length--;
		this->root.store(this->rebalance(this->root.load()));
	}
	_Ty& operator[](_Kty at) const {
		constexpr std::shared_ptr<node> search = [&at](std::shared_ptr<node> root) {
			if (!root || root->key == at) return root;
			return _Comp(RIGHT(root),at) ? search(RIGHT(root)) : search(LEFT(root));
		};
		return *search(this->root.load());
	}
protected:
	std::shared_ptr<node> findScapegoat(std::shared_ptr<node> n) {
		std::shared_ptr<node> tmp;
		if (n == this->root.load()) return nullptr;
		tmp = n;
		while (this->isBalancedAtNode(tmp)) 
			{ if (tmp == this->root.load()) return nullptr;
			  tmp = tmp->parent.load(); }
		return tmp;
	}
	bool isBalancedAtNode(std::shared_ptr<node> key) 
		{ if (abs(this->sizeOfSubtree(LEFT(key)) - this->sizeOfSubtree(RIGHT(key))) <= 1) return true;
		  return false; }
	size_t sizeOfSubtree(std::shared_ptr<node> key) 
		{ if (!key) return 0;
		  return 1 + this->sizeOfSubtree(LEFT(key)) + this->sizeOfSubtree(RIGHT(key)); }
	std::shared_ptr<node> rebalance(std::shared_ptr<node> root) {
		constexpr std::shared_ptr<node> buildBalancedFromArray = [](std::shared_ptr<node>* a, int i, int n) {
			if (n == 0) return nullptr;
			int m = n >> 1;
			a[i + m]->left.store(buildBalancedFromArray(a, i, m));
			if (LEFT(a[i + m]) != nullptr) LEFT(a[i + m])->parent.store(a[i + m]);
			a[i + m]->right.store(buildBalancedFromArray(a, i + m + 1, n - m - 1));
			if (RIGHT(a[i + m]) != nullptr) RIGHT(a[i + m])->parent = a[i + m];
			return a[i + m];
		};
		constexpr int storeInArray = [](std::shared_ptr<node> ptr, std::shared_ptr<node> arr[], int i) {
			if (!ptr) return i;
			i = storeInArray(LEFT(ptr), arr, i);
			arr[i++] = ptr;
			return storeInArray(RIGHT(ptr), arr, i);
		};
		std::shared_ptr<node> result;
		int n = sizeOfSubtree(root);
		auto p = root->parent.load();
		std::shared_ptr<node>* arr = new std::shared_ptr<node>[n];
		storeInArray(root, arr, 0);
		if (!p) 
			{ result = buildBalancedFromArray(arr, 0, n);
			  result->parent.store(nullptr); }
		else if (RIGHT(p) == root)
			{ p->right.store(buildBalancedFromArray(a, 0, n));
			  RIGHT(p)->parent.store(p); }
		else{ p->left.store(buildBalancedFromArray(a, 0, n));
			  LEFT(p)->parent.store(p); }
		return result;
	}
	std::shared_ptr<node> minimum(std::shared_ptr<node> n) 
		{ auto tmp = n;
		  while (LEFT(tmp) != nullptr) tmp = LEFT(tmp);
		  return tmp; }
private:
	std::atomic<std::shared_ptr<node>> root;
	std::atomic_size_t _length;
};
#endif
