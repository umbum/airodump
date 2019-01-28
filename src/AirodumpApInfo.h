#ifndef AIRODUMPLINE_H
#define AIRODUMPLINE_H

#include <iomanip>
#include <iostream>

#include <cstdint>
#include <string>

#include "MacAddr.h"


namespace wlan {
const uint16_t STD_OPN = 0x0001;
const uint16_t STD_WEP = 0x0002;
const uint16_t STD_WPA = 0x0004;
const uint16_t STD_WPA2 = 0x0008;

const uint16_t ENC_WEP = 0x0010;
const uint16_t ENC_TKIP = 0x0020;
const uint16_t ENC_WRAP = 0x0040;
const uint16_t ENC_CCMP = 0x0080;
const uint16_t ENC_WEP40 = 0x1000;
const uint16_t ENC_WEP104 = 0x0100;
const uint16_t ENC_GCMP = 0x4000;

const uint16_t AUTH_OPN = 0x0200;
const uint16_t AUTH_PSK = 0x0400;
const uint16_t AUTH_MGT = 0x0800;



class AirodumpApInfo {
public:
  MacAddr bssid;
  int          pwr = 0;
  unsigned int beacons = 0;
  unsigned int num_data = 0;
  unsigned int num_data_per_sec = 0;
  unsigned int channel = 0;
  unsigned int max_speed = 0;
  char         qos = ' ';
  char         preamble = ' ';
  unsigned int enc;
  unsigned int cipher;
  unsigned int auth;
  std::string essid;

public:
  AirodumpApInfo() {}
  AirodumpApInfo(MacAddr _bssid): bssid(_bssid) {}

  void parseTaggedParam(uint8_t* it, const uint8_t* packet_end);

  friend std::ostream& operator<<(std::ostream& os, AirodumpApInfo& obj) {
    std::string enc_str;
    if (obj.enc & STD_WPA2)
      enc_str = "WPA2";
    else if (obj.enc & STD_WPA)
      enc_str = "WPA ";
    else if (obj.enc & STD_WEP)
      enc_str = "WEP ";
    else if (obj.enc & STD_OPN)
      enc_str = "OPN ";
    else
      enc_str = "    ";
    
    std::string cipher_str;
    if (obj.cipher & ENC_CCMP)
      cipher_str = "CCMP   ";
    else if (obj.cipher & ENC_WRAP)
      cipher_str = "WRAP   ";
    else if (obj.cipher & ENC_TKIP)
      cipher_str = "TKIP   ";
    else if (obj.cipher & ENC_WEP104)
      cipher_str = "WEP104 ";
    else if (obj.cipher & ENC_WEP40)
      cipher_str = "WEP40  ";
    else if (obj.cipher & ENC_WEP)
      cipher_str = "WEP    ";
    else if (obj.cipher & ENC_GCMP)
      cipher_str = "GCMP   ";
    else
      cipher_str = "       ";
    
    std::string auth_str;
    if (obj.auth & AUTH_MGT)
      auth_str = "MGT";
    else if (obj.auth & AUTH_PSK) {
      if (obj.enc & STD_WEP)
        auth_str = "SKA";
      else
        auth_str = "PSK";
    }
    else if (obj.auth & AUTH_OPN)
      auth_str = "OPN";
    else
      auth_str = "   ";
      

    return os << " " << obj.bssid << 
                 std::setw(5) << obj.pwr << " " << 
                 std::setw(8) << obj.beacons << " " << 
                 std::setw(8) << obj.num_data << " " <<
                 std::setw(4) << obj.num_data_per_sec << " " <<
                 std::setw(3) << obj.channel << "  " <<
                 obj.max_speed << obj.qos << obj.preamble << " " <<
                 enc_str << " " <<
                 cipher_str <<
                 auth_str << "  " <<
                 obj.essid;
    }

};

};
#endif


/***   enum은 |= 연산자를 오버로딩 해줘야 해서 그냥 안썼다.
namespace ENC {
enum T {
  OPN = 0x0001,
  WEP = 0x0002,
  WPA = 0x0004,
  WPA2 = 0x0008
};
};

namespace CIPHER {
enum T {
  WEP = 0x0010,
  TKIP = 0x0020,
  WRAP = 0x0040,
  CCMP = 0x0080,
  WEP40 = 0x1000,
  WEP104 = 0x0100,
  GCMP = 0x4000
};
};

namespace AUTH {
enum T {
  OPN = 0x0200,
  PSK = 0x0400,
  MGT = 0x0800
};
};
*/