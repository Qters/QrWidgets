#ifndef CONTACESDATA_H
#define CONTACESDATA_H

#include <QtCore/qstring.h>

class ContactData
{
public:
    bool isVip = false;
    int level = 0;
    int uid = 0;
    int groupid = 0;
    QString name;
    QString groupName;
};

#endif // CONTACESDATA_H
