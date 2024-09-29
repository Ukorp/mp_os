#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class red_black_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:
    
    enum class node_color
    {
        RED,
        BLACK
    };

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
        
        node_color color;
        
    public:

        node(
                tkey const &key,
                tvalue const &value
        ) : binary_search_tree<tkey, tvalue>::node(key, value), color(node_color::RED){}
        node(
                tkey const &key,
                tvalue &&value
        ) : binary_search_tree<tkey, tvalue>::node(key, value), color(node_color::RED){}
        ~node() noexcept override = default;
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
    public:
        
        node_color color;
    
    public:
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            node_color color);

        iterator_data() : binary_search_tree<tkey, tvalue>::iterator_data(){}
        
    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
        red_black_tree<tkey, tvalue>::node** _root;

    public:
        
        explicit insertion_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:
        
        bool is_red(node* node)
        {
            if(!node) return false;
            return node->color == node_color::RED;
        }

        void change_node_color(node * &node)
        {
            node->color = (is_red(node)) ? node_color::BLACK : node_color::RED;
        }

        void balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override
        {
            while(!path.empty())
            {
                node* current = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*path.top());
                path.pop();

                if(path.empty())
                    current->color = node_color::BLACK;

                if(path.size() >= 2)
                {
                    node* parent = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*path.top());
                    path.pop();

                    node* grand_parent = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*path.top());
                    path.pop();

                    bool is_uncle_left;

                    node* uncle = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(parent == grand_parent->right_subtree ? grand_parent->left_subtree : grand_parent->right_subtree);

                    is_uncle_left = uncle == grand_parent->left_subtree;

                    if(!is_red(parent))
                    {
                        path.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(&parent));
                        path.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(&grand_parent));
                    }

                    else
                    {
                        if(is_red(uncle))
                        {
                            change_node_color(parent);
                            change_node_color(uncle);
                            change_node_color(grand_parent);


                            path.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(&grand_parent));

                        }

                        else
                        {
                            if(is_uncle_left && current == parent->left_subtree || !is_uncle_left && current == parent->right_subtree)
                            {
                                node* new_subtree_root = current;

                                typename binary_search_tree<tkey, tvalue>::node* bst_grand_parent = reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node*>(grand_parent);

                                is_uncle_left ? this->_tree->big_left_rotation(bst_grand_parent) : this->_tree->big_right_rotation(bst_grand_parent);

                                if(!path.empty())
                                    grand_parent == (*path.top())->left_subtree ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                                else *_root = new_subtree_root;

                                change_node_color(current);
                                change_node_color(grand_parent);

                                break;
                            }

                            else
                            {
                                node* new_subtree_root = parent;
                                typename binary_search_tree<tkey, tvalue>::node* bst_grand_parent = reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node*>(grand_parent);

                                is_uncle_left ? this->_tree->small_left_rotation(bst_grand_parent) : this->_tree->small_right_rotation(bst_grand_parent);

                                if(!path.empty())
                                    grand_parent == (*path.top())->left_subtree ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                                else *_root = new_subtree_root;

                                change_node_color(parent);
                                change_node_color(grand_parent);
                                break;
                            }
                        }

                    }

                }
            }
        }
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            red_black_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
        red_black_tree<tkey, tvalue>::node** _root;

    public:
        
        explicit disposal_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
        tvalue dispose(tkey const &key) override;

    private:

        bool is_red(node* node)
        {
            if(!node) return false;
            return (node->color == node_color::RED)
        }

        void change_node_color(node * &node)
        {
            node->color = (is_red(node)) ? node_color::BLACK : node_color::RED;
        }
        
        void balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override
        {
            node* current = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*path.top());
            path.pop();

            if(!current->right_subtree && !current->left_subtree && is_red(current))
            {
                    current == (*path.top())->left_subtree ? (*path.top())->left_subtree = nullptr : (*path.top())->right_subtree = nullptr;
                    allocator::destruct(current);
                    this->deallocate_with_guard(current);
            }

            else if(!is_red(current) && (!current->left_subtree && current->right_subtree || current->left_subtree && !current->right_subtree))
            {
                    node* child = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(current->right_subtree ? current->right_subtree : current->left_subtree);

                    this->swap(std::move(current->key), std::move(child->key));
                    this->swap(std::move(current->value), std::move(child->value));

                    child == current -> left_subtree ? current->left_subtree = nullptr : current->right_subtree = nullptr;

                    allocator::destruct(child);
                    this->deallocate_with_guard(child);
            }

            else if(!is_red(current) && !current->left_subtree && !current->right_subtree)
            {
                    node* parent = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*path.top());
                    typename binary_search_tree<tkey, tvalue>::node* parent_bst = (*path.top());
                    path.pop();

                    bool is_disposal_node_left =  parent->left_subtree;

                    node* brother = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(is_disposal_node_left ? parent->right_subtree : parent->left_subtree);

                    is_disposal_node_left ? parent->left_subtree = nullptr : parent->right_subtree = nullptr;
                    allocator::destruct(current);
                    this->deallocate_with_guard(current);

                    if(!is_red(brother))
                    {
                        if(is_disposal_node_left && !is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->left_subtree)) && is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->right_subtree)) ||
                        !is_disposal_node_left && !is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->right_subtree)) && is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->left_subtree)))
                        {
                            if(is_red(parent)) brother->color = node_color::RED;
                            else brother->color = node_color::BLACK;

                            node* new_subtree_root = brother;

                            node* brother_child = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(is_disposal_node_left ? brother->right_subtree : brother->left_subtree);

                            is_disposal_node_left ? this->_tree->small_right_rotation(parent_bst) : this->_tree->small_left_rotation(parent_bst);

                            parent->color = node_color::BLACK;
                            brother_child->color = node_color::BLACK;

                            if(!path.empty())
                                parent == (*path.top())->left_subtree ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else *_root = new_subtree_root;

                            is_disposal_node_left ? parent->left_subtree = nullptr : parent->right_subtree = nullptr;
                            allocator::destruct(current);
                            this->deallocate_with_guard(current);
                        }

                        else if(is_disposal_node_left && is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->left_subtree)) && !is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->right_subtree)) ||
                        !is_disposal_node_left && is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->right_subtree)) && !is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->left_subtree)))
                        {

                            node* brother_child = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(is_disposal_node_left ? brother->left_subtree : brother->right_subtree);

                            if(is_red(parent)) brother_child->color = node_color::RED;
                            else brother_child->color = node_color::BLACK;

                            node* new_subtree_root = brother_child;

                            is_disposal_node_left ? this->_tree->big_left_rotation(parent_bst) : this->_tree->big_right_rotation(parent_bst);

                            parent->color = node_color::BLACK;

                            if(!path.empty())
                                parent == (*path.top())->left_subtree ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else *_root = new_subtree_root;

                            is_disposal_node_left ? parent->left_subtree = nullptr : parent->right_subtree = nullptr;
                            allocator::destruct(current);
                            this->deallocate_with_guard(current);
                        }

                        else if(!is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->left_subtree)) && !is_red(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(brother->right_subtree)))
                        {
                            if(!is_red(parent))
                            {
                                node* tmp_parent = parent;
                                while(true)
                                {
                                    if(parent == *_root)
                                    {
                                        brother->color = node_color::RED;
                                        break;
                                    }

                                    else
                                    {
                                        parent->color = node_color::RED;

                                        node* new_subtree_root = brother;

                                        is_disposal_node_left ? this->_tree->small_left_rotation(parent_bst) : this->_tree->small_right_rotation(parent_bst);

                                        if(!path.empty())
                                            parent == (*path.top())->left_subtree ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                                        else *_root = new_subtree_root;

                                    }
                                    parent = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*(path.top()));
                                    parent_bst = *(path.top());
                                    brother = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(is_disposal_node_left ? parent->right_subtree : parent->right_subtree);
                                    path.pop();
                                }

                                is_disposal_node_left ? tmp_parent->left_subtree = nullptr : tmp_parent->right_subtree = nullptr;
                                allocator::destruct(current);
                                this->deallocate_with_guard(current);
                            }
                            else
                            {
                                change_node_color(parent);
                                change_node_color(brother);

                                is_disposal_node_left ? parent->left_subtree = nullptr : parent->right_subtree = nullptr;

                                change_node_color(reinterpret_cast<red_black_tree<tkey, tvalue>::node*&>(is_disposal_node_left ? brother->right_subtree : brother->left_subtree));

                                allocator::destruct(current);
                                this->deallocate_with_guard(current);
                            }

                        }
                    }

                    else
                    {
                        change_node_color(parent);
                        change_node_color(brother);

                        is_disposal_node_left ? this->_tree->small_right_rotation(parent_bst) : this->_tree->small_left_rotation(parent_bst);

                        node* new_subtree_root = brother;

                        if(!path.empty())
                            parent == (*path.top())->left_subtree ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                        else *_root = new_subtree_root;

                        is_disposal_node_left ? parent->left_subtree = nullptr : parent->right_subtree = nullptr;
                        allocator::destruct(current);
                        this->deallocate_with_guard(current);
                    }
            }
        }


    };

