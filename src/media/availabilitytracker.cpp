/****************************************************************************
 *   Copyright (C) 2018 by Bluesystems                                      *
 *   Author : Emmanuel Lepage Vallee <elv1313@gmail.com>                    *
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
#include "availabilitytracker.h"

// ring
#include "contactmethod.h"
#include "accountmodel.h"
#include "individual.h"
#include "libcard/matrixutils.h"

// libstdc++
#include <algorithm>

namespace Media {

class AvailabilityTrackerPrivate : public QObject
{
    Q_OBJECT
public:
    QWeakPointer<Individual> m_pIndividual;

    // Helpers
    ContactMethod::MediaAvailailityStatus getFirstIssue() const;

    static const Matrix1D<ContactMethod::MediaAvailailityStatus ,QString> m_mCMMASNames;

    AvailabilityTracker* q_ptr;
public Q_SLOTS:
    void slotCanCallChanged();
    void slotRegistrationChanged();
    void slotCanVideoCallChanged();
    void slotIndividualChanged();
};

const Matrix1D<ContactMethod::MediaAvailailityStatus ,QString> AvailabilityTrackerPrivate::m_mCMMASNames = {{
    { ContactMethod::MediaAvailailityStatus::NO_CALL     , QObject::tr("Sending text messages can only happen during an audio call in SIP accounts")},
    { ContactMethod::MediaAvailailityStatus::UNSUPPORTED , QObject::tr("This account doesn't support all media")},
    { ContactMethod::MediaAvailailityStatus::SETTINGS    , QObject::tr("Video isn't available because it's disabled for this account")},
    { ContactMethod::MediaAvailailityStatus::NO_ACCOUNT  , QObject::tr("There is no account capable of reaching this person")},
    { ContactMethod::MediaAvailailityStatus::CODECS      , QObject::tr("All video codecs are disabled, video call isn't possible")},
    { ContactMethod::MediaAvailailityStatus::ACCOUNT_DOWN, QObject::tr("All accounts capable of reaching this person are currently unavailable")},
    { ContactMethod::MediaAvailailityStatus::NETWORK     , QObject::tr("Ring-KDE is experiencing a network issue, please try later")},
    { ContactMethod::MediaAvailailityStatus::AVAILABLE   , QObject::tr("")}
}};

AvailabilityTracker::AvailabilityTracker(QObject* parent) : QObject(parent),
    d_ptr(new AvailabilityTrackerPrivate())
{
    d_ptr->q_ptr = this;
    connect(&AccountModel::instance(), &AccountModel::canCallChanged, d_ptr, &AvailabilityTrackerPrivate::slotCanCallChanged);
    connect(&AccountModel::instance(), &AccountModel::registrationChanged, d_ptr, &AvailabilityTrackerPrivate::slotRegistrationChanged);
    connect(&AccountModel::instance(), &AccountModel::canVideoCallChanged, d_ptr, &AvailabilityTrackerPrivate::slotCanVideoCallChanged);
}

AvailabilityTracker::~AvailabilityTracker()
{
    delete d_ptr;
}

bool AvailabilityTracker::canCall() const
{
    const QSharedPointer<Individual> i = d_ptr->m_pIndividual;
    return i ? i->canCall() : false;
}

bool AvailabilityTracker::canVideoCall() const
{
    const QSharedPointer<Individual> i = d_ptr->m_pIndividual;
    return i ? i->canVideoCall() : false;
}

bool AvailabilityTracker::canSendTexts() const
{
    const QSharedPointer<Individual> i = d_ptr->m_pIndividual;
    return i ? i->canSendTexts() : false;
}

bool AvailabilityTracker::hasWarning() const
{
    const QSharedPointer<Individual> i = d_ptr->m_pIndividual;
    return !(
        (i) && canCall() && canVideoCall() && canSendTexts() && !i->isOffline()
    );
}

ContactMethod::MediaAvailailityStatus AvailabilityTrackerPrivate::getFirstIssue() const
{
    if (!m_pIndividual)
        return ContactMethod::MediaAvailailityStatus::AVAILABLE;

    auto ret = ContactMethod::MediaAvailailityStatus::AVAILABLE;

    const QSharedPointer<Individual> i = m_pIndividual;

//     bool hasFineCM = false;

    // The order of the issue type is related to the severity
    i->forAllNumbers([&ret/*, &hasFineCM*/](ContactMethod* cm) {
        auto textsIssue = cm->canSendTexts();
        auto videoIssue = cm->canVideoCall();
        auto audioIssue = cm->canCall();

        ret = (ContactMethod::MediaAvailailityStatus) std::max(
            (int)textsIssue, std::max( (int)videoIssue, (int)audioIssue)
        );

//         if (newRet != ContactMethod::MediaAvailailityStatus::AVAILABLE)
//             hasFineCM = true;

        Q_ASSERT(ret != ContactMethod::MediaAvailailityStatus::COUNT__);
    }, false);

    return ret;
}

QString AvailabilityTracker::warningMessage() const
{
    if (!hasWarning())
        return {};

    const auto ma = d_ptr->getFirstIssue();

    const QSharedPointer<Individual> i = d_ptr->m_pIndividual;

    if (!i)
        return tr("No contact has been selected");

    if (ma == ContactMethod::MediaAvailailityStatus::AVAILABLE && i->isOffline())
        return tr("This contact doesn't appears to be online, stealth mode may be enabled.");

    return d_ptr->m_mCMMASNames[d_ptr->getFirstIssue()];
}

QSharedPointer<Individual> AvailabilityTracker::individual() const
{
    return d_ptr->m_pIndividual;
}

void AvailabilityTracker::setIndividual(const QSharedPointer<Individual>& ind)
{
    if (d_ptr->m_pIndividual) {
        const QSharedPointer<Individual> i = d_ptr->m_pIndividual;
        disconnect(i.data(), &Individual::mediaAvailabilityChanged,
            d_ptr, &AvailabilityTrackerPrivate::slotIndividualChanged);
    }

    d_ptr->m_pIndividual = ind;

    if (ind)
        connect(ind.data(), &Individual::mediaAvailabilityChanged,
            d_ptr, &AvailabilityTrackerPrivate::slotIndividualChanged);

    d_ptr->slotIndividualChanged();
}

Individual* AvailabilityTracker::rawIndividual() const
{
    return d_ptr->m_pIndividual ? d_ptr->m_pIndividual.data() : nullptr;
}

void AvailabilityTracker::setRawIndividual(Individual* ind)
{
    setIndividual(Individual::getIndividual(ind));
}

void AvailabilityTrackerPrivate::slotCanCallChanged()
{
    emit q_ptr->changed();
}

void AvailabilityTrackerPrivate::slotRegistrationChanged()
{
    emit q_ptr->changed();
}

void AvailabilityTrackerPrivate::slotCanVideoCallChanged()
{
    emit q_ptr->changed();
}

void AvailabilityTrackerPrivate::slotIndividualChanged()
{
    emit q_ptr->changed();
}

}

#include <availabilitytracker.moc>
