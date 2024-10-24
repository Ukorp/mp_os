#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H

#include "search_tree.h"
template<
    typename tkey,
    typename tvalue>
class b_tree final:
    public search_tree<tkey, tvalue>
{
private:

    size_t _t;

public:

    class infix_iterator final
    {

    public:

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator operator++(
            int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue &> operator*() const;

    private:

        std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node *, int>> _path;

    public:

        explicit infix_iterator(
            typename search_tree<tkey, tvalue>::common_node *subtree_root);


    };

    class infix_const_iterator final
    {
    private:
        infix_iterator _iterator;

    public:

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator operator++(
            int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue const &> operator*() const;

    public:

        explicit infix_const_iterator(
    typename search_tree<tkey, tvalue>::common_node *subtree_root);

    };

public:
    void insert_inner(typename associative_container<tkey, tvalue>::key_value_pair &&kvp);

    void insert(
        tkey const &key,
        tvalue const &value) override;

    void insert(
        tkey const &key,
        tvalue &&value) override;

    tvalue &obtain(
        tkey const &key) override;

    tvalue dispose(
        tkey const &key) override;

    typename search_tree<tkey, tvalue>::common_node *copy(typename search_tree<tkey, tvalue>::common_node *node);

    void clear(typename search_tree<tkey, tvalue>::common_node *node);

    void update(const tkey &key, const tvalue &value);

    void update(const tkey &key, tvalue &&value);

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;

public:

    explicit b_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr);

    b_tree(
        b_tree<tkey, tvalue> const &other);

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> const &other);

    b_tree(
        b_tree<tkey, tvalue> &&other) noexcept;

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> &&other) noexcept;

    ~b_tree();

