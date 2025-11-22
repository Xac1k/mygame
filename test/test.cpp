#include <iostream>
#include <vector>
#include <memory>

class I {
public:
   virtual void Method1() = 0;
   virtual void Method2() = 0;
protected:
   int a;
private:
   int b;
};

class C : public I {
public:
  void Method1() override
  {
    std::cout << "C::Method1" << std::endl;
  }
  void Method2() override
  {
        std::cout << "C::Method2" << std::endl;
  }
};
class D : public I {
public:
  void Method1() override
  {
    std::cout << "D::Method1" << std::endl;
  }
  void Method2() override
  {
        std::cout << "D::Method2" << std::endl;
  }
};



int main() {
    // std::vector<std::shared_ptr<I>> is;
    // is.push_back(std::make_shared<C>());
    // is.push_back(std::make_shared<D>());
    // is.push_back(std::make_shared<D>());
    // is.push_back(std::make_shared<C>());

    // for (const auto& i : is) {
    //     i->Method1();
    //     i->Method2();
    // }


}