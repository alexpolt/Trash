class Testable {
    bool ok_;
    typedef void (Testable::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}
  public:
    explicit Testable(bool b=true):ok_(b) {}
 
    operator bool_type() const {
      return ok_==true ? 
        &Testable::this_type_does_not_support_comparisons : 0;
    }
  };
class AnotherTestable ... // Identical to Testable.
{};
int main (void)
{
  Testable t1;
  AnotherTestable t2;
  if (t1) {} // Works as expected
  if (t2 == t1) {} // Fails to compile
  if (t1 < 0) {} // Fails to compile
 
  return 0;
}
