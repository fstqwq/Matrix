#ifndef SJTU_MATRIX_HPP
#define SJTU_MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <iterator>

using std::size_t;
using std::min;
using std::max;
using std::swap;

namespace sjtu
{
	template <class T, int ALLOCATE_RATIO = 2, size_t MIN_ALLOCATE = 8>
	class Vector {
		private:
			T *Data;
			size_t sz, cap;
			void reallocate(const size_t &newcap) {
				T *newData = new T [newcap];
				sz = min(sz, newcap);
				for (size_t i = 0; i < sz; i++) newData[i] = Data[i];
				delete [] Data;
				Data = newData;
				cap = newcap;
			}
		
		public:
			Vector () {
				cap = sz = 0, Data = NULL;
			}
			Vector (size_t SZ) {
				cap = sz = SZ;
				Data = new T [sz];
			}
			size_t size() const {
				return sz;
			}
			size_t capacity() const {
				return cap;
			}
			T* data() {
				return Data;
			}
			
			const T* data() const {
				return Data;
			}
			T& operator [] (const size_t &i) {
				return Data[i];
			}
			const T& operator [] (const size_t &i) const {
				return Data[i];
			}
			Vector (const Vector& b) {
				sz = b.sz;
				cap = sz;
				Data = new T [sz];
				for (size_t i = 0; i < sz; i++) Data[i] = b[i];
			}
			Vector (Vector&& b) {
				Data = b.Data;
				cap = b.cap;
				sz = b.sz;
				b.stealedClear();
			}
			void clear() {
				delete [] Data;
				Data = NULL;
				cap = sz = 0;
			}
			void stealedClear() {
				cap = sz = 0, Data = NULL;
			}
			void resize(const size_t &newsz, const T _init = T()) {
				if (newsz > cap) {
					reallocate(max(MIN_ALLOCATE, newsz));
					while (sz < newsz) Data[sz++] = _init;
				}
				else if (newsz < max(MIN_ALLOCATE, cap / ALLOCATE_RATIO)) {
					sz = newsz;
					reallocate(max(MIN_ALLOCATE, cap / ALLOCATE_RATIO));
				}
			}
			Vector &operator = (const Vector &b) {
				if (Data != b.data()) {
					delete [] Data;
					sz = b.size();
					cap = sz;
					Data = new T [sz];
					for (size_t i = 0; i < sz; i++) Data[i] = b[i];
				}
				return *this;
			}
			Vector &operator = (Vector &&b) {
				if (Data != b.data()) {
					delete [] Data;
					Data = b.data();
					cap = b.capacity();
					sz = b.size();
					b.stealedClear();
				}
				return *this;
			}
			template <class U>
			bool operator == (const Vector<U> &b) const {
				if (sz != b.size()) return false;
				for (size_t i = 0; i < sz; i++) if (Data[i] != b[i]) return false;
				return true;
			}
			void push_back(const T &x) {
				if (sz >= cap) reallocate(max(MIN_ALLOCATE, cap * ALLOCATE_RATIO));
				Data[sz++] = x;
			}
			void pop_back() {
				if (sz > 0) {
					sz--;
					if (sz < max(MIN_ALLOCATE, cap / ALLOCATE_RATIO)) {
						reallocate(max(MIN_ALLOCATE, cap / ALLOCATE_RATIO));
					}
				}
			}
			void assign(const size_t &newsz, const T &_init) {
				if (cap < newsz || newsz < max(MIN_ALLOCATE, cap / ALLOCATE_RATIO)) {
					reallocate(newsz);
				}
				sz = newsz;
				for (size_t i = 0; i < sz; i++) Data[i] = _init;
			}
			template <class U>
			Vector (const std::initializer_list<std::initializer_list<U>> &il) {
				sz = 0, cap = il.size() * il.begin()->size();
				Data = new T [cap];
				for (auto &i : il) {
					for (auto &j : i) {
						Data[sz++] = T(j);
					}
				}
			}

	};
	
	template <class T>
	class Matrix {
	private:
		Vector <T> Data;
		size_t R, C;
	public:
		Matrix() : Data(), R(0), C(0) {}
		
		Matrix(size_t n, size_t m, T _init = T()) : Data(), R(n), C(m)
		{
			Data.assign(R * C, _init);
		}
		
		explicit Matrix(std::pair<size_t, size_t> sz, T _init = T()) : Data(), R(sz.first), C(sz.second)
		{
			Data.assign(R * C, _init);
		}
	
		Matrix(const Matrix &o) : Data(o.data()), R(o.rowLength()), C(o.columnLength()) {}

		template <class U>
		Matrix(const Matrix<U> &o) : Data(R * C), R(o.rowLength()), C(o.columnLength()) {
			for (size_t i = 0; i < R * C; i++) {
				Data[i] = T(o[i]);
			}
		}
		
		Matrix &operator=(const Matrix &o)
		{
			R = o.rowLength();
			C = o.columnLength();
			Data = o.data();
			return *this;
		}
		
