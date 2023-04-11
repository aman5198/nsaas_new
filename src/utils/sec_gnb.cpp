#include "utils/network.hpp"
#include<map>
#include<pair>
#include<iostream>
#include<string>

map<int,pair<int,pair<std::string,std::string>>> primary_gnb_ue_list;
void addSecGnb(int primary_gnb_id, int secondary_gnb_id, std::string sec_ip, std::string sec_port){
    pair<std::string,std::string> sec_ip_and_port;
    sec_ip_and_port.first = sec_ip;
    sec_ip_and_port.second = sec_port;

    pair<int,pair<std::string,std::string>> sec_data;
    sec_data.first = secondary_gnb_id;
    sec_data.second = sec_ip_and_port;

    primary_gnb_ue_list[primary_gnb_id] = sec_data;
    
}