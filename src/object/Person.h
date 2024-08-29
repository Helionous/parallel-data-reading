#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
public:
    std::string RUC;
    std::string NAME;
    std::string STATUS;
    std::string DOMICILECONDITION;

    Person() = default;
    Person(const std::string& ruc, const std::string& name, const std::string& status, const std::string& domicileCondition);

    std::string getRuc() const;
    std::string getName() const;
    std::string getStatus() const;
    std::string getDomicileCondition() const;

    void setRuc(const std::string& ruc);
    void setName(const std::string& name);
    void setStatus(const std::string& status);
    void setDomicileCondition(const std::string& domicileCondition);

    std::string toString() const;
};

#endif // PERSON_H