		template <class U>
		Matrix &operator=(const Matrix<U> &o)
		{
			R = o.rowLength();
			C = o.columnLength();
			Data.resize(R * C);
			for (size_t i = 0; i < R * C; i++) {
				Data[i] = T(o[i]);
			}
			return *this;
		}
		
		Matrix(Matrix &&o) noexcept
		{
			R = o.rowLength();
			C = o.columnLength();
			Data = std::move(o.Data);
			o.data().stealedClear();
		}
		
		Matrix &operator=(Matrix &&o) noexcept
		{
			R = o.rowLength();
			C = o.columnLength();
			Data = std::move(o.Data);
			o.data().stealedClear();
			return *this;
		}
		
		~Matrix() {
			Data.clear();
		}
		
		Matrix(std::initializer_list<std::initializer_list<T>> il)
		{
			R = il.size();
			C = il.begin()->size();
			Data = Vector<T>(il);
		}
		
	public:
		size_t rowLength() const { return R; }
		
		size_t columnLength() const { return C; }

		T &operator [] (const size_t x) {
			return Data[x];
		}
		const T &operator [] (const size_t x) const {
			return Data[x];
		}

		Vector<T> & data() {
			return Data;
		}

		const Vector<T> & data() const {
			return Data;
		}
		
		size_t Size() const {return R * C;}

		void resize(size_t _n, size_t _m, T _init = T())
		{
			Data.resize(_n * _m, _init);
			R = _n, C = _m;
		}
		
		void resize(std::pair<size_t, size_t> sz, T _init = T())
		{
			Data.resize(sz.first * sz.second, _init);
			R = sz.first, C = sz.second;
		}
		
		std::pair<size_t, size_t> size() const
		{
			return std::make_pair(R, C);
		};
		
		void clear()
		{
			R = 0, C = 0;
			Data.clear();
		}
	
	public:
		const T &operator()(size_t i, size_t j) const
		{
			return Data[i * C + j];
		}
		
		T &operator()(size_t i, size_t j)
		{
			return Data[i * C + j];
		}
		
		Matrix<T> row(size_t i) const
		{
			Matrix ret(1, C);
			for (size_t c = 0; c < C; c++) {
				ret(0, c) = Data[i * C + c];
			}
			return ret;
		}
		
		Matrix<T> column(size_t i) const
		{
			Matrix ret(R, 1);
			for (size_t r = 0; r < R; r++) {
				ret(r, 0) = Data[i * r + C];
			}
			return ret;
		}
		
	public:
		template <class U>
		bool operator==(const Matrix<U> &o) const
		{
			return R == o.rowLength() && C == o.columnLength() && Data == o.data(); //XXX
		}
		
		template <class U>
		bool operator!=(const Matrix<U> &o) const
		{
			return !(R == o.rowLength() && C == o.columnLength() && Data == o.data()); //XXX
		}
		
		Matrix operator-() const
		{
			Matrix ret(*this);
			for (size_t i = 0; i < R * C; i++) {
				ret[i] = -ret[i];
			}
			return ret;
		}
		
		template <class U>
		Matrix &operator+=(const Matrix<U> &o)
		{
			// check sz
			for (size_t i = 0; i < Data.size(); i++) Data[i] = T(Data[i] + o.Data[i]);
			return *this;
		}
		
		template <class U>
		Matrix &operator-=(const Matrix<U> &o)
		{
			for (size_t i = 0; i < Data.size(); i++) Data[i] = T(Data[i] - o.Data[i]);
			return *this;
		}
		
		template <class U>
		Matrix &operator*=(const U &x)
		{
			for (size_t i = 0; i < Data.size(); i++) Data[i] = T(Data[i] * x);
			return *this;
		}
		
		Matrix tran() const
		{
			Matrix tmp(C, R);
			for (size_t i = 0; i < R; i++)
				for (size_t j = 0; j < C; j++) {
					tmp(j, i) = (*this)(i, j);
				}
			return tmp;
		}
		
	public: // iterator
		class iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type		= T;
			using pointer		   = T *;
			using reference		 = T &;
			using size_type		 = size_t;
			using difference_type   = std::ptrdiff_t;
			
			iterator() = default;
			
			iterator(const iterator &) = default;
			
			iterator &operator=(const iterator &) = default;
			
		private:
			size_t sx, sy, ex, ey, x, y;
			Matrix *mat;	

			difference_type id() const {
				return difference_type((x - sx) * (ey - sy + 1) + y);
			}
		public:

			iterator(size_t _sx, size_t _sy, size_t _ex, size_t _ey, size_t _x, size_t _y, Matrix * a) {
				sx = _sx;
				sy = _sy;
				ex = _ex;
				ey = _ey;
				x = _x;
				y = _y;
				mat = a;
			}

			difference_type operator-(const iterator &o) const
			{
				if (sx != o.sx || sy != o.sy || ex != o.ex || ey != o.ey || mat != o.mat) return difference_type(-1);
				return difference_type(id() - o.id());
			}
			
