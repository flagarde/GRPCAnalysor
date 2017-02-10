#include "db/DBInit.h"
#include <stdlib.h>
#define CONFDB  "CONFDB"

void DBInit::init()
{
  DBConnection *db= DBConnection::getConnection();
  std::string login="";
  std::string passwd="";
  std::string path="";
  getDbConfiguration(login, passwd, path);
  db->setUser(login);
  db->setPassword(passwd);
  db->setDatabase(path);  
  db->connect();
}

void DBInit::terminate()
{
    DBConnection* db = DBConnection::getConnection();
    db->disconnect();
    delete(db);
    db=nullptr;
    ConfigObject::terminate();
}

void DBInit::getDbConfiguration(std::string &login, std::string &passwd, std::string &path) 
{
  char* cpath=getenv(CONFDB);
  if (cpath == nullptr) return ;  
  std::string confdb(cpath);
  size_t ipass = confdb.find("/");
  size_t ipath = confdb.find("@");
  if ((ipass != string::npos) && (ipath != string::npos)) 
  {
    login.clear();
    login=confdb.substr(0,ipass); 
    passwd.clear();
    passwd=confdb.substr(ipass+1,ipath-ipass-1); 
    path.clear();
    path=confdb.substr(ipath+1,confdb.size());
  }
  std::cout<<login<<"  "<<passwd<<"  "<<path<<std::endl;
}
