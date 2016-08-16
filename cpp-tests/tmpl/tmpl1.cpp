#include <iostream>


using namespace std;

template <typename T, int count = 10>
class stack 
{
public:
	T* data;
	size_t head;

	stack(int _count=10) { head = 0; data = new T[count]; for(int i=0;i<count;i++) data[i] = 0; }
	~stack() {}

	stack( stack<T> const & s ) {
		cout << "copy constructor\n";
	}

	void operator=( const stack<T> & s ) {
		cout << "operator=\n";
	}

	T const pop() { if(!head) return 0; return data[ --head ]; }

	void push( const T& t ) { data[ head++ ] = t; print(); }
	
	void print() { for(int i=0;i<count;i++) cout << data[i] << " "; cout << ", head = " << head << "\n"; }

};


int main(void) {
	cout << "Start\n";
	stack<int> stack1;
	stack<int> stack2;
	int b, & a = b;

	cout << "PUSH 10 : "; stack1.push( 10 );
	cout << "PUSH 11 : "; stack1.push( 11 );
	cout << "POP: " << stack1.pop() << "\n";
	cout << "POP: " << stack1.pop() << "\n";

	stack1.push( 2 );
	stack1.push( 3 );
	a = stack1.pop();
	a = 5;
	stack1.print();
	cout << a << "\n";
	cout << b << "\n";
	stack2.push( 4 );
	stack2.push( 5 );
	
	stack1 = stack2;
return 0;
}

