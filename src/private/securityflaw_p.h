/****************************************************************************
 *   Copyright (C) 2015 by Savoir-Faire Linux                               *
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
#ifndef SECURITYFLAWPRIVATE_H
#define SECURITYFLAWPRIVATE_H

#include <QtCore/QObject>

#include "securityvalidationmodel.h"
class SecurityFlaw;

class SecurityFlawPrivate : public QObject
{
   Q_OBJECT
public:
   SecurityFlawPrivate(SecurityFlaw* parent, SecurityValidationModel::AccountSecurityFlaw f,Certificate::Type type);

   //Attributes
   SecurityValidationModel::AccountSecurityFlaw m_flaw;
   SecurityValidationModel::Severity m_severity;
   Certificate::Type m_certType;
   int m_Row;

   SecurityFlaw* q_ptr;
};

#endif