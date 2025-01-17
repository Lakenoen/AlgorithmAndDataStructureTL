#ifndef HAVL
#define HAVL

#include <functional>
#include <queue>

namespace algds {

	template <class K, class I>
	class AVLTree {
	private:
		struct node {
			K key = K();
			I value = I();
			__int32 height = 1;
			node* parent = nullptr;
			node* left = nullptr;
			node* right = nullptr;
		};
		node* root = nullptr;
	private:
		node* succesor(node* x);
		node* find(const K& key);
		void fixUp(node* x);
		__int32 height(node* x);
		short factor(node* x);
		void fixHeight(node* x);
		node* rotateLeft(node* x);
		node* rotateRight(node* x);
	public:
		AVLTree();
		AVLTree(AVLTree& obj);
		AVLTree(AVLTree&& obj);
		AVLTree& operator=(AVLTree& obj);
		AVLTree& operator=(AVLTree&& obj);
		~AVLTree();
		void run(std::function<void(std::pair<K, I>)> func);
		void insert(const K& key, const I& value);
		I& search(const K& key);
		void erase(const K& key);
	};

	template<class K, class I>
	AVLTree<K, I>::template node* AVLTree<K, I>::succesor(node* x)
	{
		node* point = x->left;
		if (!point)
			return nullptr;
		while (point->right) {
			point = point->right;
		}
		return point;
	}

	template<class K, class I>
	AVLTree<K, I>::template node* AVLTree<K, I>::find(const K& key)
	{
		node* point = this->root;
		while (point)
		{
			if (key < point->key)
				point = point->left;
			else if (key == point->key)
				return point;
			else
				point = point->right;
		}
		return point;
	}

	template<class K, class I>
	void AVLTree<K, I>::fixUp(node* x)
	{
		node* point = x;
		node* left = nullptr;
		node* right = nullptr;
		while (point) {
			left = point->left;
			right = point->right;
			if (factor(point) >= 2) {
				if (factor(left) < 0) {
					rotateLeft(left);
					fixHeight(left);
				}
				rotateRight(point);
			}
			else if (factor(point) <= -2) {
				if (factor(right) > 0) {
					rotateRight(right);
					fixHeight(right);
				}
				rotateLeft(point);
			}
			fixHeight(point);
			point = point->parent;
		}
	}

	template<class K, class I>
	inline __int32 AVLTree<K, I>::height(node* x)
	{
		return (x != nullptr) ? x->height : 0;
	}

	template<class K, class I>
	inline short AVLTree<K, I>::factor(node* x)
	{
		return height(x->left) - height(x->right);
	}

	template<class K, class I>
	inline void AVLTree<K, I>::fixHeight(node* x)
	{
		x->height = ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right)) + 1;
	}

	template<class K, class I>
	AVLTree<K, I>::template node* AVLTree<K, I>::rotateLeft(AVLTree<K, I>::node* x)
	{
		node* y = x->right;
		if (x->parent != nullptr && x->parent->right == x)
			x->parent->right = y;
		else if (x->parent != nullptr && x->parent->left == x)
			x->parent->left = y;
		x->right = y->left;
		if (y->left)
			y->left->parent = x;
		y->left = x;
		y->parent = x->parent;
		x->parent = y;
		if (x == this->root)
			this->root = y;
		return y;
	}

	template<class K, class I>
	AVLTree<K, I>::template node* AVLTree<K, I>::rotateRight(AVLTree<K, I>::node* x)
	{
		node* y = x->left;
		if (x->parent != nullptr && x->parent->right == x)
			x->parent->right = y;
		else if (x->parent != nullptr && x->parent->left == x)
			x->parent->left = y;
		x->left = y->right;
		if (y->right)
			y->right->parent = x;
		y->right = x;
		y->parent = x->parent;
		x->parent = y;
		if (x == this->root)
			this->root = y;
		return y;
	}

	template<class K, class I>
	AVLTree<K, I>::AVLTree()
	{
	}

	template<class K, class I>
	AVLTree<K, I>::~AVLTree()
	{
		std::queue<K> history;
		run([&history](std::pair<K, I> p)->void {
			history.push(p.first);
			});
		while (!history.empty()) {
			erase(history.front());
			history.pop();
		}
	}

	template<class K, class I>
	void AVLTree<K, I>::run(std::function<void(std::pair<K, I>)> func) {
		if (!this->root)
			return;
		std::queue<node*> q;
		q.push(this->root);
		while (!q.empty()) {
			node* p = q.front();
			q.pop();
			if (p->left)
				q.push(p->left);
			if (p->right)
				q.push(p->right);
			func(std::make_pair(p->key, p->value));
		}
	}

	template<class K, class I>
	inline void AVLTree<K, I>::insert(const K& key, const I& value)
	{
		node* point = this->root;
		node* parent = nullptr;
		while (point != nullptr) {
			parent = point;
			if (key < point->key)
				point = point->left;
			else if (key == point->key)
				return;
			else
				point = point->right;
		}
		point = new node{ key,value,1,parent };
		if (!parent) {
			this->root = point;
			return;
		}
		if (key < parent->key)
			parent->left = point;
		else
			parent->right = point;
		fixUp(point);
	}

	template<class K, class I>
	I& AVLTree<K, I>::search(const K& key)
	{
		node* x = find(key);
		if (!x)
			throw std::domain_error("not found");
		return x->value;
	}

	template<class K, class I>
	void AVLTree<K, I>::erase(const K& key)
	{
		node* x = find(key);
		if (x == this->root & x->height <= 1) {
			delete x;
			this->root = nullptr;
			return;
		}
		node* y = x;
		node* s = nullptr;
		if (!x)
			return;
		if (x->left && x->right) {
			y = succesor(x);
			x->value = y->value;
			x->key = y->key;
		}
		if (y->left)
			s = y->left;
		else
			s = y->right;
		if (y->parent == nullptr) {
			this->root = s;
			this->root->parent = nullptr;
		}
		else {
			if (s)
				s->parent = y->parent;
			if (y->parent->left == y)
				y->parent->left = s;
			else
				y->parent->right = s;
		}
		node* p = ((s) ? s : y->parent);
		delete y;
		fixUp(p);
	}

	template<class K, class I>
	AVLTree<K, I>::AVLTree(AVLTree& obj) {
		obj.run([this](std::pair<K, I> p)->void {
			this->insert(p.first, p.second);
			});
	}

	template<class K, class I>
	AVLTree<K, I>::AVLTree(AVLTree&& obj) {
		this->root = obj.root;
		obj.root = nullptr;
	}

	template<class K, class I>
	AVLTree<K, I>& AVLTree<K, I>::operator=(AVLTree& obj) {
		if (this == &obj)
			return *this;
		this->~AVLTree();
		obj.run([this](std::pair<K, I> p)->void {
			this->insert(p.first, p.second);
			});
		return *this;
	}

	template<class K, class I>
	AVLTree<K, I>& AVLTree<K, I>::operator=(AVLTree&& obj) {
		if (this == &obj)
			return *this;
		this->~AVLTree();
		this->root = obj.root;
		obj.root = nullptr;
		return *this;
	}
}
#endif