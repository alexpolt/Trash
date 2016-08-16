

sendmsg2.exe: sendmsg2.cpp
  g++ -static -std=c++11 -Wall -osendmsg2.exe -Os sendmsg2.cpp -lws2_32

