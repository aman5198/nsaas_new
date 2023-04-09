#include "utils/network.hpp"
#include<map>
#include<pair>

map<int,pair<int,InetAddress>> primary_gnb_ue_list;
void addSecGnb(int primary_gnb_id, int secondary_gnb_id, InetAddress & addr){
    pair<int,InetAddress> p;
    p.first = secondary_gnb_id;
    p.second = addr;
    primary_gnb_id[primary_gnb_id] = p;


}