public:

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    size_t max_keys() const noexcept;
};

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::infix_iterator::infix_iterator(
    typename search_tree<tkey, tvalue>::common_node *subtree_root)
{
    while (subtree_root != nullptr)
    {
        _path.push(std::make_pair(subtree_root, 0));
        subtree_root = subtree_root->subtrees[0];
    }
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    typename search_tree<tkey, tvalue>::common_node *subtree_root):
                                     _iterator(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator==(
    infix_iterator const &other) const noexcept
{
    if (_path.size() != other._path.size()) {
        return false;
    }
    if (_path.empty() && _path.empty()) return true;
    return _path.top() == _path.top();
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator!=(
    infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator &b_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if (_path.empty())
    {
        return *this;
    }

    auto *node = _path.top().first;
    int index = _path.top().second;

    if (node->subtrees[0] != nullptr)
    {
        index = ++_path.top().second;
        while (node->subtrees[index] != nullptr)
        {
            node = node->subtrees[index];
            index = 0;
            _path.push(std::make_pair(node, 0));
        }

        return *this;
    }

    if (index != node->virtual_size - 1)
    {
        ++_path.top().second;

        return *this;
    }

    do
    {
        _path.pop();

        if (!_path.empty())
        {
            node = _path.top().first;
            index = _path.top().second;
        }
    }
    while (!_path.empty() && index == node->virtual_size - (node->subtrees[0] == nullptr
                                    ? 1
                                    : 0));

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    infix_iterator it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue &> b_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    auto &_key_value_pair = _path.top().first->keys_and_values[_path.top().second];
    return std::tuple<size_t, size_t, tkey const &, tvalue &>(_path.size() - 1,
                                                              _path.top().second,
                                                              _key_value_pair.key,
                                                              _key_value_pair.value); // RIGHT??
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator==(
    b_tree::infix_const_iterator const &other) const noexcept
{
    return _iterator == other._iterator;
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    b_tree::infix_const_iterator const &other) const noexcept
{
   return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator &b_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    ++_iterator;
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue const &> b_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    auto data = *_iterator;
    return std::tuple<size_t, size_t, tkey const &, tvalue const &>(std::get<0>(data),
                                                                    std::get<1>(data),
                                                                    std::get<2>(data),
                                                                    std::get<3>(data));
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert_inner(
    typename associative_container<tkey, tvalue>::key_value_pair &&kvp)
{
    auto path = this->find_path(kvp.key);
    auto *node = *path.top().first;
    if (node == nullptr && path.size() == 1)
    {
        typename search_tree<tkey ,tvalue>::common_node *new_node;
        *path.top().first = new_node = this->create_node(_t);
        allocator::construct(new_node->keys_and_values, std::move(kvp));
        ++new_node->virtual_size;

        return;
    }

    if (path.top().second >= 0)
    {
        // TODO: update mechanics o_O
        throw std::logic_error("duplicate key");
    }

    size_t subtree_index = -path.top().second - 1;

    typename search_tree<tkey, tvalue>::common_node *right_subtree = nullptr;
    while (true)
    {
        if (node->virtual_size < max_keys())
        {
            this->node_insert(node, std::move(kvp), subtree_index, right_subtree);
            return;
        }

        auto res = this->node_split(node, std::move(kvp), subtree_index, right_subtree);
        right_subtree = res.first;
        kvp = std::move(res.second);

        if (path.size() == 1)
        {
            typename search_tree<tkey, tvalue>::common_node *new_root = this->create_node(_t);
            new_root->virtual_size = 1;
            allocator::construct(new_root->keys_and_values, std::move(kvp));
            new_root->subtrees[0] = node;
            new_root->subtrees[1] = right_subtree;
            *path.top().first = new_root;
            return;
        }

        path.pop();
        node = *path.top().first;
        subtree_index = -path.top().second - 1;
    }
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert(
    const tkey &key,
    const tvalue &value)
{
    insert_inner(std::move(typename associative_container<tkey, tvalue>::key_value_pair(key, value)));
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert(
    const tkey &key,
    tvalue &&value)
{
    insert_inner(std::move(typename associative_container<tkey, tvalue>::key_value_pair(key, std::move(value))));
}

template<
    typename tkey,
    typename tvalue>
tvalue &b_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    auto path = this->find_path(key);
    if (path.top().second < 0)
    {
        throw std::logic_error("tvalue const &b_tree<tkey, tvalue>::obtain -> key not found");
    }

    return (*path.top().first)->keys_and_values[path.top().second].value;
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;

    auto it = cbegin_infix();

    while ((it != cend_infix()) &&
           (this->_keys_comparer(upper_bound, std::get<2>(*it)) > (upper_bound_inclusive ? -1 : 0)))
    {
        if (this->_keys_comparer(lower_bound, std::get<2>(*it)) < (lower_bound_inclusive ? 1 : 0))
        {
            range.push_back(std::move(typename associative_container<tkey, tvalue>::key_value_pair(std::get<2>(*it), std::get<3>(*it))));
        }
        ++it;
    }

    for (auto const &elem: range) {
        std::cout << elem.key << " ";
    }

    return range;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    size_t t,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger) : search_tree<tkey, tvalue>(keys_comparer, logger, allocator), _t(t)
{
    if (_t < 2) {
        throw std::logic_error("constructor b_tree<tkey, tvalue>::b_tree(...) : t must be >= 2");
    }
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
b_tree<tkey, tvalue> const &other):
                                search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator()),
                                _t(other._t)
{

    this->_root = copy(other._root);
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(b_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
	    clear(this->_root);
	    this->_logger = other.get_logger();
	    this->_allocator = other.get_allocator();
	    this->_root = copy(other._root);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> &&other) noexcept:
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator()),
        _t(other._t)
{
    other._logger = nullptr;

    other._allocator = nullptr;

    this->_root = other._root;
    other._root = nullptr;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(
    b_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        clear();

        this->_keys_comparer = other._keys_comparer;

        this->_logger = other._logger;
        other._logger = nullptr;

        this->_allocator = other._allocator;
        other._allocator = nullptr;

        this->_root = other._root;
        other._root = nullptr;
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::~b_tree()
{
    clear(this->_root);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return infix_iterator(this->_root);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::end_infix() const noexcept
{
    return infix_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return infix_const_iterator(this->_root);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return infix_const_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose(
    const tkey &key)
{
    auto path = this->find_path(key);
    if (path.top().second < 0)
    {
        throw std::logic_error("tvalue b_tree<tkey, tvalue>::dispose -> key is not found");
    }

    size_t min_keys = _t - 1;

    if ((*path.top().first)->subtrees[0] != nullptr)
    {
        auto top_node = path.top();
        path.pop();
        typename search_tree<tkey, tvalue>::common_node **iterator = top_node.first;

        while (*iterator != nullptr)
        {
            auto index = (*iterator == *top_node.first)
                             ? top_node.second
                             : (*iterator)->virtual_size;
            path.push(std::make_pair(iterator, -index - 1));
            iterator = (*iterator)->subtrees + index;
        }

        search_tree<tkey, tvalue>::swap(std::move((*top_node.first)->keys_and_values[top_node.second]),
                                        std::move((*path.top().first)->keys_and_values[(*path.top().first)->virtual_size - 1]));
        path.top().second = -path.top().second - 2;
    }

    auto dispose_node = *path.top().first;
    auto _key_value_pair_dispose_index = path.top().second;
    path.pop();

    for (size_t i = _key_value_pair_dispose_index + 1; i < dispose_node->virtual_size; i++)
    {
        search_tree<tkey, tvalue>::swap(std::move(dispose_node->keys_and_values[i - 1]), std::move(dispose_node->keys_and_values[i]));
    }

    tvalue value = std::move(dispose_node->keys_and_values[--dispose_node->virtual_size].value);

    allocator::destruct(dispose_node->keys_and_values + dispose_node->virtual_size);

    while (true)
    {
        if (dispose_node->virtual_size >= min_keys)
        {
            return value;
        }

        if (path.size() == 0)
        {
            if (dispose_node->virtual_size == 0)
            {
                this->_root = dispose_node->subtrees[0];
                this->destroy_node(dispose_node);
            }

            return value;
        }

        typename search_tree<tkey, tvalue>::common_node *parent = *path.top().first;
        size_t position = -path.top().second - 1;
        path.pop();

        bool const left_brother_exists = (position != 0);
        bool const can_take_from_left_brother =
                left_brother_exists && (parent->subtrees[position - 1]->virtual_size > min_keys);

        bool const right_brother_exists = (position != parent->virtual_size);
        bool const can_take_from_right_brother =
                right_brother_exists && (parent->subtrees[position + 1]->virtual_size > min_keys);

        if (can_take_from_left_brother)
        {
            auto *left_brother = parent->subtrees[position - 1];
            search_tree<tkey, tvalue>::swap(std::move(parent->keys_and_values[position - 1]), std::move(left_brother->keys_and_values[left_brother->virtual_size - 1]));
            allocator::construct(dispose_node->keys_and_values + dispose_node->virtual_size, std::move(left_brother->keys_and_values[left_brother->virtual_size - 1]));
            search_tree<tkey, tvalue>::swap(std::move(left_brother->subtrees[left_brother->virtual_size]), std::move(dispose_node->subtrees[dispose_node->virtual_size]));
            dispose_node->subtrees[++dispose_node->virtual_size] = left_brother->subtrees[left_brother->virtual_size];
            for (auto i = dispose_node->virtual_size - 1; i > 0; --i)
            {
                search_tree<tkey, tvalue>::swap(std::move(dispose_node->keys_and_values[i]),
                                                std::move(dispose_node->keys_and_values[i - 1]));

                search_tree<tkey, tvalue>::swap(std::move(dispose_node->subtrees[i + 1]), std::move(dispose_node->subtrees[i]));
            }
            allocator::destruct(left_brother->keys_and_values + --left_brother->virtual_size);
            return value;
        }

        if (can_take_from_right_brother)
        {
            auto *right_brother = parent->subtrees[position + 1];
            search_tree<tkey, tvalue>::swap(std::move(parent->keys_and_values[position]), std::move(right_brother->keys_and_values[0]));

            allocator::construct(dispose_node->keys_and_values + dispose_node->virtual_size, std::move(right_brother->keys_and_values[0]));
            dispose_node->subtrees[++dispose_node->virtual_size] = right_brother->subtrees[0];

            for (size_t i = 0; i < right_brother->virtual_size - 1; ++i)
            {
                search_tree<tkey, tvalue>::swap(std::move(right_brother->keys_and_values[i]), std::move(right_brother->keys_and_values[i + 1]));

                search_tree<tkey, tvalue>::swap(std::move(right_brother->subtrees[i]), std::move(right_brother->subtrees[i + 1]));
            }

            right_brother->subtrees[right_brother->virtual_size - 1] = right_brother->subtrees[right_brother->virtual_size];

            allocator::destruct(right_brother->keys_and_values + --right_brother->virtual_size);

            return value;
        }

        if (left_brother_exists) {
            this->merge_nodes(parent, position - 1);
        }
        else {
            this->merge_nodes(parent, position);
        }

        dispose_node = parent;
    }
}

template<
    typename tkey,
    typename tvalue>
typename search_tree<tkey, tvalue>::common_node *b_tree<tkey, tvalue>::copy(
    typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    typename search_tree<tkey, tvalue>::common_node *copied = this->create_node(_t);
    copied->virtual_size = node->virtual_size;

    for (size_t i = 0; i < node->virtual_size; ++i)
    {
        allocator::construct(copied->keys_and_values + i, node->keys_and_values[i]);
    }

    for (size_t i = 0; i <= node->virtual_size; ++i)
    {
        copied->subtrees[i] = copy(node->subtrees[i]);
    }

    return copied;
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::clear(
    typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return;
    }
    for (size_t i = 0; i <= node->virtual_size; ++i)
    {
        clear(node->subtrees[i]);
    }
    search_tree<tkey, tvalue>::destroy_node(node);
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::update(
    const tkey &key, const tvalue &value)
{
    auto path = this->find_path(key);
    if (path.top().second < 0)
    {
        throw std::logic_error("void b_tree<tkey, tvalue>::update(const tkey&, const tvalue&): key is not found");
    }
    (*path.top().first)->keys_and_values[path.top().second].value = value;
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::update(
    const tkey &key, tvalue &&value)
{
    auto path = this->find_path(key);
    if (path.top().second < 0)
    {
        throw std::logic_error("void b_tree<tkey, tvalue>::update(const tkey&, const tvalue&): key is not found");
    }
    (*path.top().first)->keys_and_values[path.top().second].value = std::move(value);
}

template<
    typename tkey,
    typename tvalue>
inline size_t b_tree<tkey, tvalue>::max_keys() const noexcept
{
    return 2 * _t - 1;
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H