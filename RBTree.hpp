#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <memory>

enum NodeColor { BLACK = 1, RED };

template<typename T>
struct RBNode {
	typedef RBNode<T>	value_type;
	typedef T*			pointer;

	RBNode				*parent;
	RBNode				*left;
	RBNode				*right;
	NodeColor			color;
	T					value;

	RBNode( const T &__value ) 
		: value( __value ), parent( nullptr ), left( nullptr ), right( nullptr ), color( RED ) {}
	RBNode( const T &__value, RBNode *__parent ) 
		: value( __value ), parent( __parent ), left( nullptr ), right( nullptr ), color( RED ) {}
	RBNode( const T &__value, RBNode *__parent, NodeColor __color ) 
		: value( __value ), parent( __parent ), left( nullptr ), right( nullptr ), color( __color ) {}
	~RBNode() { }

	value_type *get_uncle(){
		value_type *grand_parent = get_grand_parent();
		value_type *parent = get_parent();

		if ( grand_parent == nullptr )
			return nullptr;
		return parent->get_sibling();
	}

	value_type *get_parent(){
		return this->parent;
	}

	value_type *get_grand_parent() {
		if ( this->parent )
			return this->parent->parent;
		return nullptr;
	}

	value_type *get_sibling(){
		if ( this->parent == nullptr )
			return nullptr;
		if ( this == this->parent->left )
			return this->parent->right;
		return this->parent->left;
	}

	bool red_collide(){
		if ( parent == nullptr )
			return false;
		return parent->color == RED && color == RED;
	}
};

template<typename T>
class RBTree {
	private:
		typedef RBNode<T>           RBNode_t;
		typedef T					value_type;
		typedef T&					reference;
		typedef T*					pointer;
		std::allocator<RBNode_t>    m_alloc;
		RBNode_t                    *m_root;
	public:
		RBTree() : m_root( nullptr ) { }
		RBTree(const T &value) 
			: m_root( this->create_node(value) ) { }
		~RBTree() { }

		void insert(const T &value){
			// Insert new node
			RBNode_t *node = _insert_new_node_recursive(m_root, value);

			// Fix the tree if properties has been broken
			_fix_tree(node);

			RBNode_t *new_root = node;

			while ( new_root->parent != nullptr )
				new_root = new_root->parent;
			m_root = new_root;
		}

		RBNode_t *get_root() {
			return m_root;
		}

		RBNode_t *search(const T &value){
			return _search_recursive(m_root, value);
		}

		void rotate_left(RBNode_t *x){
			RBNode_t *y = x->right;
			x->right = y->left;

			if ( y->left != nullptr ){
				y->left->parent = x;
			}
			y->parent = x->parent;
			if ( x->parent == nullptr ){
				m_root = y;
			}
			else if ( x == x->parent->left ){
				x->parent->left = y;
			} else {
				x->parent->right = y;
			}
			y->left = x;
			x->parent = y;
		}

		void rotate_right(RBNode_t *y){
			RBNode_t *x = y->left;
			y->left = x->right;

			if ( x->right != nullptr ){
				x->right->parent = y;
			}
			x->parent = y->parent;
			if ( y->parent == nullptr ){
				m_root = x;
			}
			else if ( y == y->parent->right ){
				y->parent->right = x;
			} else {
				y->parent->left = x;
			}
			x->right = y;
			y->parent = x;
		}


		void print(){
			_postfix_recursive_print(m_root, 0);
			std::cout << std::endl;
		}
	private:
		void _fix_case_1(RBNode_t *node){
			node->color = BLACK;
		}

		void _fix_case_2(RBNode_t *node){
			// Don't do anything ??
			return;
		}

		void _fix_case_3(RBNode_t *node){
			node->parent->color = BLACK;
			node->get_uncle()->color = BLACK;

			RBNode_t *gp = node->get_grand_parent();
			gp->color = RED;
			_fix_tree(gp);
		}

		void _fix_case_4(RBNode_t *node){
			RBNode_t *p = node->parent;
			RBNode_t *g = node->get_grand_parent();

			if ( g != nullptr ){
				if ( g->left != nullptr ){
					if ( node == g->left->right ){
						rotate_left(p);
						node = node->left;
					}
				} else if ( g->right != nullptr ){
					if ( node == g->right->left ){
						rotate_right(p);
						node = node->right;
					}
				}
				_fix_case_5(node);
			}
		}

		void _fix_case_5(RBNode_t *node){
			RBNode_t *p = node->parent;
			RBNode_t *g = node->get_grand_parent();

			if ( node == p->left ){
				rotate_right(g);
			}
			else
				rotate_left(g);
			p->color = BLACK;
			g->color = RED;
		}

		void _fix_tree(RBNode_t *node){
			RBNode_t *uncle = node->get_uncle();

			if ( node->parent == nullptr )
				_fix_case_1(node);
			else if ( node->parent->color == BLACK ){
				_fix_case_2(node);
			} 
			else if ( uncle != nullptr && uncle->color == RED ){
				_fix_case_3(node);
			} 
			else {
				_fix_case_4(node); // Pourquoi dans tous les cas ?
			}
		}

		RBNode_t *_insert_new_node_recursive(RBNode_t *node, const T &value){
			if ( node == nullptr ){
				m_root = this->create_node(value);
				return m_root;
			} else {
				if ( value > node->value ){
					if ( node->right == nullptr ){
						node->right = this->create_node(value, node);
					} else {
						return _insert_new_node_recursive(node->right, value);
					}
					return node->right;
				} else {
					if ( node->left == nullptr ){
						node->left = this->create_node(value, node);
					} else {
						return _insert_new_node_recursive(node->left, value);
					}
					return node->left;
				}
			}
		}

		RBNode_t *_search_recursive(RBNode_t *node, const T &value){
			if ( node == nullptr ){
				return nullptr;
			}
			if ( node->value == value ){
				return node;
			}
			if ( value > node->value )
				return _search_recursive(node->right, value);
			else
				return _search_recursive(node->left, value);
		}

		void _postfix_recursive_print(RBNode_t *current, int space){
			if(current != nullptr) {
				space = space + 10;
				_postfix_recursive_print(current->right, space);
				std::cout << std::endl;
				for ( int i = 10; i < space; i++)
					std::cout << " ";
				std::cout << "( " << current->value << ", " <<
					((current->color == BLACK) ? "B" : "R") << " )" << std::endl;
				_postfix_recursive_print(current->left, space);
			}
		}

		RBNode_t *create_node(const T &value){
			RBNode_t *new_node = m_alloc.allocate(1);
			m_alloc.construct( new_node, value );
			return new_node;
		}

		RBNode_t *create_node(const T &value, RBNode_t *parent) {
			RBNode_t *new_node = m_alloc.allocate(1);
			m_alloc.construct( new_node, value, parent );
			return new_node;
		}

		RBNode_t *create_node(const T &value, RBNode_t *parent, NodeColor color){
			RBNode_t *new_node = m_alloc.allocate(1);
			m_alloc.construct( new_node, value, parent, color );
			return new_node;
		}
};


#endif