public:
    
    explicit red_black_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

private:
    size_t get_node_size() const noexcept final
    {
        return sizeof(typename red_black_tree<tkey, tvalue>::node);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey, tvalue>::node* raw_space,
            tkey const&key,
            tvalue const& value) override
    {
        allocator::construct(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(raw_space), key, value);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey, tvalue>::node* raw_space,
            tkey const&key,
            tvalue && value) override
    {
        allocator::construct(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
    }

    void inject_additional_data(
            typename binary_search_tree<tkey, tvalue>::iterator_data *destination,
            typename binary_search_tree<tkey, tvalue>::node* source) override
    {
        static_cast<red_black_tree<tkey, tvalue>::iterator_data*>(destination)->color = static_cast<red_black_tree<tkey, tvalue>::node*>(source)->color;
    }

    typename binary_search_tree<tkey, tvalue>::iterator_data* create_iterator_data() const override
    {
        return new iterator_data;
    }

public:
    
    ~red_black_tree() noexcept final;
    
    red_black_tree(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept;
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> &&other) noexcept;
};

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    typename red_black_tree<tkey, tvalue>::node_color color):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
    color(color);
{
    
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy),
    _root(reinterpret_cast<red_black_tree<tkey, tvalue>::node**>(&tree->_root))
{
   
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    red_black_tree<tkey, tvalue> *tree): 
    binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(tree)
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy),
    _root(reinterpret_cast<red_black_tree<tkey, tvalue>::node**>(&tree->_root))
{

}

template<
        typename tkey,
        typename tvalue>
tvalue red_black_tree<tkey, tvalue>::disposal_template_method::dispose(const tkey &key)
{
    auto path = this->find_path(key);
    if((*path.top()) == nullptr)
    {
        switch (this->_disposal_strategy)
        {
            case binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception(key);
            case binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }

    if((*path.top())->right_subtree && (*path.top())->right_subtree)
    {
        auto *target_to_swap = *(path.top());
        auto **current = &((*(path.top()))->left_subtree);

        while(*current)
        {
            path.push(current);
            current = &((*current)->right_subtree);
        }

        this->swap(std::move(target_to_swap->key), std::move((*(path.top()))->key));
        this->swap(std::move(target_to_swap->value), std::move((*(path.top()))->value));
    }

    tvalue value = std::move((*(path.top()))->value);
    this->balance(path);

    return value;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    allocator *allocator,
    logger *logger,
    std::function<int(tkey const &, tkey const &)> comparer,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) :
        binary_search_tree<tkey, tvalue>::binary_search_tree(
                new typename red_black_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new typename red_black_tree<tkey, tvalue>::obtaining_template_method(this),
                new typename red_black_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
                comparer,
                allocator,
                logger)
{

}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
