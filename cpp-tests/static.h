
inline int* getdata() { 
  static int data = 0;
  return &data;
}