			iterator &operator+=(difference_type offset)
			{
				x += offset / (ey - sy + 1);
				y += offset % (ey - sy + 1);
				if (y > ey) {
					y -= (ey - sy + 1);
					x += 1;
				}
				if (x > ex || (x == ex && y > ey + 1)) {
					// XXX
				}
				return *this;
			}
			
			iterator operator+(difference_type offset) const
			{
				iterator ret(*this);
				ret += offset;
				return ret;
			}
			
			iterator &operator-=(difference_type offset)
			{
				if (id() < offset) {
					// XXX
				}
				else {
					x -= offset / (ey - sy + 1);
					y -= offset % (ey - sy + 1);
					if (y < sy) {
						y += (ey - sy + 1);
						x -= 1;
					}
				}
				return *this;
				
			}
			
			iterator operator-(difference_type offset) const
			{
				iterator ret(*this);
				ret -= offset;
				return ret;
			}
			
			iterator &operator++()
			{
				y++;
				if (x < ex && y > ey) y = sy, x++;
				return *this;
			}
			
			iterator operator++(int)
			{
				iterator ret(*this);
				y++;
				if (x < ex && y > ey) y = sy, x++;
				return ret;
			}
			
			iterator &operator--()
			{
				if (y == sy) {
					x--, y = ey;
				}
				else {
					y--;
				}
				return *this;
			}
			
			iterator operator--(int)
			{
				iterator ret(*this);
				if (y == sy) {
					x--, y = ey;
				}
				else {
					y--;
				}
				return ret;
			}
			
			reference operator*() const
			{
				return (*mat)(x, y);
			}
			
			pointer operator->() const
			{
				return &((*mat)(x, y));
			}
			
			bool operator==(const iterator &o) const
			{
				return x == o.x && y == o.y && sx == o.sx && sy == o.sy && ex == o.ex && ey == o.ey && mat == o.mat;
			}
			
			bool operator!=(const iterator &o) const
			{
				return !(x == o.x && y == o.y && sx == o.sx && sy == o.sy && ex == o.ex && ey == o.ey && mat == o.mat);
			}
		};
		
		iterator begin()
		{
			return iterator(0, 0, R - 1, C - 1, 0, 0, this);
		}
		
		iterator end()
		{
			return iterator(0, 0, R - 1, C - 1, R - 1, C - (R == 0 || C == 0), this);
		}
		
		std::pair<iterator, iterator> subMatrix(std::pair<size_t, size_t> l, std::pair<size_t, size_t> r)
		{
			return std::make_pair(iterator(l.first, l.second, r.first, r.second, l.first, l.second, this), iterator(l.first, l.second, r.first, r.second, r.first, r.second + 1, this));
		}
	};
		
}

//
namespace sjtu
{
	template <class T, class U>
	auto operator*(const Matrix<T> &mat, const U &x)->Matrix<decltype(T() * U())>
	{
		Matrix<decltype(T() * U())> ret(mat);
		for (size_t i = 0; i < mat.rowLength(); i++) {
			for (size_t j = 0; j < mat.columnLength(); j++) {
				ret(i, j) *= x;
			}
		}
		return ret;
	}
	
	template <class T, class U>
	auto operator*(const U &x, const Matrix<T> &mat)->Matrix<decltype(T() * U())>
	{
		Matrix<decltype(T() * U())> ret(mat);
		for (size_t i = 0; i < mat.rowLength(); i++) {
			for (size_t j = 0; j < mat.columnLength(); j++) {
				ret(i, j) *= x;
			}
		}
		return ret;
	}
	
	template <class U, class V>
	auto operator*(const Matrix<U> &a, const Matrix<V> &b)->Matrix<decltype(U() * V())>
	{
		// Assert a.C = b.R
		Matrix<decltype(U() * V())> ret(a.rowLength(), b.columnLength(), 0);
		for (size_t i = 0; i < a.rowLength(); i++)
			for (size_t k = 0; k < a.columnLength(); k++)
				for (size_t j = 0; j < b.columnLength(); j++) {
					ret(i, j) += a(i, k) * b(k, j);
				}
		return ret;
	}
	
	template <class U, class V>
	auto operator+(const Matrix<U> &a, const Matrix<V> &b)->Matrix<decltype(U() + V())>
	{
		// Assert a.R = b.R && a.C == b.C
		Matrix<decltype(U() * V())> ret(a.rowLength(), b.columnLength(), 0);
		for (size_t i = 0; i < a.rowLength(); i++)
			for (size_t j = 0; j < a.columnLength(); j++) {
				ret(i, j) = a(i, j) + b(i, j);
			}
		return ret;
	}
	
	template <class U, class V>
	auto operator-(const Matrix<U> &a, const Matrix<V> &b)->Matrix<decltype(U() + V())>
	{
		// Assert a.R = b.R && a.C == b.C
		Matrix<decltype(U() * V())> ret(a.rowLength(), b.columnLength(), 0);
		for (size_t i = 0; i < a.rowLength(); i++)
			for (size_t j = 0; j < a.columnLength(); j++) {
				ret(i, j) = a(i, j) - b(i, j);
			}
		return ret;
	}
	
}

#endif //SJTU_MATRIX_HPP

