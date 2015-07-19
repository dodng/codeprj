////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tree_rb.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:17
// @brief:  红黑树
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TREE_RB_H_
#define __TREE_RB_H_

#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

namespace algorithm
{
	enum nodeColor
	{
		BLACK = 0,
		RED,
	};

	template<class T>
	struct rbtnode
	{
		T key;
		int lcount;
		int rcount;
		rbtnode<T>* lchild;
		rbtnode<T>* rchild;
		rbtnode<T>* parent;
		nodeColor color;
	};

	template<class T>
	class rbtree
	{
	public:
		rbtree();
		~rbtree();
		rbtnode<T>* atIndex(int i);
		void		del(rbtnode<T>* node);
		void		insert(const T& key);
		rbtnode<T>* search(const T& key);
		void		print();
		int			nodeCount() { return m_root != m_null? m_root->lcount + m_root->rcount + 1: 0; }

	protected:
		void		__clear(); 
		void		__delFixup(rbtnode<T>* delNode);
		void		__insertFixup(rbtnode<T>* insertNode);
		inline void __leftRotate(rbtnode<T>* node);
		inline void __rightRotate(rbtnode<T>* node);
		rbtnode<T>* __treeMax(rbtnode<T>* root);
		rbtnode<T>* __treeMin(rbtnode<T>* root);
		rbtnode<T>* __treeNext(rbtnode<T>* node);
		rbtnode<T>* __treePre(rbtnode<T>* node);

	private:
		rbtnode<T>* m_null;
		rbtnode<T>* m_root;
	};

	template<class T>
	rbtree<T>::rbtree() 
	{
		m_null = new rbtnode<T>;
		m_null->color = BLACK;
		m_null->lchild = m_null->rchild = m_null;
		m_root = m_null;
	}

	template<class T>
	rbtree<T>::~rbtree()
	{
		__clear();
		delete m_null;
		m_null = NULL;
	}

	template<class T>
	void 
		rbtree<T>::__clear() 
	{//
		rbtnode<T>* p = m_root;
		while (p != m_null) {
			if (p->lchild != m_null) {
				p = p->lchild;
			}else if (p->rchild != m_null) {
				p = p->rchild;
			}else {
				rbtnode<T>* temp = p;
				p = p->parent;
				if (temp == p->lchild) {
					p->lchild = m_null;
				}else {
					p->rchild = m_null;
				}
				delete temp;
			}
		}
		m_root = m_null;
	}

	template<class T>
	void 
		rbtree<T>::__delFixup(rbtnode<T>* delNode) 
	{
		rbtnode<T>* p = delNode;
		while (p != m_root && p->color == BLACK) {
			if (p == p->parent->lchild) {
				rbtnode<T>* sibling = p->parent->rchild;
				if (sibling->color == RED) {
					sibling->color = BLACK;
					p->parent->color = RED;
					__leftRotate(p->parent);
					sibling = p->parent->rchild;
				}
				if (sibling->lchild->color == BLACK
					&& sibling->rchild->color == BLACK
					) {
						sibling->color = RED;
						p = p->parent;
				}else {
					if (sibling->rchild->color == BLACK) {
						sibling->lchild->color = BLACK;
						sibling->color = RED;
						__rightRotate(sibling);
						sibling = sibling->parent;
					}
					sibling->color = sibling->parent->color;
					sibling->parent->color = BLACK;
					sibling->rchild->color = BLACK;
					__leftRotate(sibling->parent);
					p = m_root;
				}
			}else {
				rbtnode<T>* sibling = p->parent->lchild;
				if (sibling->color == RED) {
					sibling->color = BLACK;
					p->parent->color = RED;
					__rightRotate(p->parent);
					sibling = p->parent->lchild;
				}
				if (sibling->lchild->color == BLACK
					&& sibling->rchild->color == BLACK
					) {
						sibling->color = RED;
						p = p->parent;
				}else {
					if (sibling->lchild->color == BLACK) {
						sibling->rchild->color = BLACK;
						sibling->color = RED;
						__leftRotate(sibling);
						sibling = sibling->parent;
					}
					sibling->color = sibling->parent->color;
					sibling->parent->color = BLACK;
					sibling->lchild->color = BLACK;
					__rightRotate(sibling->parent);
					p = m_root;
				}
			}
		}
		p->color = BLACK;
	}

