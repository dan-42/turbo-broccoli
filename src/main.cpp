
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

  namespace tb = turbo_broccoli;
  using blob = turbo_broccoli::blob;

  tb::database db(argc[1]);

  {
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

    blob data("my_key");
    data.add_tag("primitive");
    data.add_tag("slc-device");
    data.put(d);

    blob data2("foobar");
    data2.add_tag("primitive");
    data2.put(d);

    db.store(data);
    db.store(data2);

  }

  {
    auto result = db.find("my_key");
    if(result.success) {
      for(auto& record : result.results) {
        std::cout << "key : "       <<  record.key() << std::endl;
        std::cout << "key_hash : "  <<  record.key_hash() << std::endl;
        std::cout << "tags : "      <<  record.tags() << std::endl;
        std::cout << "version : "   <<  record.version() << std::endl;
        std::cout << "data : "      <<  record.data() << std::endl;
      }
    }
  }

  std::cout << std::endl << "----------------"  << std::endl;


  {
     auto result = db.find("primitive");
     if(result.success) {
       for(auto& record : result.results) {
         std::cout << "key : "       <<  record.key() << std::endl;
         std::cout << "key_hash : "  <<  record.key_hash() << std::endl;
         std::cout << "tags : "      <<  record.tags() << std::endl;
         std::cout << "version : "   <<  record.version() << std::endl;
         std::cout << "data : "      <<  record.data() << std::endl;
       }
     }
   }

  std::cout << std::endl << "----------------"  << std::endl;
  std::cout << std::endl << "----------------"  << std::endl;

  auto result = db.find("my_key");
  if(result.success) {
    auto record = result.results.front();
    record.remove_tag("primitive");
    db.store(record);
  }

  {
       auto result = db.find("primitive");
       if(result.success) {
         for(auto& record : result.results) {
           std::cout << "key : "       <<  record.key() << std::endl;
           std::cout << "key_hash : "  <<  record.key_hash() << std::endl;
           std::cout << "tags : "      <<  record.tags() << std::endl;
           std::cout << "version : "   <<  record.version() << std::endl;
           std::cout << "data : "      <<  record.data() << std::endl;
         }
       }
     }


  /*
  auto result_remove = db.remove("primitive");
  if(result_remove) {
    std::cout << "deleted " << remove_result.number();
  }
*/
  return 0;

}
