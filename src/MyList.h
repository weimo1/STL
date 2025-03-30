#include <cstddef>
#include <iterator>
template<typename T>
struct Node{
    T data;
    Node* next;
    Node* prev;

    Node(const T& value=T()):data(value), prev(nullptr), next(nullptr) {}
};


template <typename T>
class List;

template<typename T>
class Iterator{
    public:
    using self_type=Iterator<T>;
    using value_type=T;
    using reference=T&;
    using pointer=T*;
    using iterator_category=std::bidirectional_iterator_tag;
    using difference_type=std::ptrdiff_t;

    Iterator(Node<T>* ptr=nullptr):node_ptr(ptr){}

    reference operator*() const { return node_ptr->data; }

    pointer operator->() const { return &(node_ptr->data); }

    self_type& operator++() {
        if (node_ptr) node_ptr = node_ptr->next;
        return *this;
    }

    self_type operator++(int) {
        self_type temp = *this;
        ++(*this);
        return temp;
    }

    self_type& operator--() {
        if (node_ptr) node_ptr = node_ptr->prev;
        return *this;
    }

    self_type operator--(int) {
        self_type temp = *this;
        --(*this);
        return temp;
    }
    bool operator==(const self_type& other) const {
        return node_ptr == other.node_ptr;
    }

    bool operator!=(const self_type& other) const {
        return node_ptr != other.node_ptr;
    }

    private:
    Node<T>* node_ptr;
    friend class List<T>;
};


template<typename T>
class List{
    public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<T>;

    List(){
        head = new Node<T>();
        tail = new Node<T>(); 
        head->next = tail;
        tail->prev = head;
    }
    ~List() {
        clear();
        delete head;
        delete tail;
    }
    List(const List& other) = delete;
    List& operator=(const List& other) = delete;

    iterator insert(iterator pos, const T& value) {
        Node<T>* current = pos.node_ptr;
        Node<T>* new_node = new Node<T>(value);

        Node<T>* prev_node = current->prev;

        new_node->next = current;
        new_node->prev = prev_node;

        prev_node->next = new_node;
        current->prev = new_node;

        return iterator(new_node);
    }
    iterator erase(iterator pos) {
        Node<T>* current = pos.node_ptr;
        if (current == head || current == tail) {
            // 不能删除哨兵节点
            return pos;
        }

        Node<T>* prev_node = current->prev;
        Node<T>* next_node = current->next;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        delete current;

        return iterator(next_node);
    }
    void push_front(const T& value) {
        insert(begin(), value);
    }
    void push_back(const T& value) {
        insert(end(), value);
    }
    void pop_front() {
        if (!empty()) {
            erase(begin());
        }
    }

    void pop_back() {
        if (!empty()) {
            iterator temp = end();
            --temp;
            erase(temp);
        }
    }
    T& front() {
        return head->next->data;
    }

    // 获取尾元素引用
    T& back() {
        return tail->prev->data;
    }

    // 判断是否为空
    bool empty() const {
        return head->next == tail;
    }

    // 获取链表大小（O(n)复杂度）
    size_t size() const {
        size_t count = 0;
        for(auto it = begin(); it != end(); ++it) {
            ++count;
        }
        return count;
    }

    void clear() {
        Node<T>* current = head->next;
        while(current != tail) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        head->next = tail;
        tail->prev = head;
    }

    iterator begin() {
        return iterator(head->next);
    }

    // 获取结束迭代器
    iterator end() {
        return iterator(tail);
    }


    private:
    Node<T>* head; 
    Node<T>* tail; 
};