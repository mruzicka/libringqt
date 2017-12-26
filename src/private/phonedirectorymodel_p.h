/****************************************************************************
 *   Copyright (C) 2013-2016 by Savoir-faire Linux                          *
 *   Author : Emmanuel Lepage Vallee <emmanuel.lepage@savoirfairelinux.com> *
 *                                                                          *
 *   This library is free software; you can redistribute it and/or          *
 *   modify it under the terms of the GNU Lesser General Public             *
 *   License as published by the Free Software Foundation; either           *
 *   version 2.1 of the License, or (at your option) any later version.     *
 *                                                                          *
 *   This library is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 *   Lesser General Public License for more details.                        *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/
#pragma once

#include <QtCore/QObject>

//Ring
class PhoneDirectoryModel;
class LocalNameServiceCache;
#include "contactmethod.h"
#include "namedirectory.h"

//Internal data structures
///@struct NumberWrapper Wrap phone numbers to prevent collisions
struct NumberWrapper final {
   explicit NumberWrapper(const QString& k) : key(k) {}

   QString key;
   QVector<ContactMethod*> numbers;
};

class MostPopularNumberModel final : public QAbstractListModel
{
   Q_OBJECT
public:
   explicit MostPopularNumberModel();

   //Model functions
   virtual QVariant      data     ( const QModelIndex& index, int role = Qt::DisplayRole     ) const override;
   virtual int           rowCount ( const QModelIndex& parent = QModelIndex()                ) const override;
   virtual Qt::ItemFlags flags    ( const QModelIndex& index                                 ) const override;
   virtual bool          setData  ( const QModelIndex& index, const QVariant &value, int role)       override;

   void addRow();
   void reload();
};

class PhoneDirectoryModelPrivate final : public QObject
{
   Q_OBJECT
public:
   explicit PhoneDirectoryModelPrivate(PhoneDirectoryModel* parent);


   //Model columns
   enum class Columns {
      URI              = 0,
      TYPE             = 1,
      CONTACT          = 2,
      IS_SELF          = 3,
      ACCOUNT          = 4,
      STATE            = 5,
      CALL_COUNT       = 6,
      WEEK_COUNT       = 7,
      TRIM_COUNT       = 8,
      HAVE_CALLED      = 9,
      LAST_USED        = 10,
      NAME_COUNT       = 11,
      TOTAL_SECONDS    = 12,
      POPULARITY_INDEX = 13,
      BOOKMARED        = 14,
      TRACKED          = 15,
      HAS_CERTIFICATE  = 16,
      PRESENT          = 17,
      PRESENCE_MESSAGE = 18,
      UID              = 19,
      REGISTERED_NAME  = 20,
   };


   //Helpers
   void indexNumber(ContactMethod* number, const QStringList& names   );
   void setAccount (ContactMethod* number,       Account*     account );
   ContactMethod* fillDetails(NumberWrapper* wrap, const URI& strippedUri, Account* account, Person* contact, const QString& type);
   void registerAlternateNames(ContactMethod* number, Account* account, const URI& uri, const URI& extendedUri);

   //Attributes
   QVector<ContactMethod*>         m_lNumbers         ;
   QHash<QString,NumberWrapper*> m_hDirectory       ;
   QVector<ContactMethod*>         m_lPopularityIndex ;
   QMap<QString,NumberWrapper*>  m_lSortedNames     ;
   QMap<QString,NumberWrapper*>  m_hSortedNumbers   ;
   QHash<QString,NumberWrapper*> m_hNumbersByNames  ;
   bool                          m_CallWithAccount  ;
   MostPopularNumberModel*       m_pPopularModel    ;
   LocalNameServiceCache*        m_pNameServiceCache;

   Q_DECLARE_PUBLIC(PhoneDirectoryModel)

private:
   PhoneDirectoryModel* q_ptr;

private Q_SLOTS:
   void slotCallAdded(Call* call);
   void slotChanged();
   void slotLastUsedChanged(time_t t);
   void slotContactChanged(Person* newContact, Person* oldContact);
   void slotRegisteredNameFound(Account* account, NameDirectory::LookupStatus status, const QString& address, const QString& name);
   void slotContactMethodMerged(ContactMethod* other);

   //From DBus
   void slotNewBuddySubscription(const QString& uri, const QString& accountId, bool status, const QString& message);
};
