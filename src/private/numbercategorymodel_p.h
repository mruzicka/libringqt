/****************************************************************************
 *   Copyright (C) 2015-2016 by Savoir-faire Linux                               *
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

class NumberCategoryModelPrivate final
{
public:
   struct InternalTypeRepresentation final {
      NumberCategory* category { nullptr };
      int             index    {   -1    };
      bool            enabled  {   true  };
      int             counter  {    0    };
   };

   //Attributes
   QVector<InternalTypeRepresentation*>                     m_lCategories ;
   QHash<int,InternalTypeRepresentation*>                   m_hByIdx      ;
   QHash<QString,InternalTypeRepresentation*>               m_hByName     ;
   QHash<const NumberCategory*,InternalTypeRepresentation*> m_hToInternal ;

   //Mutator
   void registerNumber  ( ContactMethod* number     ); //FIXME this should be private
   void unregisterNumber( ContactMethod* number     );
   int  getSize         ( const NumberCategory* cat ) const;
};

