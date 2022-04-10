// NA 2019/2020: Zadaća 1, Zadatak 1
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

class Vector {
  std::vector<double> v;

public:
  explicit Vector(int n);
  Vector(std::initializer_list<double> l);
  int NElems() const;
  double &operator[](int i);
  double operator[](int i) const;
  double &operator()(int i);
  double operator()(int i) const;
  double Norm() const;
  friend double VectorNorm(const Vector &v);
  double GetEpsilon() const;
  void Print(char separator = '\n', double eps = -1) const;
  friend void PrintVector(const Vector &v, char separator = '\n',
                          double eps = -1) {
    double eps1 = eps;
    if (eps < 0)
      eps1 = v.GetEpsilon();
    for (int i = 0; i < v.v.size(); i++) {
      if (fabs(v.v.at(i)) < eps1)
        std::cout << 0;
      else
        std::cout << v.v.at(i);
      if (i != v.v.size() - 1 || separator == '\n')
        std::cout << separator;
    }
  }
  friend Vector operator+(const Vector &v1, const Vector &v2);
  Vector &operator+=(const Vector &v);
  friend Vector operator-(const Vector &v1, const Vector &v2);
  Vector &operator-=(const Vector &v);
  friend Vector operator*(double s, const Vector &v);
  friend Vector operator*(const Vector &v, double s);
  Vector &operator*=(double s);
  friend double operator*(const Vector &v1, const Vector &v2);
  friend Vector operator/(const Vector &v, double s);
  Vector &operator/=(double s);
};

Vector::Vector(int n) {
  if (n <= 0)
    throw std::range_error("Bad dimension");
  v.resize(n);
}

Vector::Vector(std::initializer_list<double> l) {
  if (!l.size())
    throw std::range_error("Bad dimension");
  for (auto x : l)
    v.push_back(x);
}

int Vector::NElems() const { return v.size(); }

double &Vector::operator[](int i) { return v.at(i); }

double Vector::operator[](int i) const { return v.at(i); }

double &Vector::operator()(int i) {
  if (i <= 0 || i > v.size())
    throw std::range_error("Invalid index");
  return v.at(i - 1);
}

double Vector::operator()(int i) const {
  if (i <= 0 || i > v.size())
    throw std::range_error("Invalid index");
  return v.at(i - 1);
}

double Vector::Norm() const {
  double sum{};
  for (auto x : v)
    sum += x * x;
  return sqrt(sum);
}

double VectorNorm(const Vector &v) {
  double sum{};
  for (auto x : v.v)
    sum += x * x;
  return sqrt(sum);
}

double Vector::GetEpsilon() const {
  return 10 * Norm() * std::numeric_limits<double>::epsilon();
}

void Vector::Print(char separator, double eps) const {
  double eps1 = eps;
  if (eps < 0)
    eps1 = GetEpsilon();
  for (int i = 0; i < v.size(); i++) {
    if (fabs(v.at(i)) < eps1)
      std::cout << 0;
    else
      std::cout << v.at(i);
    if (i != v.size() - 1 || separator == '\n')
      std::cout << separator;
  }
}

Vector operator+(const Vector &v1, const Vector &v2) {
  if (v1.v.size() != v2.v.size())
    throw std::domain_error("Incompatible formats");
  Vector v3(v1.v.size());
  for (int i = 0; i < v1.v.size(); i++)
    v3.v.at(i) = (v1.v.at(i) + v2.v.at(i));
  return v3;
}
Vector &Vector::operator+=(const Vector &v) {
  if (this->v.size() != v.v.size())
    throw std::domain_error("Incompatible formats");
  for (int i = 0; i < this->v.size(); i++)
    this->v.at(i) = (this->v.at(i) + v.v.at(i));
  return *this;
}

Vector operator-(const Vector &v1, const Vector &v2) {
  if (v1.v.size() != v2.v.size())
    throw std::domain_error("Incompatible formats");
  Vector v3(v1.v.size());
  for (int i = 0; i < v1.v.size(); i++)
    v3.v.at(i) = (v1.v.at(i) - v2.v.at(i));
  return v3;
}

Vector &Vector::operator-=(const Vector &v) {
  if (this->v.size() != v.v.size())
    throw std::domain_error("Incompatible formats");
  for (int i = 0; i < this->v.size(); i++)
    this->v.at(i) = (this->v.at(i) - v.v.at(i));
  return *this;
}

Vector operator*(double s, const Vector &v) {
  Vector v1(v.v.size());
  for (int i = 0; i < v.v.size(); i++)
    v1.v.at(i) = (v.v.at(i) * s);
  return v1;
}

Vector operator*(const Vector &v, double s) {
  Vector v1(v.v.size());
  for (int i = 0; i < v.v.size(); i++)
    v1.v.at(i) = (v.v.at(i) * s);
  return v1;
}

Vector &Vector::operator*=(double s) {
  for (int i = 0; i < v.size(); i++)
    v.at(i) = (v.at(i) * s);
  return *this;
}

double operator*(const Vector &v1, const Vector &v2) {
  if (v1.v.size() != v2.v.size())
    throw std::domain_error("Incompatible formats");
  double sum{};
  for (int i = 0; i < v1.v.size(); i++)
    sum += v1.v.at(i) * v2.v.at(i);
  return sum;
}

Vector operator/(const Vector &v, double s) {
  if (fabs(s) <= v.GetEpsilon() * fabs(s))
    throw std::domain_error("Division by zero");
  Vector v1(v.v.size());
  for (int i = 0; i < v.v.size(); i++)
    v1.v.at(i) = v.v.at(i) / s;
  return v1;
}

