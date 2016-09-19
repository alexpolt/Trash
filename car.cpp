#include <iostream>
#include <memory>
#include <exception>
#include <vector>
#include <algorithm>


namespace vehicle {

  using input_t = char;

  enum class car_state {           
    north, east, west, south
  };

  enum class car_action {
    beep
  };

  //car interface
  class car {

    public:

    virtual ~car() {}
    virtual void command( input_t ) = 0;
    virtual void print_state() = 0;

    private:

    virtual car_state get_state() const = 0;
    virtual void set_state( car_state ) = 0;
    virtual bool do_action( car_action ) = 0;
    virtual void set_action( car_action ) = 0;

  };

  using car_uptr = std::unique_ptr<car>;

  struct bad_input : std::exception {
    char const* what() const noexcept override {
      return "bad input";
    }
  };

  //car control class
  struct control_common {
   
    template<typename car_t>
    static void process_input( car_t obj, input_t cmd ) {
      
      auto state = obj->get_state();
      
      switch( cmd ) {
        
        case 'L' : 
          switch( state ) {
            case car_state::north : obj->set_state( car_state::west ); break;
            case car_state::west  : obj->set_state( car_state::south ); break;
            case car_state::east  : obj->set_state( car_state::north ); break;
            case car_state::south : obj->set_state( car_state::east ); break;
          }
          break;
        
        case 'R' :
          switch( state ) {
            case car_state::north : obj->set_state( car_state::east ); break;
            case car_state::west  : obj->set_state( car_state::north ); break;
            case car_state::east  : obj->set_state( car_state::south ); break;
            case car_state::south : obj->set_state( car_state::west ); break;
          }
          break;
        
        case 'B' :
          obj->set_action( car_action::beep );
          break;
        
        default:
          throw bad_input{};
      }
      
    }
    
  };

  //a car implementation, parameterized by a control
  template< typename control_t >
  class bmw : public car {

    friend control_t;

    public:
      
    void command( input_t cmd ) override {
      control_t::process_input( this, cmd );
    }
 
    void print_state() override {

      switch( state ) {
        case car_state::north:
          std::cout << "^";
          break;
        case car_state::west:
          std::cout << "<";
          break;
        case car_state::east:
          std::cout << ">";
          break;
        case car_state::south:
          std::cout << "v";
          break;
        default:
          std::cout << "error: no such state";
      }

      if( do_action( car_action::beep ) ) std::cout << " beep";

      std::cout << std::endl;

    }

    private:

    car_state get_state() const override {
      return state;
    }
    
    void set_state( car_state state_new ) override {
        state = state_new;
    }

    void set_action( car_action action ) override {
      actions.push_back( action );
    }

    bool do_action( car_action action ) override {

      auto it_end = end( actions );
      auto it = std::find( begin( actions ), it_end, action );

      if( it != it_end ) {
        actions.erase( std::remove( begin(actions), it_end, action ), it_end );
        return true;
      }

      return false;
    }
    
    car_state state = car_state::north;
    using actions_t = std::vector< car_action >;
    actions_t actions;
  };

  //car factory
  template< typename control_type >
  car_uptr make_unique_bmw() {
    return car_uptr{ new bmw<control_type>{} };
  }

}



int main() {

 auto car = vehicle::make_unique_bmw< vehicle::control_common >();

 std::cout << "enter car command ( L - left, R - right, B - beep, E - exit )" << std::endl;

 car->print_state();

 while( true ) {

   char cmd;

   std::cin >> cmd;

   if( cmd == 'E' ) break;

   try {

    car->command( cmd );

    car->print_state();

   } catch ( vehicle::bad_input& e ) {
     std::cout << "use only L, R or B" << std::endl;
   }

 }

}

