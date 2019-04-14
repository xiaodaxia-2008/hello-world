#include <string>

class Dog
{
  protected:
    /* data */
    int age;
    std::string sex;
    std::string name;

  public:
    Dog(std::string name, int age, std::string sex);
    Dog();
    int bark();
    void info();
    ~Dog();
};

class SmallDog : public Dog
{
  public:
    SmallDog(std::string name, int age, std::string sex);
    void SetAge(int age);
};
