#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H

#include <iostream>
#include <functional>
#include <stack>
#include <vector>

#include "../allocator/include/allocator.h"
#include "../allocator/include/allocator_guardant.h"
#include "associative_container.h"
#include "../logger/logger/include/logger.h"
#include "../logger/logger/include/logger_guardant.h"

template<
    typename tkey,
    typename tvalue>
class search_tree:
    public associative_container<tkey, tvalue>,
    protected allocator_guardant,
    protected logger_guardant
{

public:
    
    struct __attribute__((unused)) common_node
    {
    
    public:
        
        typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values;
        
        common_node **subtrees;
        
        size_t virtual_size;
    
    public:
    
        common_node() = default;

        common_node(typename associative_container<tkey, tvalue>::key_value_pair *key_value_pair,
                    common_node **subtrees,
                    size_t t);

        virtual ~common_node() noexcept;
        
    };

protected:
    
    std::function<int(tkey const &, tkey const &)> _keys_comparer;

protected:
    
    logger *_logger;
    
    allocator *_allocator;

protected:
    common_node *_root;

protected:

    std::pair<
        common_node *,
        typename associative_container<tkey, tvalue>::key_value_pair> node_split(
            common_node *node,
            typename associative_container<tkey, tvalue>::key_value_pair &&_key_value_pair,
            size_t subtree_index,
            common_node *right_subtree);

    void merge_nodes(common_node *parent, int left_index);

    std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node **, int>> find_path(tkey const &key);

    common_node *create_node(
        size_t t) const
    {
        auto *keys_and_values = reinterpret_cast<typename associative_container<tkey, tvalue>::key_value_pair *>(allocate_with_guard(
            sizeof(typename associative_container<tkey, tvalue>::key_value_pair), 2 * t - 1));

        auto *subtrees = reinterpret_cast<typename search_tree<tkey, tvalue>::common_node **>(allocate_with_guard(
            sizeof(typename search_tree<tkey, tvalue>::common_node *), 2 * t));

        auto *node = reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(allocate_with_guard(
            sizeof(typename search_tree<tkey, tvalue>::common_node), 1));
        allocator::construct(node, keys_and_values, subtrees, t);

        return node;
    }

    void destroy_node(
    common_node *to_destroy)
    {
        for (auto i = 0; i < to_destroy->virtual_size; ++i)
        {
            allocator::destruct(to_destroy->keys_and_values + i);
        }

        deallocate_with_guard(to_destroy->keys_and_values);
        deallocate_with_guard(to_destroy->subtrees);
        allocator::destruct(to_destroy);
        deallocate_with_guard(to_destroy);
    }
    
    explicit search_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        logger *logger = nullptr,
        allocator *allocator = nullptr);
    
public:
    
    virtual std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) = 0;

protected:
    
    [[nodiscard]] inline allocator *get_allocator() const final;
    
    [[nodiscard]] inline logger *get_logger() const final;

    int find_path_node(common_node const *node, tkey const &key, size_t left_bound, size_t right_bound);

    void node_insert(common_node *node, typename associative_container<tkey, tvalue>::key_value_pair &&_key_value_pair,
                     size_t index, common_node *right_subtree);

    template <typename T>
    static void swap(T &&first, T &&second) {
        T tmp = std::forward<T>(first);
        first = std::forward<T>(second);
        second = std::move(tmp);
    }

};

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::common_node::common_node(
    typename associative_container<tkey, tvalue>::key_value_pair * key_value_pair,
    common_node **subtrees,
    size_t t):  keys_and_values(key_value_pair),
                subtrees(subtrees),
                virtual_size(0)
{
    for (auto i = 0; i < 2 * t; i++)
    {
        subtrees[i] = nullptr;
    }
}



template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::common_node::~common_node() noexcept
{
    virtual_size = 0;
}

