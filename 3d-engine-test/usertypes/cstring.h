#ifndef alexpolt_cstring
#  define alexpolt_cstring


namespace alexpolt {

  /*
    Constant string.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */


struct cstring {
    const char* data_m;
    size_t size_m;

    default_copy_assign( cstring )
    default_copy_assign_move( cstring )

    cstring() = default;
  
    constexpr cstring( const char* str, size_t size ) : data_m{ str }, size_m{ size } {}
    
    ~cstring() = default;

    operator const char*() { return data_m; }

    size_t size() const { return size_m; }
    const char* data() const { return data_m; }
    const char* c_str() const { return data_m; }

    bool empty() { return size_m <= 1; }
};

constexpr cstring cnullstring{nullptr, 0};
constexpr cstring cemptystring{"", 1};

inline cstring operator"" _str( const char* str, unsigned size ) { return cstring{ str, size }; }


}

#endif //alexpolt_cstring
