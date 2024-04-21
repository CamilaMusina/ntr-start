#include <iostream>
#include <memory>
#include <stdexcept>

template<typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr) {}
    ~LinkedList();

    bool empty() const;                          // проверка списка на пустоту

    void push_front(const T& val);               // добавление элемента в начало списка
    void push_back(const T& val);                // добавление элемента в конец списка
    void insert(const T& val, std::size_t pos);  // добавление элемента после любого другого элемента списка
    template<typename... Args>
    void emplace_front(Args&&... args);          // вставка нескольких элементов в начало списка
    template<typename... Args>
    void emplace_back(Args&&... args);           // вставка нескольких элементов в конец списка

    void print() const;                          // вывод элементов списка

    T front() const;                              // возвращает первый элемент списка
    LinkedList<T> tail() const;                  // доступ ко всем элементам списка, кроме первого

    typename LinkedList<T>::Node* findVal(const T& val) const;           // поиск узла по значению
    typename LinkedList<T>::Node* findAt(std::size_t pos) const;         // поиск узла по индексу

    void pop_front();                            // удаление первого узла
    void pop_back();                             // удаление последнего узла
    void removeVal(const T& val);                // удаление по значению
    void removeAt(std::size_t pos);              // удаление по индексу узла
    void clear();                                // очистка списка

    std::size_t size() const;                    // количество элементов в списке

    void reverse();                              // переворачивание списка

private:
    struct Node;

    std::unique_ptr<Node> head;
};

template<typename T>
struct LinkedList<T>::Node {
    T data;
    std::unique_ptr<Node> next;

    Node(const T& data) : data(data), next(nullptr) {};
};

template<typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template<typename T>
bool LinkedList<T>::empty() const {
    return !head;
}

template<typename T>
void LinkedList<T>::push_front(const T& val) {
    auto newNode = std::make_unique<Node>(val);
    newNode->next = std::move(head);
    head = std::move(newNode);
}

template<typename T>
void LinkedList<T>::push_back(const T& val) {
    auto newNode = std::make_unique<Node>(val);

    if (empty()) {
        head = std::move(newNode);
        return;
    }

    Node* curr = head.get();
    while (curr->next) {
        curr = curr->next.get();
    }
    curr->next = std::move(newNode);
}

template<typename T>
void LinkedList<T>::insert(const T& val, std::size_t pos) {
    if (pos == 0) {
        push_front(val);
        return;
    }

    auto newNode = std::make_unique<Node>(val);
    Node* curr = head.get();

    for (std::size_t i = 0; i < pos - i && curr; i++) {
        curr = curr->next.get();
    }
    if (!curr) {
        throw std::out_of_range("Position is out of range");
    }

    newNode->next = std::move(curr->next);
    curr->next = std::move(newNode);
}

template<typename T>
template<typename... Args>
void LinkedList<T>::emplace_front(Args&&... args) {
    head = std::make_unique<Node>(std::forward<Args>(args)..., std::move(head));
}

template<typename T>
template<typename... Args>
void LinkedList<T>::emplace_back(Args&&... args) {
    auto newNode = std::make_unique<Node>(std::forward<Args>(args)...);

    if (empty()) {
        head = std::move(newNode);
        return;
    }

    Node* curr = head.get();
    while (curr->next) {
        curr = curr->next.get();
    }
    curr->next = std::move(newNode);
}

template<typename T>
void LinkedList<T>::print() const {
    Node* curr = head.get();
    while (curr) {
        std::cout << curr->data << " ";
        curr = curr->next.get();
    }
    std::cout << std::endl;
}

template<typename T>
T LinkedList<T>::front() const {
    if (empty()) {
        throw std::runtime_error("List is empty");
    }
    return head->data;
}

template<typename T>
LinkedList<T> LinkedList<T>::tail() const {
    LinkedList<T> resList;

    if (!head || !head->next)
        return std::runtime_error("List or list's tail is empty");

    Node* curr = head->next.get();
    while (curr) {
        resList.push_back(curr->data);
        curr = curr->next.get();
    }

    return resList;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::findVal(const T& val) const {
    Node* curr = head.get();
    while (curr) {
        if (curr->data == val) {
            return curr;
        }
        curr = curr->next.get();
    }
    return nullptr;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::findAt(std::size_t pos) const {
    if (pos >= size()) {
        throw std::out_of_range("Position is out of range");
    }
    if (!head) {
        throw std::runtime_error("List is empty");
    }

    Node* curr = head.get();
    for (size_t i = 0; i < pos; i++) {
        curr = curr->next.get();
    }
    return curr;
}

template<typename T>
void LinkedList<T>::pop_front() {
    if (empty()) {
        throw std::runtime_error("List is empty");
    }
    head = std::move(head->next);
}

template<typename T>
void LinkedList<T>::pop_back() {
    if (empty()) {
        throw std::runtime_error("List is empty");
    }

    if (!head->next) {
        head.reset();
        return;
    }

    Node* curr = head.get();
    while (curr->next->next) {
        curr = curr->next.get();
    }
    curr->next.reset();
}

template<typename T>
void LinkedList<T>::removeVal(const T& val) {
    if (empty()) {
        throw std::runtime_error("List is empty");
    }

    if (head->data == val) {
        pop_front();
        return;
    }

    Node* curr = head.get();
    while (curr->next && curr->next->data != val) {
        curr = curr->next.get();
    }

    if (!curr->next) {
        throw std::runtime_error("Value not found in the list");
    }

    auto tmp = std::move(curr->next);
    curr->next = std::move(tmp->next);
    tmp.reset();
}

template<typename T>
void LinkedList<T>::removeAt(size_t pos) {
    if (pos == 0) {
        pop_front();
        return;
    }

    Node* prev = findAt(pos - 1);
    auto tmp = std::move(prev->next);
    if (!tmp) {
        throw std::out_of_range("Position is out of range");
    }
    prev->next = std::move(tmp->next);
    tmp.reset();
}

template<typename T>
void LinkedList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}

template<typename T>
std::size_t LinkedList<T>::size() const {
    std::size_t count = 0;
    Node* curr = head.get();
    while (curr) {
        ++count;
        curr = curr->next.get();
    }
    return count;
}

template<typename T>
void LinkedList<T>::reverse() {
    if (empty() || !head->next) {
        return;
    }

    Node* prev = nullptr;
    Node* curr = head.get();

    while (curr) {
        auto next = curr->next.release();
        curr->next.reset(prev);
        prev = curr;
        curr = next;
    }
    head.reset(prev);
}

int main() {
    return 0;
}
