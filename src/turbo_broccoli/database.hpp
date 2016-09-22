/*
 * database.hpp
 *
 *  Created on: Sep 22, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_DATABASE_HPP_
#define SRC_TURBO_BROCCOLI_DATABASE_HPP_

#include <boost/filesystem.hpp>
#include <boost/uuid/sha1.hpp>
#include <nlohmann/json.hpp>
#include <pre/bytes/utils.hpp>

namespace trubo_broccoli {

using key_t        = std::array<uint8_t, 20>;
using result_value = std::pair<bool, nlohmann::json>;
using result_key   = std::pair<bool, key_t>;
namespace fs = boost::filesystem;

struct database {


  database(const std::string& path) : path_(path) {

  }


  result_value find(const key_t& key) {


    static const result_value failed_result{false, nlohmann::json{}};

    if(!fs::exists(path_)) {
      return failed_result;
    }

    auto filename = to_filename(key);

    if(!fs::exists(filename)) {
      std::cerr << "no file with " << filename << std::endl;
      return failed_result;
    }

    try {
      nlohmann::json json;
      std::ifstream ifs(filename.generic_string(), std::ifstream::in);
      ifs.exceptions( std::ifstream::badbit );
      ifs >> json;
      ifs.close();
      return {true, json};
    }
    catch(...) {
      std::cerr << "cannot parse file  " << filename << std::endl;
    }

    return failed_result;
  }


  result_key store(const nlohmann::json& value) {
    static const result_key failed_result{false, nil_key()};
    auto binary_data = value.dump(2);
    auto key = calculate_key(binary_data);
    if( fs::exists(to_filename(key) ) ) {
      return {true, key};
    }
    else {
      try {
        create_folder(key);
        std::ofstream ofs(to_filename(key).generic_string(), std::fstream::out | std::fstream::trunc);
        ofs.exceptions ( std::ifstream::badbit );
        ofs << binary_data << std::endl;
        ofs.flush();
        ofs.close();
        return {true, key};
      }
      catch(...) {

      }
    }

    return failed_result;
  }



private:

  void create_folder(const key_t& key) {
    fs::create_directories(path_ / fs::path(pre::bytes::to_hexstring(key.data(), 2)));
  }

  fs::path to_filename(const key_t &k) {
    return path_ / fs::path(pre::bytes::to_hexstring(k.data(), 2)) / fs::path(pre::bytes::to_hexstring(k.data() + 2 , 14));
  }

  key_t nil_key() {
    static const key_t nil{};
    return nil;
  }


  key_t calculate_key(const std::string data) {
    try {

     boost::uuids::detail::sha1 hasher{};
     for(auto &c : data ) {
       hasher.process_byte(c);
     }


     unsigned int hash[5];
     hasher.get_digest(hash);
     return to_key(hash);
    }
    catch(...) {
      return nil_key();
    }
  }

  key_t to_key(const unsigned int hash[5]) {
    auto h = reinterpret_cast<const uint8_t*>(hash);
    key_t k;
    for(int i = 0; i < 5; ++i)  {
      k[i*4]  = h[i*4+3];
      k[i*4+1] = h[i*4+2];
      k[i*4+2] = h[i*4+1];
      k[i*4+3] = h[i*4];
    }
    return k;
  }


  using path_t = boost::filesystem::path;
  path_t path_;

};


}




#endif /* SRC_TURBO_BROCCOLI_DATABASE_HPP_ */
