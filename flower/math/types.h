#pragma once

namespace m {


  TP<TN T0, ssize_t... NN>
  struct vec {

    static constexpr ssize_t size = sizeof...( NN );

    auto& operator[]( ssize_t idx ) { return data[ idx ]; }
    auto& operator[]( ssize_t idx ) const { return data[ idx ]; }

    T0 data[ size ];
  };

  TP<ssize_t... NN>
  using vecf = vec< float, NN... >;


}


