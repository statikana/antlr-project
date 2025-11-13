#include <map>
#include <vector>
#include "koda_values.cpp"

typedef std::map<std::string, Instance> InstanceMap;

class Scope {
    private:
        InstanceMap map;
    
    public:
        Scope() {}

        Instance& operator[](std::string name) {
            if (!is_defined(name)) {
                throw name + " not defined. Use .define";
            }
            return map.at(name);
        }

        void define(std::string name, const Instance& value) {
            map.insert_or_assign(name, value);
        }



        bool is_defined(std::string name) {
            return map.count(name) != 0;
        }

        std::vector<std::string> names() {
            std::vector<std::string> names_vec;
            for (auto const& val: map) {
                names_vec.push_back(val.first);
            }
            return names_vec;
        }

        void print() {
            std::cout << "Current Scope:" << std::endl;
            int counter = 1;
            for (const auto& vals: map) {
                std::cout << "Defintion " << std::to_string(counter) << ":" << std::endl;
                std::cout << "  Name:  '" << vals.first << "'" << std::endl;
                std::cout << "  Value: " << vals.second.to_text() << std::endl;
                std::cout << "  Type:  '"<< vals.second.type->get_name() << "'";
                std::cout << std::endl << std::endl;
                counter++;
            }
        }
};