	template<class T>
	void 
		rbtree<T>::__insertFixup(rbtnode<T>* insertNode)
	{
		rbtnode<T>* p = insertNode;
		while (p->parent->color == RED) {
			if (p->parent == p->parent->parent->lchild) {
				rbtnode<T>* parentRight = p->parent->parent->rchild;
				if (parentRight->color == RED) {
					p->parent->color = BLACK;
					parentRight->color = BLACK;
					p->parent->parent->color = RED;
					p = p->parent->parent;
				}else {
					if (p == p->parent->rchild) {
						p = p->parent;
						__leftRotate(p);
					}
					p->parent->color = BLACK;
					p->parent->parent->color = RED;
					__rightRotate(p->parent->parent);
				}
			}else {
				rbtnode<T>* parentLeft = p->parent->parent->lchild;
				if (parentLeft->color == RED) {
					p->parent->color = BLACK;
					parentLeft->color = BLACK;
					p->parent->parent->color = RED;
					p = p->parent->parent;
				}else {
					if (p == p->parent->lchild) {
						p = p->parent;
						__rightRotate(p);
					}
					p->parent->color = BLACK;
					p->parent->parent->color = RED;
					__leftRotate(p->parent->parent);
				}
			}
		}
		m_root->color = BLACK;
	}

	template<class T>
	inline void 
		rbtree<T>::__leftRotate(rbtnode<T>* node)
	{
		//把一个节点向左下方移一格，并让他原来的右子节点代替它的位置。
		rbtnode<T>* right = node->rchild;
		node->rchild = right->lchild;
		node->rcount = right->lcount;
		node->rchild->parent = node;
		right->parent = node->parent;
		if (right->parent == m_null) {
			m_root = right;
		}else if (node == node->parent->lchild) {
			node->parent->lchild = right;
		}else {
			node->parent->rchild = right;
		}
		right->lchild = node;
		right->lcount += node->lcount + 1;
		node->parent = right;
	}

	template<class T>
	inline void 
		rbtree<T>::__rightRotate(rbtnode<T>* node) 
	{
		//把一个节点向右下方移一格，并让他原来的左子节点代替它的位置。
		rbtnode<T>* left = node->lchild;
		node->lchild = left->rchild;
		node->lcount = left->rcount;
		node->lchild->parent = node;
		left->parent = node->parent;
		if (left->parent == m_null) {
			m_root = left;
		}else if (node == node->parent->lchild) {
			node->parent->lchild = left;
		}else {
			node->parent->rchild = left;
		}
		left->rchild = node;
		left->rcount += node->rcount + 1;
		node->parent = left;
	}

	template<class T>
	rbtnode<T>* 
		rbtree<T>::__treeMax(rbtnode<T>* root) 
	{//找到子树中最大的一个节点
		rbtnode<T>* result = root;
		while (result->rchild != m_null) {
			result = result->rchild;
		}
		return result;
	}

	template<class T>
	rbtnode<T>* 
		rbtree<T>::__treeMin(rbtnode<T>* root) 
	{//找到子树中最小的一个节点
		rbtnode<T>* result = root;
		while (result->lchild != m_null) {
			result = result->lchild;
		}
		return result;
	}

	template<class T>
	rbtnode<T>* 
		rbtree<T>::__treeNext(rbtnode<T>* node) 
	{//一个节点在中序遍列中的下一个节点。后继
		rbtnode<T>* result;
		if (node->rchild != m_null) {
			result = __treeMin(node->rchild);
		}else {
			result = node->parent;
			rbtnode<T>* temp = node;
			while (result != m_null && temp == result->rchild) {
				temp = result;
				result = result->parent;
			}
		}
		return result;
	}