// endregion search_tree<tkey, tvalue>::node implementation

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    logger *logger,
    allocator *allocator): _keys_comparer(keys_comparer), _logger(logger), _allocator(allocator), _root(nullptr)
{

}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *search_tree<tkey, tvalue>::get_allocator() const
{
    return nullptr;
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *search_tree<tkey, tvalue>::get_logger() const
{
    return this->_logger;
}

template<
    typename tkey,
    typename tvalue>
int search_tree<tkey, tvalue>::find_path_node(
    common_node const *node,
    tkey const &key,
    size_t left_bound,
    size_t right_bound)
{
    while (true)
    {
        int index = static_cast<int>(left_bound + right_bound) / 2;
        auto comparison_result = _keys_comparer(key, node->keys_and_values[index].key);
        if (comparison_result == 0)
        {
            return index;
        }

        if (left_bound == right_bound)
        {
            if (comparison_result < 0) {
                return -(index + 1);
            }
            return -(index + 2);
        }

        if (comparison_result < 0)
        {
            right_bound = index;
        }
        else
        {
            left_bound = index + 1;
        }
    }
}

template<
    typename tkey,
    typename tvalue>
void search_tree<tkey, tvalue>::node_insert(
    common_node *node,
    typename associative_container<tkey, tvalue>::key_value_pair &&_key_value_pair,
    size_t index,
    common_node *right_subtree)
{
    allocator::construct(node->keys_and_values + node->virtual_size, std::move(_key_value_pair));
    node->subtrees[node->virtual_size + 1] = right_subtree;

    for (auto i = 0; i < node->virtual_size - index; i++)
    {
        swap(std::move(node->keys_and_values[node->virtual_size - i]),
             std::move(node->keys_and_values[node->virtual_size - i - 1]));
        swap(std::move(node->subtrees[node->virtual_size + 1 - i]), std::move(node->subtrees[node->virtual_size - i]));
    }

    ++node->virtual_size;
}

template<
	typename tkey,
	typename tvalue>
std::pair<
    typename search_tree<tkey, tvalue>::common_node *,
    typename associative_container<tkey, tvalue>::key_value_pair> search_tree<tkey, tvalue>::node_split(
	common_node *node,
	typename associative_container<tkey, tvalue>::key_value_pair &&_key_value_pair,
	size_t subtree_index,
	common_node *right_subtree)
{
    unsigned long const t = (node->virtual_size + 1) / 2;
    unsigned long const median_index = t;

    if (subtree_index != median_index)
    {
        if (subtree_index < median_index) {
            swap(std::move(_key_value_pair), std::move(node->keys_and_values[median_index - 1]));
            swap(std::move(right_subtree), std::move(node->subtrees[median_index]));
        }
        else {
            swap(std::move(_key_value_pair), std::move(node->keys_and_values[median_index]));
            swap(std::move(right_subtree), std::move(node->subtrees[median_index + 1]));
        }
    }

    unsigned long index = median_index;
    const int index_increment = index < subtree_index
                                    ? 1
                                    : -1;
    if (index_increment == -1)
    {
	    --index;
    }

    if (index < subtree_index)
    {
	    while (index + 1 != subtree_index)
	    {
	        swap(std::move(node->keys_and_values[index]), std::move(node->keys_and_values[index + 1]));
	        swap(std::move(node->subtrees[index + 1]), std::move(node->subtrees[index + 2]));
	        ++index;
	    }
    }
    else
    {
	    while (index != subtree_index)
	    {
	        swap(std::move(node->keys_and_values[index]), std::move(node->keys_and_values[index - 1]));
	        swap(std::move(node->subtrees[index + 1]), std::move(node->subtrees[index]));
	        --index;
	    }
    }

    common_node *new_node = create_node(t);
    for (auto i = 0; i < t - 1; i++)
    {
	    allocator::construct(new_node->keys_and_values + i, std::move(node->keys_and_values[t + i]));
	    allocator::destruct(node->keys_and_values + t + i);

	    swap(std::move(new_node->subtrees[1 + i]), std::move(node->subtrees[t + 1 + i]));
    }

    new_node->subtrees[0] = right_subtree;

    new_node->virtual_size = t - 1;
    node->virtual_size = t;

    return std::make_pair(new_node, std::move(_key_value_pair));
}

template<
    typename tkey,
    typename tvalue>
void search_tree<tkey, tvalue>::merge_nodes(
    common_node *parent,
    int left_index)
{
    common_node *left_subtree = parent->subtrees[left_index];
    common_node *right_subtree = parent->subtrees[left_index + 1];

    allocator::construct(left_subtree->keys_and_values + left_subtree->virtual_size + 1,
                            std::move(parent->keys_and_values[left_index]));

    for (int i = left_index; i < parent->virtual_size; ++i)
    {
        search_tree::swap(std::move(parent->keys_and_values[i]),
                          std::move(parent->keys_and_values[i + 1]));
        search_tree::swap(std::move(parent->subtrees[i + 1]),
                          std::move(parent->subtrees[i + 2]));
    }
    allocator::destruct(parent->keys_and_values + parent->virtual_size);
    for (auto i = 0; i < right_subtree->virtual_size; i++)
    {
        allocator::construct(left_subtree->keys_and_values + left_subtree->virtual_size,
                                std::move(right_subtree->keys_and_values[i]));
        left_subtree->subtrees[left_subtree->virtual_size++] = right_subtree->subtrees[i];
    }
    left_subtree->subtrees[left_subtree->virtual_size] = right_subtree->subtrees[right_subtree->virtual_size];

    destroy_node(right_subtree);
}

template<
    typename tkey,
    typename tvalue>
std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node **, int>> search_tree<tkey, tvalue>::find_path(
    tkey const &key)
{
    std::stack<std::pair<common_node **, int>> result;
    int index = -1;
    if (_root == nullptr)
    {
        result.push(std::pair<common_node **, int>(&_root, index));
        return result;
    }
    common_node **iter = &_root;
    while (*iter != nullptr && index < 0)
    {
        index = find_path_node(*iter, key, 0, (*iter)->virtual_size - 1);
        result.push(std::pair<common_node **, int>(iter, index));
        if (index < 0)
        {
            iter = (*iter)->subtrees - index - 1;
        }
    }
    return result;
}




#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H