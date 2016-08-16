enum newtag {
tag1, tag2, tag3
};


void* operator new( unsigned size, newtag t1, newtag t2 ) {
return (void*)0;
}


//#define new2
#define new2(...) __VA_ARGS__
#define new new2(tag1,tag2)


int main() {
  new int;
  new( tag1, tag2 ) int;
}

