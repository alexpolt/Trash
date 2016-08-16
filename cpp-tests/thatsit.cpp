#include <stdexcept>
void thatsit() { throw std::runtime_error(__func__); }