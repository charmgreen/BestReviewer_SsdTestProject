#include<map>
#include<string>

class SSD{
public:
    virtual void Read();
    virtual void Write();

private:
    std::map<int, std::string> memory;
};
