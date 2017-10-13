/****************************************************************************
 *   Copyright (C) 2015-2016 by Savoir-faire Linux                          *
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


class CollectionExtensionModelSpecific final
{
public:
   static QList<CollectionExtensionInterface*>& entries();
   static QList<std::function<void()>>& queuedEntries();
};

template<class T>
int CollectionExtensionModel::registerExtension()
{
   static bool typeInit = false;

   static int typeId = CollectionExtensionModelSpecific::entries().size();

   if (!typeInit) {
      CollectionExtensionModelSpecific::queuedEntries() << []() {
          CollectionExtensionModelSpecific::entries() << new T(nullptr);
      };
   }

   return typeId;
}

template<class T>
int CollectionExtensionModel::getExtensionId()
{
   return registerExtension<T>();
}

template<class T>
T* CollectionExtensionModel::getExtension()
{
   CollectionExtensionModel::instance();
   return static_cast<T*>(CollectionExtensionModelSpecific::entries()[registerExtension<T>()]);
}
