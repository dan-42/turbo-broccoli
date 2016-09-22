
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <functional>
#include <thread>
#include <cassert>

#include <boost/fusion/include/adapt_struct.hpp>
#include <pre/json/to_json.hpp>
#include <turbo_broccoli/database.hpp>


struct my_data {
  std::string name;
  uint32_t  id;
  std::map<std::string, std::string> mapped_data;
};


BOOST_FUSION_ADAPT_STRUCT(my_data, name, id, mapped_data);

int main(int args, char **argc) {


  trubo_broccoli::database db(argc[1]);

  my_data d;
  d.name = "Hello broccoli";
  d.id = 42;
  d.mapped_data = { {"1", "Hello"},
                    {"2", "World"},
                   {"3", "trolololololo"}
                  };

  auto key = db.store(pre::json::to_json(d));

  std::cout << pre::bytes::to_hexstring(key.second.data(), key.second.size()) << std::endl;


  auto dd = db.find(key.second);

  std::cout << dd.second << std::endl;

  return 0;
}
