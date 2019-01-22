#include <stdio.h>
#include <stdint.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pcap.h>

#include <map>

#include "MacAddr.h"
#include "packet.h"
#include "dot11.h"
#include "util.h"
#include "my_radiotap.h"


using namespace wlan;


void usage(char *fname) {
  printf("syntax: %s <interface>\n", fname);
  printf("sample: %s mon0\n", fname);
}

// std::map<MacAddr, 

int main(int argc, char* argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    return -1;
  }

  char* dev = argv[1];
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "couldn't open device %s: %s\n", dev, errbuf);
    return -1;
  }

  while (true) {
    struct pcap_pkthdr* header;
    const u_char* packet;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == 0) continue;
    if (res == -1 || res == -2) break;
    // printf("%ld   : %u bytes captured\n", header->ts.tv_sec, header->caplen);

    RadiotapHeader* radiotap = (RadiotapHeader*)packet;
    // printf("radiotap length [%hu]\n", radiotap->length);
    Dot11FrameControl* fc = (Dot11FrameControl*)(packet + radiotap->length);
    switch (fc->getTypeSubtype()) {
      case Dot11FC::TypeSubtype::BEACON:
      {
        printf("%d\n", *(uint16_t*)radiotap->getField(PresentFlag::CHANNEL));
        Dot11BeaconFrame* beacon_frame = (Dot11BeaconFrame*)fc;
        beacon_frame->bssid.print();
        // if 새로운 BSSID면 map에 추가 아예 new로 하나 할당해서 잡아버리는게 좋겠군.
        // 기존에 있는거면 count ++
        
        // 일단 airodump에서 사용하는 column들에 대한 파싱을 완료해서 구조체로 정리하고, map<MacAddr, 구조체>이렇게 하면 될듯.
        
        break;
      }
      default:
        printf("this is not beacon\n");
        break;
    }
  }

  pcap_close(handle);
  return 0;
}


// 암호화는 dot11 header에서 flag protected ==1 이고 ccmpㅣ면wpa
// WEP는 radiotap header의 flag에 항목이 있는 것 같다.
// power는 그냥 radiotap에서 가져오면 되는 것 같고
// RXQ는 sequence number를 봐서 비율을 따지면 되겠고.

