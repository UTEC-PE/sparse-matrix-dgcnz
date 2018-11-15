#ifndef MATRIX_H
#define MATRIX_H
#include "node.h"
#include <cstdlib>
#include <time.h>
#include <utility>

template <typename T> struct PtrCoordinates {

  bool found;
  Node<T> **ptrLeft;
  Node<T> **ptrUp;

  PtrCoordinates(bool found_, Node<T> **ptrLeft_, Node<T> **ptrUp_)
      : found(found_), ptrLeft(ptrLeft_), ptrUp(ptrUp_) {}
};

template <typename T> class Matrix {
private:
  mutable Node<T> *hrows;
  mutable Node<T> *hcols;
  int rows;
  int cols;

public:
  typedef std::pair<int, int> coordinates;
  explicit Matrix();

  Matrix(int rows_, int cols_) { this->buildMatrix(rows_, cols_); }

  void buildMatrix(int &rows_, int &cols_) {
    this->rows = rows_;
    this->cols = cols_;
    Node<T> *temp = nullptr;
    while (rows_--) {
      temp = new Node<T>(0, rows_, -1, nullptr, temp);
    }
    this->hrows = temp;

    temp = nullptr;
    while (cols_--) {
      temp = new Node<T>(0, -1, cols_, temp, nullptr);
    }
    this->hcols = temp;
  }

  void fill(char type, int value = -1) {

    srand(time(NULL));
    if (type == 'r') {
      for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
          this->set(row, col, rand() % 10);
        }
      }
    } else if (type == 's') {
      for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
          this->set(row, col, value);
        }
      }
    }
  }

  PtrCoordinates<T> find(int row_, int col_) const {

    // std::cout << "\n************** PHASE 0 **************\n";
    bool foundL = false;
    bool foundR = false;
    Node<T> **fromLeft = &(this->hrows);
    Node<T> **fromUp = &(this->hcols);

    // std::cout << "\n************** PHASE 1 **************\n";
    for (int row = 0; row < row_; ++row) {
      fromLeft = &((*fromLeft)->down);
    }

    for (int col = 0; col < col_; ++col) {
      fromUp = &((*fromUp)->next);
    }

    // std::cout << "\n************** PHASE 2 **************\n";
    fromLeft = &((*fromLeft)->next);
    fromUp = &((*fromUp)->down);

    // std::cout << "\n************** PHASE 3 **************\n";
    while (*fromLeft) {
      if ((*fromLeft)->getCoordinates() == std::make_pair(row_, col_)) {
        foundL = true;
        break;
      } else {
        fromLeft = &((*fromLeft)->next);
      }
    }

    while (*fromUp) {
      if ((*fromUp)->getCoordinates() == std::make_pair(row_, col_)) {
        foundR = true;
        break;
      } else {
        fromUp = &((*fromUp)->down);
      }
    }

    // std::cout << "\n************** returning **************\n";
    return PtrCoordinates<T>(foundR && foundL, fromLeft, fromUp);
  }

  void set(int row, int col, T data) {
    if (row < this->rows && col < this->cols) {

      PtrCoordinates<T> searchResult = this->find(row, col);
      if (data == 0) {

        if (searchResult.found) {
          this->remove(searchResult.ptrLeft, searchResult.ptrUp);
        }
      } else {
        if (searchResult.found) {

          this->replace(data, searchResult.ptrLeft, searchResult.ptrUp);
        } else {
          this->insert(row, col, data, searchResult.ptrLeft,
                       searchResult.ptrUp);
        }
      }
    } else {
      std::cout
          << "(" << row << ", " << col
          << ") : trying to set an element outside of the max x and max y "
             "boundaries\n";
    }
  }

private:
  void insert(int row, int col, T data, Node<T> **ptrLeft, Node<T> **ptrUp) {

    Node<T> *node = new Node<T>(data, row, col, *ptrLeft, *ptrUp);
    *ptrUp = node;
    *ptrLeft = node;
  }
  void replace(T data, Node<T> **ptrLeft, Node<T> **ptrUp) {
    (*ptrLeft)->setData(data);
  }
  void remove(Node<T> **ptrLeft, Node<T> **ptrUp) {

    Node<T> *temp = *ptrLeft;
    *ptrLeft = (*ptrLeft)->next;
    *ptrUp = (*ptrUp)->down;

    delete temp;
    temp = nullptr;
  }

