#include <iostream>
#include <memory>
#include <exception>


namespace vehicle {

  using input_char = char;

  enum class car_state {           
    north, east, west, south
  };


  //car interface
  struct car {
    virtual void command( input_char ) = 0;
    virtual car_state get_state() = 0;
    virtual void set_state( car_state ) = 0;
    virtual bool do_beep() = 0;
    virtual void set_beep() = 0;
    virtual ~car() {}
  };

  //car control interface
  struct control {
    virtual void process_input( car*, input_char ) = 0;
    virtual ~control() {}
  };


  struct bad_input : std::exception {
    char const* what() const noexcept override {
      return "bad input";
    }
  };


  //basic control implementation
  struct control_common : control {
    
    void process_input( car* obj, input_char cmd ) override {
      
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
          obj->set_beep();
          break;
        
        default:
          throw bad_input{};
      }
      
    }
    
  };
  

  //a car implementation, parameterized by a control
  template< typename control_type >
  struct bmw : car {
      
    void command( input_char cmd ) override {
      ctrl.process_input( this, cmd );
    }
    
    car_state get_state() override {
      return state;
    }
    
    void set_state( car_state state_new ) override {
        state = state_new;
    }

    void set_beep() override {
      beep = true;
    }

    bool do_beep() override {
      bool b = beep;
      beep = false;
      return b;
    }
    
    car_state state = car_state::north;
    bool beep = false;
    control_type ctrl;
  };


  //car factory
  template< typename control_type >
  car* make_bmw() {
    return new bmw<control_type>{};
  }

}

//print out car state
void print_state( std::unique_ptr<vehicle::car> const& car ) {

  using vehicle::car_state;

  switch( car->get_state() ) {
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

  if( car->do_beep() )
    std::cout << " beep";

  std::cout << std::endl;
}


int main() {

 std::unique_ptr<vehicle::car> car{ vehicle::make_bmw< vehicle::control_common >() };

 std::cout << "enter car command ( L - left, R - right, B - beep, E - exit )" << std::endl;

 print_state( car );

 while( true ) {

   char cmd;
   std::cin >> cmd;

   if( cmd == 'E' ) break;

   try {

    car->command( cmd );
    print_state( car );

   } catch ( vehicle::bad_input& e ) {

     std::cout << "use only L, R or B" << std::endl;

   }

 }

}

