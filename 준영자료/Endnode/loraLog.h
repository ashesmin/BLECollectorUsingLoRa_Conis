#include "loraPacket.h"

string loraJSONString(MyPacket *);
string replaceString(string, const string &search, const string &replace);
bool writeToFile(const char *, const char *, int);