#pragma once
#include<vector>
#include<map>
using namespace std;
class Edge;
class Item;
class User;
class Location;

typedef map<int ,Edge * > EdgeMap;
typedef map<int,User*> UserMap;
typedef map<int,Location*> LocMap;

//ȫ�ֱ��������ڴ洢���е��û������е�λ�õĽڵ�ָ��


User* getUserPtrbyId(int id);
Location* getLocationPtrbyId(int id);
