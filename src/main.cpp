
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <functional>
#include <thread>
#include <cassert>

#include <database.hpp>


struct my_data {
  std::string name;
  uint32_t  id;
  std::map<std::string, std::string> mapped_data;
};

BOOST_FUSION_ADAPT_STRUCT(my_data, name, id, mapped_data);


namespace database { namespace tag {

template<> std::vector<std::string> inline get_tags<my_data>() {  return {"bacnet", "primitive"}; }

}}




int main(int args, char **argc) {


  database::database db(argc[1]);

    my_data d;
    d.name = "Hello broccoli";
    d.id = 42;
    d.mapped_data = {
                      {"1", "Hello"},
                      {"2", "World"},
                      {"3", "trolololololo"},
                      {"33", "trolololololo"},
                      {"333", "trolololololo"},
                      {"3333", "trolololololo"}
                    };

    db.store("my_key_1", d);
    db.store("my_key_2", d);


    auto results = db.find<my_data>("my_key_1");
    for(auto& record : results) {
      std::cout << "record : "       <<  record.name << std::endl;
    }

  return 0;

}
