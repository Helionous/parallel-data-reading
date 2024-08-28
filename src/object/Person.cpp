#include <string>
#include <iostream>

class Person {
    private:
        std::string idPerson;
        std::string firstName;
        std::string surName;
        std::string DNI;
        int age;

    public:
        // Constructor
        Person(const std::string& id, const std::string& first, const std::string& last, const std::string& dni, int age)
            : idPerson(id), firstName(first), surName(last), DNI(dni), age(age) {}

        // Getters
        std::string getIdPerson() const { return idPerson; }
        std::string getFirstName() const { return firstName; }
        std::string getSurName() const { return surName; }
        std::string getDNI() const { return DNI; }
        int getAge() const { return age; }

        // Setters
        void setIdPerson(const std::string& id) { idPerson = id; }
        void setFirstName(const std::string& first) { firstName = first; }
        void setSurName(const std::string& last) { surName = last; }
        void setDNI(const std::string& dni) { DNI = dni; }
        void setAge(int a) { age = a; }

        std::string toString() const {
            return idPerson + "," + firstName + "," + surName + "," + DNI + "," + std::to_string(age);
        }

        friend std::ostream& operator<<(std::ostream& os, const Person& person) {
            os << person.toString();
            return os;
        }
};
