#include <bits/stdc++.h>
#include <functional>

template<typename T>
class skipList {
    struct skipListNode {
        T val;
        skipListNode **_prev, **_next;
        explicit skipListNode(T value_, int8_t size) {
            val = value_;
            _prev = new skipListNode *[size];
            _next = new skipListNode *[size];
            for (int8_t i = 0; i < size; i++) {
                _prev[i] = _next[i] = nullptr;
            }
        }
        ~skipListNode() {
            delete next();
            delete[] _prev;
            delete[] _next;
        }
        skipListNode *next(int8_t lvl = 0) const {
            return _next[lvl];
        }
        skipListNode *prev(uint8_t lvl = 0) const {
            return _prev[lvl];
        }
        static void connect(skipListNode *A, skipListNode *B, uint8_t level) {
            if (B) B->_prev[level] = A;
            if (A) A->_next[level] = B;
        }
        static void connect3(skipListNode *A, skipListNode *B, skipListNode *C, uint8_t level) {
            connect(A, B, level);
            connect(B, C, level);
        }
    };
    class skipListIterator : public std::iterator<std::bidirectional_iterator_tag, skipList<T>> {
        skipListNode *_nodeptr;
    public:
        explicit skipListIterator(skipListNode *N) {
            _nodeptr = N;
        }
        skipListNode *operator~() {
            return _nodeptr;
        }
        T &operator*() {
            return _nodeptr->val;
        }
        const T &operator*() const {
            return _nodeptr->val;
        }
        skipListIterator &operator++() {
            _nodeptr = _nodeptr->next();
            return *this;
        }
        skipListIterator &operator--() {
            _nodeptr = _nodeptr->prev();
            return *this;
        }
        bool operator==(const skipListIterator &other) const {
            return _nodeptr == other._nodeptr;
        }
        bool operator!=(const skipListIterator &other) const {
            return _nodeptr != other._nodeptr;
        }
    };

    std::function<int(T, T)> compare;
    uint64_t _stored = 0;
    int8_t _maxDepth{}, _depth{};
    float _prob{};
    skipListNode *_head, *_tail;
public:
    explicit skipList(int8_t maxDepth = 6, float probability = .25, std::function<int(T, T)> cmp = [](T a, T b) { return a - b; }) {
        compare = cmp;
        _maxDepth = maxDepth;
        _prob = probability;
        _depth = 1;
        _tail = _head = new skipListNode(-1, _maxDepth);
    }
    void copy(const skipList<T> &src) {
        compare = src.compare;
        _maxDepth = src._maxDepth;
        _depth = src._depth;
        _stored = src._stored;
        _tail = src._tail;
        _prob = src._prob;
        _head = new skipListNode(-1, _maxDepth);
        auto **mostRight = new skipListNode *[_maxDepth];
        auto **mostRightSrc = new skipListNode *[_maxDepth];
        for (int i = 0; i < _maxDepth; i++) {
            mostRight[i] = _head;
            mostRightSrc[i] = src._head;
        }
        skipListNode *temp = _head;
        skipListNode *tempSrc = src._head;
        while (tempSrc) {
            for (int i = 0; i < _maxDepth; i++) {
                if (mostRightSrc[i]->_next[i] == tempSrc) {
                    skipListNode::connect(mostRight[i], temp, i);
                    mostRightSrc[i] = tempSrc;
                    mostRight[i] = temp;
                }
            }
            tempSrc = tempSrc->_next[0];
            if (tempSrc) {
                temp->_next[0] = new skipListNode(tempSrc->val, _maxDepth);
            }
            temp = temp->_next[0];
        }
        delete[] mostRight;
        delete[] mostRightSrc;
    }
    skipList(const skipList<T> &src) {
        copy(src);
    }
    void move(skipList<T> &&src) {
        _maxDepth = src._maxDepth;
        _prob = src._prob;
        _depth = src._depth;
        src._depth = 0;
        _head = src._head;
        _tail = src._tail;
        _stored = src._stored;
        src._stored = 0;
        src._tail = src._head = nullptr;
        compare = src.compare;
    }
    skipList(skipList<T> &&src) noexcept {
        move();
    }
    skipList &operator=(skipList<T> &&src) noexcept {
        if (&src != this) {
            delete[] _head;
            move();
        }
        return *this;
    }

