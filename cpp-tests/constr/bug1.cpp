 #include <iostream>
 #include <string>

 void f(const char *p, const char *q) {
     std::cout << "hello, world" + '!';

     if (p != q) {
           std::string s = false;
           std::cout << s;
     }
 }

 int main() {
     f(2*2-4, "false");
 }

	