Vector &Vector::operator/=(double s) {
  if (fabs(s) <= GetEpsilon() * fabs(s))
    throw std::domain_error("Division by zero");
  for (int i = 0; i < v.size(); i++)
    v.at(i) /= s;
  return *this;
}

class Matrix {
  std::vector<std::vector<double>> m;

public:
  Matrix(int m, int n);
  Matrix(const Vector &v);
  Matrix(std::initializer_list<std::vector<double>> l);
  int NRows() const;
  int NCols() const;
  double *operator[](int i);
  const double *operator[](int i) const;
  double &operator()(int i, int j);
  double operator()(int i, int j) const;
  double Norm() const;
  friend double MatrixNorm(const Matrix &m);
  double GetEpsilon() const;
  void Print(int width = 10, double eps = -1) const;
  friend void PrintMatrix(const Matrix &m, int width = 10, double eps = -1) {
    double eps1 = eps;
    if (eps < 0)
      eps1 = m.GetEpsilon();
    for (auto x : m.m) {
      for (auto y : x) {
        if (fabs(y) < eps1)
          std::cout << std::setw(width) << 0;
        else
          std::cout << std::setw(width) << y;
      }
      std::cout << std::endl;
    }
  }
  friend Matrix operator+(const Matrix &m1, const Matrix &m2);
  Matrix &operator+=(const Matrix &m);
  friend Matrix operator-(const Matrix &m1, const Matrix &m2);
  Matrix &operator-=(const Matrix &m);
  friend Matrix operator*(double s, const Matrix &m);
  friend Matrix operator*(const Matrix &m, double s);
  Matrix &operator*=(double s);
  friend Matrix operator*(const Matrix &m1, const Matrix &m2);
  Matrix &operator*=(const Matrix &m);
  friend Vector operator*(const Matrix &m, const Vector &v);
  friend Matrix Transpose(const Matrix &m);
  void Transpose();
};

Matrix::Matrix(int m, int n) {
  if (m <= 0 || n <= 0)
    throw std::range_error("Bad dimension");
  this->m.resize(m);
  for (int i = 0; i < m; i++)
    this->m.at(i).resize(n);
}

Matrix::Matrix(const Vector &v) {
  m.resize(1);
  for (int i = 0; i < v.NElems(); i++)
    m.at(0).push_back(v[i]);
}

Matrix::Matrix(std::initializer_list<std::vector<double>> l) {
  if (!l.size())
    throw std::range_error("Bad dimension");
  int s = l.begin()->size();
  for (auto x : l) {
    if (!x.size())
      throw std::range_error("Bad dimension");
    if (x.size() != s)
      throw std::logic_error("Bad matrix");
  }
  m.resize(l.size());
  int i{};
  for (auto x : l) {
    m.at(i++) = x;
  }
}

int Matrix::NRows() const { return m.size(); }

int Matrix::NCols() const { return m.at(0).size(); }

double *Matrix::operator[](int i) { return &m.at(i).at(0); }

const double *Matrix::operator[](int i) const { return &m.at(i).at(0); }

double &Matrix::operator()(int i, int j) {
  if (i > m.size() || i <= 0 || j > m.at(0).size() || j <= 0)
    throw std::range_error("Invalid index");
  return m.at(i - 1).at(j - 1);
}

double Matrix::operator()(int i, int j) const {
  if (i > m.size() || i <= 0 || j > m.at(0).size() || j <= 0)
    throw std::range_error("Invalid index");
  return m.at(i - 1).at(j - 1);
}

double Matrix::Norm() const {
  double sum{};
  for (auto x : m)
    for (auto y : x)
      sum += y * y;
  return sqrt(sum);
}

double MatrixNorm(const Matrix &m) {
  double sum{};
  for (auto x : m.m)
    for (auto y : x)
      sum += y * y;
  return sqrt(sum);
}

double Matrix::GetEpsilon() const {
  return 10 * Norm() * std::numeric_limits<double>::epsilon();
}

void Matrix::Print(int width, double eps) const {
  double eps1 = eps;
  if (eps < 0)
    eps1 = GetEpsilon();
  for (auto x : m) {
    for (auto y : x) {
      if (fabs(y) < eps1)
        std::cout << std::setw(width) << 0;
      else
        std::cout << std::setw(width) << y;
    }
    std::cout << std::endl;
  }
}

Matrix operator+(const Matrix &m1, const Matrix &m2) {
  if (m1.m.size() != m2.m.size() || m1.m.at(0).size() != m2.m.at(0).size())
    throw std::domain_error("Incompatible formats");
  Matrix m3(m1.m.size(), m1.m.at(0).size());
  for (int i = 0; i < m1.m.size(); i++)
    for (int j = 0; j < m1.m.at(0).size(); j++)
      m3.m.at(i).at(j) = m1.m.at(i).at(j) + m2.m.at(i).at(j);
  return m3;
}

Matrix &Matrix::operator+=(const Matrix &m) {
  if (this->m.size() != m.m.size() || this->m.at(0).size() != m.m.at(0).size())
    throw std::domain_error("Incompatible formats");
  for (int i = 0; i < m.m.size(); i++)
    for (int j = 0; j < m.m.at(0).size(); j++)
      this->m.at(i).at(j) = this->m.at(i).at(j) + m.m.at(i).at(j);
  return *this;
}

