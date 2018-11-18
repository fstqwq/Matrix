#ifndef SJTU_MATRIX_HPP
#define SJTU_MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <iterator>

using std::size_t;

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
			void clear() {
				delete [] Data;
				Data = NULL;
				cap = sz = 0;
			}
			Vector (const Vector& b) {
				delete [] Data;
				sz = b.sz;
				cap = sz;
				Data = new T [sz];
				for (size_t i = 0; i < sz; i++) Data[i] = b[i];
			}
			Vector (Vector&& b) {
				delete [] Data;
				Data = b.Data;
				cap = b.cap;
				sz = b.sz;
				b.Data = NULL, b.cap = 0, b.sz = 0;
			}
			Vector &operator = (const Vector &b) {
				delete [] Data;
				sz = b.sz;
				cap = sz;
				Data = new T [sz];
				for (size_t i = 0; i < sz; i++) Data[i] = b[i];
				return *this;
			}
			Vector &operator = (Vector &&b) {
				delete [] Data;
				Data = b.Data;
				cap = b.cap;
				sz = b.sz;
				b.Data = NULL, b.cap = 0, b.sz = 0;
				return *this;
			}
			void push_back(const T &x) {
				if (sz >= cap) reallocate(max(MIN_ALLOCATE, cap * ALLOCATE_RATIO));
				Data[++sz] = x;
			}
			void pop_back() {
				if (sz > 0) {
					sz--;
					if (sz < max(MIN_ALLOCATE, cap / ALLOCATE_RATIO)) {
						reallocate(max(MIN_ALLOCATE, cap / ALLOCATE_RATIO));
					}
				}
			}
			void resize(const size_t &newsz) {
				if (newsz > cap) {
					reallocate(max(MIN_ALLOCATE, newsz));
					while (sz < newsz) Data[sz++] = T();
				}
				else if (newsz < max(MIN_ALLOCATE, cap / ALLOCATE_RATIO)) {
					sz = newsz;
					reallocate(max(MIN_ALLOCATE, cap / ALLOCATE_RATIO));
				}
			}
			void assign(const size_t &newsz, const T &_init) {
				if (cap < newsz || newsz < max(MIN_ALLOCATE, cap / ALLOCATE_RATIO)) {
					reallocate(newsz);
				}
				sz = newsz;
				for (size_t i = 0; i < sz; i++) Data[i] = _init();
			}
			size_t size() {
				return sz;
			}
			size_t capacity() {
				return cap;
			}
			T* data() {
				return Data;
			}
			T& operator [] (const size_t &i) {
				return Data[i];
			}
	};
	
	template <class T>
	class Matrix {
	private:
		// your private member variables here.
		Vector <T> Data;
		size_t R, C;
	public:
		Matrix() = default;
		
		Matrix(size_t n, size_t m, T _init = T())
		{
			R = n;
			C = m;
			Data.assign(R * C, _init);
		}
		
		explicit Matrix(std::pair<size_t, size_t> sz, T _init = T())
		{
			R = sz.first;
			C = sz.second;
			Data.assign(R * C, _init);
		}
	
		Matrix(const Matrix &o)
		{
			R = o.R;
			C = o.C;
			Data = o.Data;
		}
		
		template <class U>
		Matrix(const Matrix<U> &o)
		{
			R = o.R;
			C = o.C;
			Data.resize(R * C);
			for (size_t i = 0; i < R * C; i++) {
				Data[i] = static_cast<T>(o.Data[i]);
			}
		}
		
		Matrix &operator=(const Matrix &o)
		{
			R = o.R;
			C = o.C;
			Data = o.Data;
			return *this;
		}
		
		template <class U>
		Matrix &operator=(const Matrix<U> &o)
		{
			R = o.R;
			C = o.C;
			Data.resize(R * C);
			for (size_t i = 0; i < R * C; i++) {
				Data[i] = static_cast<T>(o.Data[i]);
			}
			return *this;
		}
		
		Matrix(Matrix &&o) noexcept
		{
			
		}
		
		Matrix &operator=(Matrix &&o) noexcept
		{
			
		}
		
		~Matrix() { }
		
		Matrix(std::initializer_list<std::initializer_list<T>> il)
		{
			
		}
		
	public:
		size_t rowLength() const { }
		
		size_t columnLength() const { }
		
		void resize(size_t _n, size_t _m, T _init = T())
		{
			
		}
		
		void resize(std::pair<size_t, size_t> sz, T _init = T())
		{
			
		}
		
		std::pair<size_t, size_t> size() const
		{
			
		};
		
		void clear()
		{
			
		}
		
	public:
		const T &operator()(size_t i, size_t j) const
		{
			
		}
		
		T &operator()(size_t i, size_t j)
		{
			
		}
		
		Matrix<T> row(size_t i) const
		{
			
		}
		
		Matrix<T> column(size_t i) const
		{
			
		}
		
		
	public:
		template <class U>
		bool operator==(const Matrix<U> &o) const
		{
			
		}
		
		template <class U>
		bool operator!=(const Matrix<U> &o) const
		{
			
		}
		
		Matrix operator-() const
		{
			
		}
		
		template <class U>
		Matrix &operator+=(const Matrix<U> &o)
		{
			
		}
		
		template <class U>
		Matrix &operator-=(const Matrix<U> &o)
		{
			
		}
		
		template <class U>
		Matrix &operator*=(const U &x)
		{
			
		}
		
		Matrix tran() const
		{
			
		}
		
	public: // iterator
		class iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type        = T;
			using pointer           = T *;
			using reference         = T &;
			using size_type         = size_t;
			using difference_type   = std::ptrdiff_t;
			
			iterator() = default;
			
			iterator(const iterator &) = default;
			
			iterator &operator=(const iterator &) = default;
			
		private:

			
		public:
			difference_type operator-(const iterator &o)
			{
				
			}
			
			iterator &operator+=(difference_type offset)
			{
				
			}
			
			iterator operator+(difference_type offset) const
			{
				
			}
			
			iterator &operator-=(difference_type offset)
			{
				
			}
			
			iterator operator-(difference_type offset) const
			{
				
			}
			
			iterator &operator++()
			{
				
			}
			
			iterator operator++(int)
			{
				
			}
			
			iterator &operator--()
			{
				
			}
			
			iterator operator--(int)
			{
				
			}
			
			reference operator*() const
			{
				
			}
			
			pointer operator->() const
			{
				
			}
			
			bool operator==(const iterator &o) const
			{
				
			}
			
			bool operator!=(const iterator &o) const
			{
				
			}
		};
		
		iterator begin()
		{
			
		}
		
		iterator end()
		{
			
		}
		
		std::pair<iterator, iterator> subMatrix(std::pair<size_t, size_t> l, std::pair<size_t, size_t> r)
		{
			
        }
	};
		
}

//
namespace sjtu
{
	template <class T, class U>
	auto operator*(const Matrix<T> &mat, const U &x)
	{
		
	}
	
	template <class T, class U>
	auto operator*(const U &x, const Matrix<T> &mat)
	{
		
	}
	
	template <class U, class V>
	auto operator*(const Matrix<U> &a, const Matrix<V> &b)
	{
		
	}
	
	template <class U, class V>
	auto operator+(const Matrix<U> &a, const Matrix<V> &b)
	{
		
	}
	
	template <class U, class V>
	auto operator-(const Matrix<U> &a, const Matrix<V> &b)
	{
		
	}
	
}

#endif //SJTU_MATRIX_HPP

