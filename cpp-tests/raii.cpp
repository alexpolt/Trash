template<class T, class C = void*>
struct raii_ptr {
  T data_m;
  C cb_m;
  explicit raii_ptr( T data ) : data_m{ data }, cb_m{ nullptr } {}
  explicit raii_ptr( T data, C cb ) : data_m{ data }, cb_m{ cb } {}
  raii_ptr( const raii_ptr<T, C>& ) = delete;
  raii_ptr<T,C>& operator=( const raii_ptr<T, C>& ) = delete;
  raii_ptr( raii_ptr<T, C>&& rhs ) : data_m{ rhs.data_m }, cb_m{ rhs.cb_m } { rhs.data_m = 0; }
  ~raii_ptr() { if( data_m ) cb_m( data_m ); }
  T operator->() { return data_m; }
  operator T() { return data_m; }
  bool operator !() { return !data_m; }
  T& data() { return data_m; }
};

template<class T, class C = void*>
raii_ptr<T,C> raii( T data, C cb = nullptr ) {
  return raii_ptr<T,C>{ data, cb };
}
