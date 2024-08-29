#include "Person.h"

Person::Person(const std::string& ruc, const std::string& name, const std::string& status, const std::string& domicileCondition)
{
    RUC = ruc;
    NAME = name;
    STATUS = status;
    DOMICILECONDITION = domicileCondition;
}

    std::string Person::getRuc() const { return RUC; }
    std::string Person::getName() const { return NAME; }
    std::string Person::getStatus() const { return STATUS; }
    std::string Person::getDomicileCondition() const { return DOMICILECONDITION; }

    void Person::setRuc(const std::string& ruc) { RUC = ruc; }
    void Person::setName(const std::string& name) { NAME = name; }
    void Person::setStatus(const std::string& status) { STATUS = status; }
    void Person::setDomicileCondition(const std::string& domicileCondition) { DOMICILECONDITION = domicileCondition; }

std::string Person::toString() const {
    return RUC + "," + NAME + "," + STATUS + "," + DOMICILECONDITION;
}