Matrix operator-(const Matrix &m1, const Matrix &m2) {
  if (m1.m.size() != m2.m.size() || m1.m.at(0).size() != m2.m.at(0).size())
    throw std::domain_error("Incompatible formats");
  Matrix m3(m1.m.size(), m1.m.at(0).size());
  for (int i = 0; i < m1.m.size(); i++)
    for (int j = 0; j < m1.m.at(0).size(); j++)
      m3.m.at(i).at(j) = m1.m.at(i).at(j) - m2.m.at(i).at(j);
  return m3;
}

Matrix &Matrix::operator-=(const Matrix &m) {
  if (this->m.size() != m.m.size() || this->m.at(0).size() != m.m.at(0).size())
    throw std::domain_error("Incompatible formats");
  for (int i = 0; i < m.m.size(); i++)
    for (int j = 0; j < m.m.at(0).size(); j++)
      this->m.at(i).at(j) = this->m.at(i).at(j) - m.m.at(i).at(j);
  return *this;
}

Matrix operator*(double s, const Matrix &m) {
  Matrix m1(m.m.size(), m.m.at(0).size());
  for (int i = 0; i < m.m.size(); i++)
    for (int j = 0; j < m.m.at(0).size(); j++)
      m1.m.at(i).at(j) = s * m.m.at(i).at(j);
  return m1;
}

Matrix operator*(const Matrix &m, double s) {
  Matrix m1(m.m.size(), m.m.at(0).size());
  for (int i = 0; i < m.m.size(); i++)
    for (int j = 0; j < m.m.at(0).size(); j++)
      m1.m.at(i).at(j) = s * m.m.at(i).at(j);
  return m1;
}

Matrix &Matrix::operator*=(double s) {
  for (int i = 0; i < m.size(); i++)
    for (int j = 0; j < m.at(0).size(); j++)
      m.at(i).at(j) *= s;
  return *this;
}

Matrix operator*(const Matrix &m1, const Matrix &m2) {
  if (m1.m.at(0).size() != m2.m.size())
    throw std::domain_error("Incompatible formats");
  Matrix m3(m1.m.size(), m2.m.at(0).size());
  for (int k = 0; k < m2.m.at(0).size(); k++)
    for (int i = 0; i < m1.m.size(); i++) {
      double x{};
      for (int j = 0; j < m1.m.at(0).size(); j++)
        x += m1.m.at(i).at(j) * m2.m.at(j).at(k);
      m3.m.at(i).at(k) = x;
    }
  return m3;
}

Matrix &Matrix::operator*=(const Matrix &m) {
  if (this->m.at(0).size() != m.m.size())
    throw std::domain_error("Incompatible formats");
  Matrix m3(this->m.size(), m.m.at(0).size());
  for (int k = 0; k < m.m.at(0).size(); k++)
    for (int i = 0; i < this->m.size(); i++) {
      double x{};
      for (int j = 0; j < this->m.at(0).size(); j++)
        x += this->m.at(i).at(j) * m.m.at(j).at(k);
      m3.m.at(i).at(k) = x;
    }
  this->m = m3.m;
  return *this;
}

Vector operator*(const Matrix &m, const Vector &v) {
  if (m.m.at(0).size() != v.NElems())
    throw std::domain_error("Incompatible formats");
  Vector v1(m.m.size());
  for (int i = 0; i < m.m.size(); i++) {
    double x{};
    for (int j = 0; j < m.m.at(0).size(); j++)
      x += m.m.at(i).at(j) * v[j];
    v1[i] = x;
  }
  return v1;
}

Matrix Transpose(const Matrix &m) {
  Matrix m1(m.m.at(0).size(), m.m.size());
  if (m.m.size() == m.m.at(0).size()) {
    for (int i = 0; i < m.m.size(); i++) {
      for (int j = 0; j < i; j++) {
        m1.m.at(i).at(j) = m.m.at(j).at(i);
        m1.m.at(j).at(i) = m.m.at(i).at(j);
      }
      m1.m.at(i).at(i) = m.m.at(i).at(i);
    }
  } else {
    for (int i = 0; i < m.m.at(0).size(); i++) {
      for (int j = 0; j < m.m.size(); j++) {
        m1.m.at(i).at(j) = m.m.at(j).at(i);
      }
    }
  }
  return m1;
}

void Matrix::Transpose() {
  if (m.size() == m.at(0).size()) {
    for (int i = 0; i < m.size(); i++)
      for (int j = 0; j < i; j++) {
        auto x = m.at(i).at(j);
        m.at(i).at(j) = m.at(j).at(i);
        m.at(j).at(i) = x;
      }
  } else {
    Matrix m1(m.at(0).size(), m.size());
    for (int i = 0; i < m.at(0).size(); i++) {
      for (int j = 0; j < m.size(); j++) {
        m1.m.at(i).at(j) = m.at(j).at(i);
      }
    }
    m = m1.m;
  }
}