    skipList<T> &operator=(const skipList<T> &other) {
        if (this == &other) return *this;
        delete _head;
        copy(other);
        return *this;
    }
    ~skipList() {
        delete _head;
    }
    uint64_t size() const {
        return _stored;
    }
    bool empty() const {
        return !(_head->next());
    }
    bool getChance() const {
        return (((float) rand()) / RAND_MAX) < _prob;
    }
    void insert(T value) {
        skipListNode **mostRight = new skipListNode *[_maxDepth];
        for (int8_t i = 0; i < _maxDepth; i++) {
            mostRight[i] = _head;
        }
        skipListNode *P = _head, *P_;
        for (int8_t L = _depth - 1; L >= 0; L--) {
            P_ = P->next(L);
            while (P_ != nullptr && compare(P_->val, value) < 0) {
                P = P_;
                P_ = P->next(L);
            }
            mostRight[L] = P;
        }
        skipListNode *newNode = new skipListNode(value, _maxDepth);
        skipListNode::connect3(mostRight[0], newNode, mostRight[0]->next(0), 0);
        for (int8_t i = 1; (i < _maxDepth) && getChance(); i++) {
            skipListNode::connect3(mostRight[i], newNode, mostRight[i]->next(i), i);
            if (i + 1 > _depth)_depth = i + 1;
        }
        _stored++;
        if (mostRight[0] == _tail)_tail = P;
        delete[] mostRight;
    }
    void print() {
        for (int8_t l = _maxDepth - 1; l >= 0; l--) {
            printf("lvl #%d: ", l);
            skipListNode *P = _head->next(l);
            while (P) {
                std::cout << P->val << ' ';
                P = P->next(l);
            }
            printf("\n");
        }
        printf("\n");
    }
    void clear() {
        delete _head;
        _tail = _head = new skipListNode(-1, _maxDepth);
        _depth = 1;
        _stored = 0;
    }

    skipListIterator begin() {
        return skipListIterator(_head->next());
    }
    skipListIterator end() {
        return skipListIterator(nullptr);
    }
    skipListIterator find(T value) {
        skipListNode *P = _head, *P_;
        for (int8_t L = _depth - 1; L >= 0; L--) {
            P_ = P->next(L);
            while (P_ != nullptr) {
                if (compare(P_->val, value) < 0) {
                    P = P_;
                    P_ = P->next(L);
                } else if (compare(P_->val, value) == 0) {
                    return skipListIterator(P_);
                } else {
                    break;
                }
            }
        }
        return end();
    }
    skipListIterator lower_bound(T value) {
        skipListNode *P = _head, *P_;
        for (int8_t L = _depth - 1; L >= 0; L--) {
            P_ = P->next(L);
            while (P_ != nullptr) {
                if (compare(P_->val, value) < 0) {
                    P = P_;
                    P_ = P->next(L);
                } else {
                    break;
                }
            }
        }
        return skipListIterator(P_);
    }
    skipListIterator upper_bound(T value) {
        skipListNode *P = _head, *P_;
        for (int8_t L = _depth - 1; L >= 0; L--) {
            P_ = P->next(L);
            while (P_ != nullptr) {
                if (compare(P_->val, value) <= 0) {
                    P = P_;
                    P_ = P->next(L);
                } else {
                    break;
                }
            }
        }
        return skipListIterator(P_);
    }
    void erase(skipListIterator N_) {
        skipListNode *N = ~N_;
        for (int8_t i = 0; i < _maxDepth; i++) {
            skipListNode::connect(N->prev(i), N->next(i), i);
            N->_next[i] = nullptr;
        }
        delete N;
    }
    void erase(skipListIterator from, skipListIterator to) {
        skipListIterator to_ = to;
        ++to_;
        while (from != to_) {
            skipListIterator temp = from;
            ++temp;
            erase(from);
            from = temp;
        }
    }
    void erase(std::pair<skipListIterator, skipListIterator> zone) {
        erase(zone.first, zone.second);
    }
    uint64_t count(T value) {
        skipListIterator Nl = find(value);
        if (Nl == end())return 0;
        uint64_t C = 1;
        skipListIterator Nr = Nl, Nt = Nr;
        while (*++Nt == *Nr) {
            Nr = Nt;
            C++;
        }
        Nt = Nl;
        while (*--Nt == *Nl) {
            Nl = Nt;
            C++;
        }
        return C;
    }
    std::pair<skipListIterator, skipListIterator> equal_range(T value) {
        skipListIterator Nl = find(value);
        if (Nl == end()) std::make_pair(Nl, Nl);
        skipListIterator Nr = Nl, Nt = Nr;
        while (*++Nt == *Nr) {
            Nr = Nt;
        }
        Nt = Nl;
        while (*--Nt == *Nl) {
            Nl = Nt;
        }
        return std::make_pair(Nl, Nr);
    }
};


int main() {
    skipList<int> list(4, 0.25, [](int a, int b) { return a - b; });
    {
        list.insert(7);
        list.insert(8);
        list.insert(2);
        list.insert(3);
        list.insert(5);
        list.insert(6);
        list.insert(11);
        list.insert(4);
        list.insert(9);
        list.insert(21);
        list.insert(22);
        list.insert(23);
        list.insert(8);
        list.insert(16);
        list.insert(16);
        list.insert(1);
        list.insert(13);
        list.insert(12);
        list.insert(13);
        list.insert(14);
        list.insert(15);
        list.insert(16);
        list.insert(17);
        list.insert(11);
        list.insert(19);
        list.insert(20);
    }
    list.print();
    printf(">%lu\n", list.count(16));
    list.erase(list.equal_range(11));
    list.print();
    list.erase(list.equal_range(16));
    list.print();

    printf("from 11: ");
    for (auto x = list.upper_bound(11); x != list.end(); ++x) {
        std::cout << *x << " ";
    }
    printf("\n(for :) ");
    for (auto x: list) {
        std::cout << x << " ";
    }
    return 0;
}