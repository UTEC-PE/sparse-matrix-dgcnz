#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <utility>

template <class T> class Node {

  T data;

public:
  int row, col;

public:
  typedef std::pair<int, int> coordinates;
  Node<T> *next;
  Node<T> *down;

  explicit Node(T data_, int row_, int col_, Node<T> *next_ = nullptr,
                Node<T> *down_ = nullptr)
      : data(data_), row(row_), col(col_), next(next_), down(down_) {}

  explicit Node(Node<T> *next_ = nullptr, Node<T> *down_ = nullptr)
      : next(next_), down(down_) {}

  explicit Node() : next(nullptr), down(nullptr) {}

  ~Node() {
    /*
    std::cout << "Deleting node (" << row << ", " << col << ": " << data
              << ")\n";*/
    this->next = nullptr;
    this->down = nullptr;
    this->data = this->row = this->col = -1;
  }
  void killSelf(bool fromUp = 1) {
    if (fromUp) {
      if (down) {
        this->down->killSelf();
      }
    } else {
      if (next) {
        this->next->killSelf();
      }
    }

    delete this;
  }
  coordinates inline getCoordinates() const {
    return std::make_pair(this->row, this->col);
  }
  bool inline is(T data_) const { return this->data == data_; }
  T inline getData() const { return this->data; }
  T inline &getDataByReference() { return this->data; }
  void inline setData(T data_) { this->data = data_; }
  bool inline operator<=(const Node<T> &node) const {
    return this->data <= node.data;
  }

  bool inline isHeader() const {
    return (this->row == -1 ? true : (this->col == -1 ? true : false));
  }
  bool inline operator!=(const T &data_) { return this->data != data_; }

  bool inline operator<(const Node<T> &node) { return this->data < node.data; }

  bool inline operator<(const T &data_) { return this->data < data_; }
};

#endif
