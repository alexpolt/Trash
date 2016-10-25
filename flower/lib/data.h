
namespace lib{

  struct data_ptr { 

      data_ptr() : _ptr{} { }

      ~data_ptr() { 

        auto ptr = type_cast< ssize_t* >( & _ptr );

        if( is_owner and *ptr != 0 ) 

            type_cast< T0* >( & _ptr )->~T0(); 
      }

      data_ptr( data_ptr const& other ) noexcept { 

        memcpy( _ptr, other._ptr, value_size ); 
      }

      data_ptr( data_ptr&& other ) noexcept { 

        memcpy( _ptr, other._ptr, value_size ); 
        memset( other._ptr, 0, value_size );
      }

      auto& operator=( data_ptr const& other ) noexcept {

        memcpy( _ptr, other._ptr, value_size ); 

        return $this;
      }

      auto& operator=( data_ptr&& other ) noexcept {

        memcpy( _ptr, other._ptr, value_size ); 
        memset( other._ptr, 0, value_size );

        return $this;
      }

      char* get() { return _ptr; }

      char const* get() const { return _ptr; }
      
      char _ptr[ value_size ]; 
    };


    alignas( alignof( T0 ) ) data_ptr _data;

  };

}


