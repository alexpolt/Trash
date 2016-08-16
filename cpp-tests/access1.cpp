struct a {
public:
virtual void print();
};
struct b : a {
private:
virtual void print() override;
};
