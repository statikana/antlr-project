#include <iostream>
#include "koda_src/koda_values.cpp"

int main() {
    // explicit instances
    Instance i = Instance::new_int(42);
    Instance b = Instance::new_bool(true);
    Instance s = Instance::new_string(std::string("hello"));
    std::cout << "int: " << i.to_text() << " (" << i.type->get_name() << ")" << std::endl;
    std::cout << "bool: " << b.to_text() << " (" << b.type->get_name() << ")" << std::endl;
    std::cout << "str: " << s.to_text() << " (" << s.type->get_name() << ")" << std::endl;

    return 0;
}
