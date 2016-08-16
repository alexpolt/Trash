#ifndef alexpolt_util
#  define alexpolt_util

namespace alexpolt { namespace util {
  
  /*
    Some template helpers.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  template<class T> struct is_reference { static const int value = 0; };
  template<class T> struct is_reference<T&> { static const int value = 1; };

  template<class T> struct remove_reference { using type = T; };
  template<class T> struct remove_reference<T&> { using type = T; };
  
  template<bool C, class A, class B> struct conditional;
  template<class A, class B> struct conditional<true, A, B>{ using type = A; };
  template<class A, class B> struct conditional<false, A, B>{ using type = B; };

  template<class T> struct identity { using type = T; };
  
  template<class T> typename remove_reference<T>::type&& 
    move( T&& value ) { return static_cast< typename remove_reference<T>::type&& > ( value ); }
  
  template<class T> typename conditional< is_reference<T>::value, T, T&& >::type
    forward( typename remove_reference<T>::type& value ) { return static_cast< typename conditional< is_reference<T>::value, T, T&& >::type > ( value ); }
    
  template<int H, int... T> struct conjunction { static const int value = H && conjunction<T...>::value; };
  template<int H> struct conjunction<H> { static const int value = H; };
  template<int H, int... T> struct disjunction { static const int value = H || conjunction<T...>::value; };
  template<int H> struct disjunction<H> { static const int value = H; };


  template<class T> T getValue();
  template<class T> int checkCall( T );

  // Very elegant is_base_of! Proud of it! :)
  template<class A, class B> struct is_base_of { 
    static char isbaseof( A*, A* );
    static int isbaseof( B*, B* );
    static int isbaseof( ... );
    static const int value = sizeof(  isbaseof( (A*)0, (B*)0 )  ) == 1;
  };

  template<int B, class T> struct enable_if;
  template<class T> struct enable_if<1, T>{ using type = T; };

  template<int B, class T> struct disable_if;
  template<class T> struct disable_if<0, T>{ using type = T; };

#define VALTRUE static const int value = 1;
#define VALTFALSE static const int value = 0;
  template<class T> struct is_simple {  VALTFALSE };
  template<> struct is_simple<bool> { VALTRUE };
  template<> struct is_simple<char> { VALTRUE };
  template<> struct is_simple<short> { VALTRUE };
  template<> struct is_simple<unsigned char> { VALTRUE };
  template<> struct is_simple<int> { VALTRUE };
  template<> struct is_simple<unsigned> { VALTRUE };
  template<> struct is_simple<long> { VALTRUE };
  template<> struct is_simple<unsigned long> { VALTRUE };
  template<> struct is_simple<long long> { VALTRUE };
  template<> struct is_simple<unsigned long long> { VALTRUE };
  template<> struct is_simple<float> { VALTRUE };
  template<> struct is_simple<double> { VALTRUE };
  template<class T> struct is_simple<T*> { VALTRUE };


}}

#endif //alexpolt_util