public:
  T operator()(int row, int col) const {

    PtrCoordinates<T> searchResult = this->find(row, col);
    if (searchResult.found) {
      return (*searchResult.ptrLeft)->getData();
    } else {
      return 0;
    }
  }
  T &operator()(int row, int col) {
    PtrCoordinates<T> searchResult = this->find(row, col);
    if (searchResult.found) {
      return (*searchResult.ptrLeft)->getDataByReference();
    } else {
      this->insert(row, col, -1, searchResult.ptrLeft, searchResult.ptrUp);
      return (*searchResult.ptrLeft)->getDataByReference();
    }
  }

  Matrix<T> operator*(const Matrix<T> &other) const {
    Matrix<T> result(this->rows, other.cols);
    /*
    std::cout << "(" << this->rows << " x " << this->cols << ") * ("
              << other.rows << " x " << other.cols << ")\n";
              */
    if (this->cols == other.rows) {
      int innerSum = 0;

      for (int row = 0; row < this->rows; ++row) {
        for (int element = 0; element < other.cols; ++element) {
          for (int col = 0; col < this->cols; ++col) {
            /*
            std::cout << "(" << row << ", " << col << ")"
                      << "(" << (*this)(row, col) << ")"
                      << " * "
                      << "(" << col << ", " << element << ")"
                      << "(" << other(col, element) << ")";
                      */
            innerSum += (*this)(row, col) * other(col, element);
            /*
            std::cout << "  =  " << (*this)(row, col) * other(col, element);
            std::cout << "      :     innerSum = " << innerSum << '\n';
            */
          }
          result.set(row, element, innerSum);
          innerSum = 0;
        }
      }
    } else {
      std::cout << "number of columns of first matrix don't match number of "
                   "rows of second matrix. Returning empty matrix.\n";
    }
    return result;
  }
  Matrix<T> operator*(const T &scalar) const {
    Matrix<T> result(this->rows, this->cols);
    for (int row = 0; row < this->rows; ++row) {
      for (int col = 0; col < this->cols; ++col) {
        result.set(row, col, (*this)(row, col) * scalar);
      }
    }
    return result;
  }
  Matrix<T> operator+(const Matrix<T> &other) const {

    Matrix<T> result(this->rows, this->cols);
    if (this->rows == other.rows && this->cols == other.cols) {
      for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
          result.set(row, col, (*this)(row, col) + other(row, col));
        }
      }
    } else {
      std::cout
          << "number of rows and columns don't match, returning empty matrix\n";
    }

    return result;
  }
  Matrix<T> operator-(const Matrix<T> &other) const {

    Matrix<T> result(this->rows, this->cols);
    if (this->rows == other.rows && this->cols == other.cols) {
      for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
          result.set(row, col, (*this)(row, col) - other(row, col));
        }
      }
    } else {
      std::cout
          << "number of rows and columns don't match, returning empty matrix\n";
    }
    return result;
  }
  Matrix<T> transpose() const {
    Matrix<T> result(this->cols, this->rows);
    for (int row = 0; row < this->rows; ++row) {
      for (int col = 0; col < this->cols; ++col) {
        result.set(col, row, (*this)(row, col));
      }
    }
    return result;
  }
  Matrix<T> &operator=(const Matrix<T> &other) {
    this->hardReset(other.rows, other.cols);
    for (int row = 0; row < this->rows; ++row) {
      for (int col = 0; col < this->cols; ++col) {
        this->set(row, col, other(row, col));
      }
    }
    return *this;
  }

  void hardReset(int rows_, int cols_) {
    // Current matrix gets totally cleared
    this->killSelf();

    // Header nodes are built with new rows and column sizes
    this->buildMatrix(rows_, cols_);
  }

  void clear() {
    Node<T> *temp = this->hcols;
    while (temp) {
      if (temp->down) {
        temp->down->killSelf();
      }
      temp->down = nullptr;
      temp = temp->next;
    }

    temp = this->hrows;
    while (temp) {
      temp->next = nullptr;
      temp = temp->down;
    }
  }

  void killSelf() {
    // Clears all dataNodes
    this->clear();
    // Triggers horizontal killSelf()
    Node<T> *temp = this->hcols;
    temp->killSelf(0);
    this->hcols = nullptr;

    // Triggers vertical killSelf()
    temp = this->hrows;
    temp->killSelf(1);
    this->hrows = nullptr;
  }
  void print() {
    std::cout << "\n__|_";

    for (int col_h = 0; col_h < this->cols; ++col_h) {
      std::cout << col_h << '_';
    }
    std::cout << "_\n";
    for (int row = 0; row < this->rows; ++row) {

      std::cout << row << " | ";
      for (int col = 0; col < this->cols; ++col) {
        std::cout << (*this)(row, col) << ' ';
      }
      std::cout << "|\n";
    }
    std::cout << '\n';
  }

  ~Matrix() { this->killSelf(); };
};

#endif