	template<class T>
	rbtnode<T>* 
		rbtree<T>::__treePre(rbtnode<T>* node)
	{//一个节点在中序遍列中的前一个节点。前驱
		rbtnode<T>* result;
		if (node->lchild != m_null) {
			result = __treeMax(node->lchild);
		}else {
			result = node->parent;
			rbtnode<T>* temp = node;
			while (result != m_null && temp == result->lchild) {
				temp = result;
				result = result->parent;
			}
		}
		return result;
	}

	template<class T>
	rbtnode<T>* 
		rbtree<T>::atIndex(int i) 
	{//找到从小到大排序后下标为i的节点。i从0开始。
		rbtnode<T>* result = m_root;
		if (i > result->lcount + result->rcount) {
			result = NULL;
		}else {
			while (i != result->lcount) {
				if (i < result->lcount) {
					result = result->lchild;
				}else {
					i -= result->lcount + 1;
					result = result->rchild;
				}
			}
		}
		return result;
	}

	template<class T>
	void 
		rbtree<T>::del(rbtnode<T>* node) 
	{//删除一个节点
		rbtnode<T>* toDel = node;
		if (node->lchild != m_null && node->rchild != m_null) {
			toDel = __treeNext(node);//找到中序后继：即右子树最左节点
		}
		rbtnode<T>* temp = toDel;
		while (temp->parent != m_null) {
			if (temp == temp->parent->lchild) {
				temp->parent->lcount--;
			}else {
				temp->parent->rcount--;
			}
			temp = temp->parent;
		}
		rbtnode<T>* replace = toDel->lchild != m_null? toDel->lchild: toDel->rchild;
		replace->parent = toDel->parent;
		if (replace->parent == m_null) {
			m_root = replace;
		}else if (toDel == toDel->parent->lchild) {
			replace->parent->lchild = replace;
		}else {
			replace->parent->rchild = replace;
		}
		if (toDel != node) {
			node->key = toDel->key;
		}
		if (toDel->color == BLACK) {
			//修改树，以保持平衡。
			__delFixup(replace);
		}
		delete toDel;
	}

	template<class T>
	void 
		rbtree<T>::insert(const T& key)
	{//插入一个节点
		rbtnode<T>* node = new rbtnode<T>;
		node->key = key;
		node->lcount = 0;
		node->rcount = 0;
		node->lchild = m_null;
		node->rchild = m_null;
		node->color = RED;

		rbtnode<T>* p = m_root;
		rbtnode<T>* leaf = m_null;
		while (p != m_null) {
			leaf = p;
			if (node->key < p->key) {
				p->lcount++;
				p = p->lchild;
			}else {
				p->rcount++;
				p = p->rchild;
			}
		}
		node->parent = leaf;
		if (leaf == m_null) {//如果是空树。
			m_root = node;
		}else if (node->key < leaf->key) {
			leaf->lchild = node;
		}else {
			leaf->rchild = node;
		}
		//修改树，以保持平衡。
		__insertFixup(node);
	}

	template<class T>
	rbtnode<T>* 
		rbtree<T>::search(const T& key)
	{//按照key查找一个节点。
		rbtnode<T>* result = m_root;
		while (result != m_null && !( result->key == key)) {
			result = ( result->key > key)  ? result->lchild : result->rchild;
		}
		return result == m_null? NULL: result;
	}

	template<class T>
	void 
		rbtree<T>::print() 
	{//把树中节点的值放进一个数组。
		rbtnode<T>* p = __treeMin(m_root);
		int i = 0;
		while (p != m_null) {
			cout<<p->key<<endl;
			i++;
			p = __treeNext(p);
		}
		cout<<"/*---------------------------------*/";
		cout<<endl;
	}

}	// namespace algorithm
using namespace algorithm;

#endif	//__TREE_RB_H_