void test_Vector_ctor1() {
  Vector v(5);
  std::cout << "Constructor1 test: ";
  if (v.NElems() == 5)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_ctor1_exception() {
  std::cout << "Constructor1 exception test: ";
  try {
    Vector v(-5);
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_ctor2() {
  std::initializer_list<double> l{1.5, 2.3, 3.7, 4.9, 5.2};
  Vector v(l);
  std::cout << "Constructor2 test: ";
  if (v.NElems() == 5 &&
      fabs(v[2] - 3.7) <= v.GetEpsilon() * (fabs(v[2]) + 3.7))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_ctor2_exception() {
  std::cout << "Constructor2 exception test: ";
  try {
    std::initializer_list<double> l;
    Vector v(l);
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_NElems() {
  Vector v(172);
  std::cout << "Class method NElems test: ";
  if (v.NElems() == 172)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_square_brackets() {
  Vector v(4);
  v[2] = 53.4;
  const Vector v1(v);
  std::cout << "Overloaded operator square brackets test: ";
  if (fabs(v1[2] - 53.4) <= v1.GetEpsilon() * (fabs(v1[2]) + 53.4))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_round_brackets() {
  Vector v(4);
  v(4) = 12.234;
  const Vector v1(v);
  std::cout << "Overloaded operator round brackets test: ";
  if (fabs(v1(4) - 12.234) <= v1.GetEpsilon() * (fabs(v1(4)) + 12.234))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_round_brackets_exception() {
  std::cout << "Overloaded operator round brackets exception test: ";
  try {
    Vector v(4);
    v(0) = 3.3;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_Norm() {
  Vector v{5.4, 5.5, 5.6, 5.7, 5.8};
  std::cout << "Class method Norm test: ";
  double x = v.Norm(), y = sqrt(156.9);
  if (fabs(x - y) <= v.GetEpsilon() * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_VectorNorm() {
  Vector v{5.4, 5.5, 5.6, 5.7, 5.8};
  std::cout << "Friend function VectorNorm test: ";
  double x = VectorNorm(v), y = sqrt(156.9);
  if (fabs(x - y) <= v.GetEpsilon() * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_GetEpsilon() {
  Vector v{2.5, 3.5, 4.5};
  std::cout << "Class method GetEpsilon test: ";
  double x = v.GetEpsilon(),
         y = 10 * v.Norm() * std::numeric_limits<double>::epsilon();
  if (fabs(x - y) <= v.GetEpsilon() * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_Print() {
  Vector v{1e-15, 1e-20, 1e-30, 5};
  std::cout << "Class method Print test:" << std::endl;
  std::cout << "    Expected output: 0,0,0,5" << std::endl;
  std::cout << "    Actual output: ";
  v.Print(',');
  std::cout << std::endl;
}

void test_Vector_PrintVector() {
  Vector v{1e-15, 5, 4e-30, 5};
  std::cout << "Friend function PrintVector test:" << std::endl;
  std::cout << "    Expected output: 0/5/0/5" << std::endl;
  std::cout << "    Actual output: ";
  v.Print('/');
  std::cout << std::endl;
}

void test_Vector_operator_plus() {
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13, 23.55, 19.56, 7.88};
  Vector v3 = v1 + v2;
  std::cout << "Overloaded operator + test:" << std::endl;
  std::cout << "    Expected output: " << 5.1 + 4.13 << "," << 6.2 + 23.55
            << "," << 10.34 + 19.56 << "," << 23.22 + 7.88 << std::endl;
  std::cout << "    Actual output: ";
  v3.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_plus_exception() {
  std::cout << "Overloaded operator + exception test: ";
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13, 23.55};
  try {
    Vector v3 = v1 + v2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_operator_plus_equal() {
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13, 23.55, 19.56, 7.88};
  v1 += v2;
  std::cout << "Overloaded operator += test:" << std::endl;
  std::cout << "    Expected output: " << 5.1 + 4.13 << "," << 6.2 + 23.55
            << "," << 10.34 + 19.56 << "," << 23.22 + 7.88 << std::endl;
  std::cout << "    Actual output: ";
  v1.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_plus_equal_exception() {
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13};
  std::cout << "Overloaded operator += exception test: ";
  try {
    v1 += v2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_operator_minus() {
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13, 23.55, 19.56, 7.88};
  Vector v3 = v1 - v2;
  std::cout << "Overloaded operator - test:" << std::endl;
  std::cout << "    Expected output: " << 5.1 - 4.13 << "," << 6.2 - 23.55
            << "," << 10.34 - 19.56 << "," << 23.22 - 7.88 << std::endl;
  std::cout << "    Actual output: ";
  v3.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_minus_exception() {
  std::cout << "Overloaded operator - exception test: ";
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13, 23.55};
  try {
    Vector v3 = v1 - v2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_operator_minus_equal() {
  Vector v1{5.1, 6.2, 10.34, 23.22}, v2{4.13, 23.55, 19.56, 7.88};
  v1 -= v2;
  std::cout << "Overloaded operator -= test:" << std::endl;
  std::cout << "    Expected output: " << 5.1 - 4.13 << "," << 6.2 - 23.55
            << "," << 10.34 - 19.56 << "," << 23.22 - 7.88 << std::endl;
  std::cout << "    Actual output: ";
  v1.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_minus_equal_exception() {
  Vector v1{5.1, 6.2}, v2{4.13, 23.55, 19.56, 7.88};
  std::cout << "Overloaded operator -= exception test: ";
  try {
    v1 -= v2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_operator_multiply_by_a_scalar() {
  Vector v1{1.5, 3.5, 7.5, 12.5};
  Vector v2 = 2 * v1;
  std::cout << "Overloaded operator * for multiplying Vector by a scalar test:"
            << std::endl;
  std::cout << "    Expected output: " << 3 << "," << 7 << "," << 15 << ","
            << 25 << std::endl;
  std::cout << "    Actual output: ";
  v2.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_multiply_by_a_scalar_equal() {
  Vector v1{1.5, 3.5, 7.5, 12.5};
  v1 *= 4;
  std::cout << "Overloaded operator *= for multiplying Vector by a scalar test:"
            << std::endl;
  std::cout << "    Expected output: " << 6 << "," << 14 << "," << 30 << ","
            << 50 << std::endl;
  std::cout << "    Actual output: ";
  v1.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_multiply_Vectors() {
  Vector v1{1.5, 3.5, 7.5, 12.5}, v2{2, 4, 6, 8};
  double x = v1 * v2, y = 162;
  std::cout << "Overloaded operator * for multiplying Vectors test: ";
  if (fabs(x - y) <= v1.GetEpsilon() * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Vector_operator_multiply_Vectors_exception() {
  Vector v1{5.1, 6.2}, v2{4.13, 23.55, 19.56, 7.88};
  std::cout << "Overloaded operator * for multiplying Vectors exception test: ";
  try {
    double x = v1 * v2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_operator_divide() {
  Vector v1{12, 15.5, 23.65, 15.77, 8.99};
  Vector v2 = v1 / 0.01;
  std::cout << "Overloaded operator / test:" << std::endl;
  std::cout << "    Expected output: " << 12 * 100 << "," << 15.5 * 100 << ","
            << 23.65 * 100 << "," << 15.77 * 100 << "," << 8.99 * 100
            << std::endl;
  std::cout << "    Actual output: ";
  v2.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_divide_exception() {
  Vector v1{12, 15.5, 23.65, 15.77, 8.99};
  std::cout << "Overloaded operator / exception test: ";
  try {
    Vector v2 = v1 / 0;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Vector_operator_divide_equal() {
  Vector v1{12, 15.5, 23.65, 15.77, 8.99};
  v1 /= 3.2;
  std::cout << "Overloaded operator /= test:" << std::endl;
  std::cout << "    Expected output: " << 12 / 3.2 << "," << 15.5 / 3.2 << ","
            << 23.65 / 3.2 << "," << 15.77 / 3.2 << "," << 8.99 / 3.2
            << std::endl;
  std::cout << "    Actual output: ";
  v1.Print(',');
  std::cout << std::endl;
}

void test_Vector_operator_divide_equal_exception() {
  Vector v1{12, 15.5, 23.65, 15.77, 8.99};
  std::cout << "Overloaded operator /= exception test: ";
  try {
    v1 /= 0;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_ctor1() {
  Matrix m(5, 4);
  std::cout << "Constructor1 test: ";
  if (m.NRows() == 5 && m.NCols() == 4)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_ctor1_exception() {
  std::cout << "Constructor1 exception test: ";
  try {
    Matrix m(-5, 2);
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_ctor2() {
  Vector v{1, 2, 3, 4, 5};
  Matrix m(v);
  std::cout << "Constructor2 test: ";
  if (m.NRows() == 1 && m.NCols() == v.NElems())
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_ctor3() {
  std::initializer_list<std::vector<double>> l{
      {1.5, 2.5, 3.5}, {4.5, 5.5, 6.5}, {7.5, 8.5, 9.5}};
  Matrix m(l);
  std::cout << "Constructor3 test:" << std::endl;
  std::cout << "    Expected output:" << std::endl;
  for (auto x : l) {
    for (auto y : x)
      std::cout << std::setw(10) << y;
    std::cout << std::endl;
  }
  std::cout << "    Actual output:" << std::endl;
  m.Print();
}

void test_Matrix_ctor3_exception_1() {
  std::cout << "Constructor3 exception test1: ";
  try {
    std::initializer_list<std::vector<double>> l;
    Matrix m(l);
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_ctor3_exception_2() {
  std::cout << "Constructor3 exception test2: ";
  try {
    std::initializer_list<std::vector<double>> l{{}, {}, {}};
    Matrix m(l);
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_ctor3_exception_3() {
  std::cout << "Constructor3 exception test3: ";
  try {
    std::initializer_list<std::vector<double>> l{
        {1, 2}, {1.1, 2.2, 3.3, 4.4}, {55}};
    Matrix m(l);
    std::cout << "NOT OK!" << std::endl;
  } catch (std::logic_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_NRows() {
  Matrix m(17, 2);
  std::cout << "Class method NRows test: ";
  if (m.NRows() == 17)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_NCols() {
  Matrix m(1, 22);
  std::cout << "Class method NCols test: ";
  if (m.NCols() == 22)
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_square_brackets() {
  Matrix m(4, 5);
  m[2][3] = 66.66;
  const Matrix m1(m);
  std::cout << "Overloaded operator square brackets test: ";
  if (fabs(m1[2][3] - 66.66) <= m1.GetEpsilon() * (fabs(m1[2][3]) + 66.66))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_round_brackets() {
  Matrix m(4, 3);
  m(4, 3) = 12.234;
  const Matrix m1(m);
  std::cout << "Overloaded operator round brackets test: " << std::endl;
  std::cout << "    Expected output:" << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++)
      if (i == 3 && j == 2)
        std::cout << std::setw(10) << 12.234;
      else
        std::cout << std::setw(10) << 0;
    std::cout << std::endl;
  }
  std::cout << "    Actual output:" << std::endl;
  m.Print();
}

void test_Matrix_round_brackets_exception() {
  std::cout << "Overloaded operator round brackets exception test: ";
  try {
    Matrix m(4, 4);
    m(-5, 2) = 3.3;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::range_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_Norm() {
  Matrix m{{5.4, 5.5}, {5.6, 5.7}, {5.8, 5.9}};
  std::cout << "Class method Norm test: ";
  double x = m.Norm(), y = sqrt(5.4 * 5.4 + 5.5 * 5.5 + 5.6 * 5.6 + 5.7 * 5.7 +
                                5.8 * 5.8 + 5.9 * 5.9);
  if (fabs(x - y) <= m.GetEpsilon() * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_MatrixNorm() {
  Matrix m{{5.4, 5.5}, {5.6, 5.7}, {5.8, 5.9}};
  std::cout << "Friend function MatrixNorm test: ";
  double x = MatrixNorm(m), y = sqrt(5.4 * 5.4 + 5.5 * 5.5 + 5.6 * 5.6 +
                                     5.7 * 5.7 + 5.8 * 5.8 + 5.9 * 5.9);
  if (fabs(x - y) <= m.GetEpsilon() * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_GetEpsilon() {
  Matrix m{{2.5, 3.5}, {4.5, 5.5}};
  std::cout << "Class method GetEpsilon test: ";
  double x = m.GetEpsilon(),
         y = 10 * m.Norm() * std::numeric_limits<double>::epsilon();
  if (fabs(x - y) <= x * (x + y))
    std::cout << "OK!" << std::endl;
  else
    std::cout << "NOT OK!" << std::endl;
}

void test_Matrix_Print() {
  Matrix m{{1e-15, 1e-20}, {1e-30, 10.5}};
  std::cout << "Class method Print test:" << std::endl;
  std::cout << "    Expected output:\n     0     0\n     0  10.5" << std::endl;
  std::cout << "    Actual output:\n";
  m.Print(6);
}

void test_Matrix_PrintMatrix() {
  Matrix m{{1e-15, 5}, {4e-30, 10.5}};
  std::cout << "Friend function PrintMatrix test:" << std::endl;
  std::cout << "    Expected output:\n     0     5\n     0  10.5" << std::endl;
  std::cout << "    Actual output:\n";
  m.Print(6);
}

void test_Matrix_operator_plus() {
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13, 23.55}, {19.56, 7.88}};
  Matrix m3 = m1 + m2;
  std::cout << "Overloaded operator + test:" << std::endl;
  std::cout << "    Expected output:\n"
            << std::setw(10) << 5.1 + 4.13 << std::setw(10) << 6.2 + 23.55
            << "\n"
            << std::setw(10) << 10.34 + 19.56 << std::setw(10) << 23.22 + 7.88
            << std::endl;
  std::cout << "    Actual output:\n";
  m3.Print();
}

void test_Matrix_operator_plus_exception() {
  std::cout << "Overloaded operator + exception test: ";
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13, 23.55}};
  try {
    Matrix m3 = m1 + m2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_operator_plus_equal() {
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13, 23.55}, {19.56, 7.88}};
  m1 += m2;
  std::cout << "Overloaded operator += test:" << std::endl;
  std::cout << "    Expected output:\n"
            << std::setw(10) << 5.1 + 4.13 << std::setw(10) << 6.2 + 23.55
            << "\n"
            << std::setw(10) << 10.34 + 19.56 << std::setw(10) << 23.22 + 7.88
            << std::endl;
  std::cout << "    Actual output:\n";
  m1.Print();
}

void test_Matrix_operator_plus_equal_exception() {
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13}, {19.56}};
  std::cout << "Overloaded operator += exception test: ";
  try {
    m1 += m2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}
void test_Matrix_operator_minus() {
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13, 23.55}, {19.56, 7.88}};
  Matrix m3 = m1 - m2;
  std::cout << "Overloaded operator - test:" << std::endl;
  std::cout << "    Expected output:\n"
            << std::setw(10) << 5.1 - 4.13 << std::setw(10) << 6.2 - 23.55
            << "\n"
            << std::setw(10) << 10.34 - 19.56 << std::setw(10) << 23.22 - 7.88
            << std::endl;
  std::cout << "    Actual output:\n";
  m3.Print();
}

void test_Matrix_operator_minus_exception() {
  std::cout << "Overloaded operator - exception test: ";
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13, 23.55}};
  try {
    Matrix m3 = m1 - m2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_operator_minus_equal() {
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13, 23.55}, {19.56, 7.88}};
  m1 -= m2;
  std::cout << "Overloaded operator -= test:" << std::endl;
  std::cout << "    Expected output:\n"
            << std::setw(10) << 5.1 - 4.13 << std::setw(10) << 6.2 - 23.55
            << "\n"
            << std::setw(10) << 10.34 - 19.56 << std::setw(10) << 23.22 - 7.88
            << std::endl;
  std::cout << "    Actual output:\n";
  m1.Print();
}

void test_Matrix_operator_minus_equal_exception() {
  Matrix m1{{5.1, 6.2}, {10.34, 23.22}}, m2{{4.13}, {19.56}};
  std::cout << "Overloaded operator -= exception test: ";
  try {
    m1 -= m2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_operator_multiply_by_a_scalar() {
  Matrix m1{{1.5, 3.5}, {7.5, 12.5}};
  Matrix m2 = 2 * m1;
  std::cout << "Overloaded operator * for multiplying Matrix by a scalar test:"
            << std::endl;
  std::cout << "    Expected output:\n";
  for (auto x : {1.5, 3.5})
    std::cout << std::setw(10) << 2 * x;
  std::cout << std::endl;
  for (auto x : {7.5, 12.5})
    std::cout << std::setw(10) << 2 * x;
  std::cout << std::endl;
  std::cout << "    Actual output:\n";
  m2.Print();
}

void test_Matrix_operator_multiply_by_a_scalar_equal() {
  Matrix m1{{1.5, 3.5}, {7.5, 12.5}};
  m1 *= 4;
  std::cout << "Overloaded operator *= for multiplying Matrix by a scalar test:"
            << std::endl;
  std::cout << "    Expected output:\n";
  for (auto x : {1.5, 3.5})
    std::cout << std::setw(10) << 4 * x;
  std::cout << std::endl;
  for (auto x : {7.5, 12.5})
    std::cout << std::setw(10) << 4 * x;
  std::cout << std::endl;
  std::cout << "    Actual output:\n";
  m1.Print();
}

void test_Matrix_operator_multiply_Matrices() {
  Matrix m1{{1.5, 3.5}, {7.5, 12.5}}, m2{{2, 4}, {6, 8}};
  Matrix m3 = m1 * m2;
  std::cout << "Overloaded operator * for multiplying Matrices test:"
            << std::endl;
  std::cout << "    Expected output:\n"
            << std::setw(10) << 1.5 * 2 + 3.5 * 6 << std::setw(10)
            << 1.5 * 4 + 3.5 * 8 << "\n"
            << std::setw(10) << 7.5 * 2 + 12.5 * 6 << std::setw(10)
            << 7.5 * 4 + 12.5 * 8 << std::endl;
  std::cout << "    Actual output:\n";
  m3.Print();
}

void test_Matrix_operator_multiply_Matrices_exception() {
  Matrix m1{{5.1, 6.2}}, m2{{4.13, 23.55, 19.56}};
  std::cout
      << "Overloaded operator * for multiplying Matrices exception test: ";
  try {
    Matrix m3 = m1 * m2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_operator_multiply_equal_Matrices() {
  Matrix m1{{1.5, 3.5}, {7.5, 12.5}}, m2{{2, 4}, {6, 8}};
  m1 *= m2;
  std::cout << "Overloaded operator *= for multiplying Matrices test:"
            << std::endl;
  std::cout << "    Expected output:\n"
            << std::setw(10) << 1.5 * 2 + 3.5 * 6 << std::setw(10)
            << 1.5 * 4 + 3.5 * 8 << "\n"
            << std::setw(10) << 7.5 * 2 + 12.5 * 6 << std::setw(10)
            << 7.5 * 4 + 12.5 * 8 << std::endl;
  std::cout << "    Actual output:\n";
  m1.Print();
}

void test_Matrix_operator_multiply_equal_Matrices_exception() {
  Matrix m1{{5.1, 6.2}}, m2{{4.13, 23.55, 19.56}};
  std::cout
      << "Overloaded operator * for multiplying Matrices exception test: ";
  try {
    m1 *= m2;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_operator_multiply_Matrix_by_Vector() {
  Matrix m1{{1.5, 3.5}, {7.5, 12.5}};
  Vector v1{1.1, 1.2};
  Vector v2 = m1 * v1;
  std::cout << "Overloaded operator * for multiplying Matrix by a Vector test:"
            << std::endl;
  std::cout << "    Expected output:\n"
            << 1.5 * 1.1 + 3.5 * 1.2 << "\n"
            << 7.5 * 1.1 + 12.5 * 1.2 << std::endl;
  std::cout << "    Actual output:\n";
  v2.Print();
}

void test_Matrix_operator_multiply_Matrix_by_Vector_exception() {
  Matrix m1{{5.1, 6.2}};
  Vector v1{4.13, 23.55, 19.56};
  std::cout << "Overloaded operator * for multiplying Matrix by a Vector "
               "exception test: ";
  try {
    Vector v2 = m1 * v1;
    std::cout << "NOT OK!" << std::endl;
  } catch (std::domain_error e) {
    std::cout << "OK!" << std::endl;
  } catch (...) {
    std::cout << "NOT OK!" << std::endl;
  }
}

void test_Matrix_Transpose_method() {
  Matrix m1{
      {1.5, 3.5, 4.5}, {7.5, 12.5, 11.5}, {2.5, 6.5, 9.5}, {5.5, 6.7, 8.9}};
  m1.Transpose();
  std::cout << "Class method Transpose test:" << std::endl;
  std::cout << "    Expected output:\n";
  for (auto x : {1.5, 7.5, 2.5, 5.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {3.5, 12.5, 6.5, 6.7})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {4.5, 11.5, 9.5, 8.9})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  std::cout << "    Actual output:\n";
  m1.Print();
}

void test_Matrix_Transpose_method_quadratic() {
  Matrix m1{{1.5, 3.5, 4.5}, {7.5, 12.5, 11.5}, {2.5, 6.5, 9.5}};
  m1.Transpose();
  std::cout << "Class method Transpose with a quadratic Matrix test:"
            << std::endl;
  std::cout << "    Expected output:\n";
  for (auto x : {1.5, 7.5, 2.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {3.5, 12.5, 6.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {4.5, 11.5, 9.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  std::cout << "    Actual output:\n";
  m1.Print();
}

void test_Matrix_Transpose() {
  Matrix m1{
      {1.5, 3.5, 4.5}, {7.5, 12.5, 11.5}, {2.5, 6.5, 9.5}, {5.5, 6.7, 8.9}};
  Matrix m2 = Transpose(m1);
  std::cout << "Friend function Transpose test:" << std::endl;
  std::cout << "    Expected output:\n";
  for (auto x : {1.5, 7.5, 2.5, 5.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {3.5, 12.5, 6.5, 6.7})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {4.5, 11.5, 9.5, 8.9})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  std::cout << "    Actual output:\n";
  m2.Print();
}

void test_Matrix_Transpose_quadratic() {
  Matrix m1{{1.5, 3.5, 4.5}, {7.5, 12.5, 11.5}, {2.5, 6.5, 9.5}};
  Matrix m2 = Transpose(m1);
  std::cout << "Friend function Transpose with a quadratic Matrix test:"
            << std::endl;
  std::cout << "    Expected output:\n";
  for (auto x : {1.5, 7.5, 2.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {3.5, 12.5, 6.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  for (auto x : {4.5, 11.5, 9.5})
    std::cout << std::setw(10) << x;
  std::cout << std::endl;
  std::cout << "    Actual output:\n";
  m2.Print();
}

int main() {
  std::cout << "Tests for class Vector" << std::endl
            << "-----------------------" << std::endl
            << std::endl;
  test_Vector_ctor1();
  std::cout << std::endl;
  test_Vector_ctor1_exception();
  std::cout << std::endl;
  test_Vector_ctor2();
  std::cout << std::endl;
  test_Vector_ctor2_exception();
  std::cout << std::endl;
  test_Vector_NElems();
  std::cout << std::endl;
  test_Vector_square_brackets();
  std::cout << std::endl;
  test_Vector_round_brackets();
  std::cout << std::endl;
  test_Vector_round_brackets_exception();
  std::cout << std::endl;
  test_Vector_Norm();
  std::cout << std::endl;
  test_Vector_VectorNorm();
  std::cout << std::endl;
  test_Vector_GetEpsilon();
  std::cout << std::endl;
  test_Vector_Print();
  std::cout << std::endl;
  test_Vector_PrintVector();
  std::cout << std::endl;
  test_Vector_operator_plus();
  std::cout << std::endl;
  test_Vector_operator_plus_exception();
  std::cout << std::endl;
  test_Vector_operator_plus_equal();
  std::cout << std::endl;
  test_Vector_operator_plus_equal_exception();
  std::cout << std::endl;
  test_Vector_operator_minus();
  std::cout << std::endl;
  test_Vector_operator_minus_exception();
  std::cout << std::endl;
  test_Vector_operator_minus_equal();
  std::cout << std::endl;
  test_Vector_operator_minus_equal_exception();
  std::cout << std::endl;
  test_Vector_operator_multiply_by_a_scalar();
  std::cout << std::endl;
  test_Vector_operator_multiply_by_a_scalar_equal();
  std::cout << std::endl;
  test_Vector_operator_multiply_Vectors();
  std::cout << std::endl;
  test_Vector_operator_multiply_Vectors_exception();
  std::cout << std::endl;
  test_Vector_operator_divide();
  std::cout << std::endl;
  test_Vector_operator_divide_exception();
  std::cout << std::endl;
  test_Vector_operator_divide_equal();
  std::cout << std::endl;
  test_Vector_operator_divide_equal_exception();
  std::cout << std::endl;

  std::cout << "----------------------------------------------" << std::endl
            << std::endl;

  std::cout << "Tests for class Matrix" << std::endl
            << "-----------------------" << std::endl
            << std::endl;
  test_Matrix_ctor1();
  std::cout << std::endl;
  test_Matrix_ctor1_exception();
  std::cout << std::endl;
  test_Matrix_ctor2();
  std::cout << std::endl;
  test_Matrix_ctor3();
  std::cout << std::endl;
  test_Matrix_ctor3_exception_1();
  std::cout << std::endl;
  test_Matrix_ctor3_exception_2();
  std::cout << std::endl;
  test_Matrix_ctor3_exception_3();
  std::cout << std::endl;
  test_Matrix_NRows();
  std::cout << std::endl;
  test_Matrix_NCols();
  std::cout << std::endl;
  test_Matrix_square_brackets();
  std::cout << std::endl;
  test_Matrix_round_brackets();
  std::cout << std::endl;
  test_Matrix_round_brackets_exception();
  std::cout << std::endl;
  test_Matrix_Norm();
  std::cout << std::endl;
  test_Matrix_MatrixNorm();
  std::cout << std::endl;
  test_Matrix_GetEpsilon();
  std::cout << std::endl;
  test_Matrix_Print();
  std::cout << std::endl;
  test_Matrix_PrintMatrix();
  std::cout << std::endl;
  test_Matrix_operator_plus();
  std::cout << std::endl;
  test_Matrix_operator_plus_exception();
  std::cout << std::endl;
  test_Matrix_operator_plus_equal();
  std::cout << std::endl;
  test_Matrix_operator_plus_equal_exception();
  std::cout << std::endl;
  test_Matrix_operator_minus();
  std::cout << std::endl;
  test_Matrix_operator_minus_exception();
  std::cout << std::endl;
  test_Matrix_operator_minus_equal();
  std::cout << std::endl;
  test_Matrix_operator_minus_equal_exception();
  std::cout << std::endl;
  test_Matrix_operator_multiply_by_a_scalar();
  std::cout << std::endl;
  test_Matrix_operator_multiply_by_a_scalar_equal();
  std::cout << std::endl;
  test_Matrix_operator_multiply_Matrices();
  std::cout << std::endl;
  test_Matrix_operator_multiply_Matrices_exception();
  std::cout << std::endl;
  test_Matrix_operator_multiply_equal_Matrices();
  std::cout << std::endl;
  test_Matrix_operator_multiply_equal_Matrices_exception();
  std::cout << std::endl;
  test_Matrix_operator_multiply_Matrix_by_Vector();
  std::cout << std::endl;
  test_Matrix_operator_multiply_Matrix_by_Vector_exception();
  std::cout << std::endl;
  test_Matrix_Transpose_method();
  std::cout << std::endl;
  test_Matrix_Transpose_method_quadratic();
  std::cout << std::endl;
  test_Matrix_Transpose();
  std::cout << std::endl;
  test_Matrix_Transpose_quadratic();
  std::cout << std::endl;
  return 0;
}
