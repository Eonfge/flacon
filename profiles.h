/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2019-2020
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef PROFILES_H
#define PROFILES_H

#include <QString>
#include <QVariant>
#include <QVector>

class OutFormat;
class QSettings;

class Profile
{
    friend QDebug operator<<(QDebug dbg, const Profile &profile);
public:
    Profile();
    explicit Profile(const QString &id);
    Profile(const Profile &other);
    Profile &operator =(const Profile &other);

    QString id() const { return mId; }

    QString name() const { return mName; }
    void setName(const QString &value);

    QString formatId() const { return mFormatId; }
    void setFormatId(QString formatId);

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);

    OutFormat *format() const;
    bool isValid() const noexcept;

    void load(QSettings &settings, const QString &group);
    void save(QSettings &settings, const QString &group) const;

private:
    QString mId;
    QString mFormatId;
    QString mName;
    QHash<QString, QVariant> mValues;

};


class Profiles: public QVector<Profile>
{
public:
//    inline Profiles() Q_DECL_NOTHROW :    QVector<Profile>() { }
//    explicit Profiles(int size):          QVector<Profile>(size) {}
//    Profiles(int size, const Profile &t): QVector<Profile>(size, t) {}
//    inline Profiles(const Profiles &v):   QVector<Profile>(v) {}
//    const Profile &find(const QString id) const;
//          Profile &find(const QString id);

    //int indexOf(const Profile &p, int from = 0) const;
    int indexOf(const QString &id, int from = 0) const;
};

QDebug operator<<(QDebug dbg, const Profile &profile);
QDebug operator<<(QDebug dbg, const Profiles &profiles);

#endif // PROFILES_H