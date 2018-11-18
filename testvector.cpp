#include <bits/stdc++.h>
using namespace std;

using std::size_t;
using std::min;
using std::max;

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
			void clear() {
				delete [] Data;
				Data = NULL;
				cap = sz = 0;
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
				for (size_t i = 0; i < sz; i++) Data[i] = _init();
			}
			Vector (std::initializer_list<T> il) {
				resize(il.size());
				sz = 0;
				for (auto i : il) {
					Data[sz++] = i;
				}
			}
			Vector (std::initializer_list<std::initializer_list<T>> il) {
				resize(il.size() * il.begin()->size());
				sz = 0;
				for (auto i : il) {
					for (auto j : i) {
						Data[sz++] = j;
					}
				}
			}

	};

Vector <int> a;

Vector <int> b() {
	return Vector<int>();
}

int main() {
	a = b();
	for (int i = 0; i < 100; i++) {
		a.push_back(1);
		printf("%d %d %d\n", a[i], (int)a.size(), (int)a.capacity());
	}
	return 0;
}
