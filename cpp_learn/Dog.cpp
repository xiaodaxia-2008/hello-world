#include "include/Dog.h"
#include <iostream>

int main()
{
    Dog heizi = Dog("heizi", 12, "male");
    heizi.info();
    heizi.bark();

    SmallDog huanhuan = SmallDog("huanhuan", 12, "femal");
    huanhuan.info();
}

void Dog::info()
{
    std::cout << "name: " << name << "\nage: " << age << "\nsex: " << sex << std::endl;
}

int Dog::bark()
{
    std::cout << "wang wang" << std::endl;
    return 0;
}

Dog::Dog(std::string name, int age, std::string sex) : name(name), age(age), sex(sex) {}
Dog::Dog() {}
Dog::~Dog() {}

SmallDog::SmallDog(std::string name, int age, std::string sex) : Dog(name, age, sex)
{
    if (age > 3)
    {
        std::cout << "this is not small dog." << std::endl;
        SetAge(0);
    }
}

void SmallDog::SetAge(int age)
{
    age = age;
}