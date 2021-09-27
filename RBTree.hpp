#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <memory>

enum NodeColor { BLACK = 1, RED };

template<typename T>
struct RBNode {
	typedef RBNode<T>	value_type;

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
};

template<typename T>
class RBTree {
	private:
		typedef RBNode<T>           RBNode_t;
		std::allocator<RBNode_t>    m_alloc;
		RBNode_t                    *m_root;
	public:
		RBTree() : m_root( nullptr ) { }
		RBTree(const T &value) 
			: m_root( this->create_node(value) ) { }
		~RBTree() { }

		void insert(const T &value){
			RBNode_t *current = m_root;

			if ( current == nullptr ){
				m_root = this->create_node(value);
			} else {
				while ( current->right != nullptr || current->left != nullptr ){
					if ( value > current->value )
						current = current->right;
					else
						current = current->left;
				}
				RBNode_t *node = this->create_node(value, current);

				if ( node > current ){
					current->right = node;
				} else {
					current->left = node;
				}
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
