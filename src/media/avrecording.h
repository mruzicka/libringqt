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

#include <media/recording.h>
#include <media/attachment.h>

#include <QtCore/QUrl>

namespace Media {

class AVRecordingPrivate;

class LIB_EXPORT AVRecording : public Recording, public Attachment
{
   Q_OBJECT

   friend class ::RecordingPlaybackManager;
public:
   //Types
   typedef double Position;

   //Properties
   Q_PROPERTY( QUrl    path                 READ path                 CONSTANT                          )
   Q_PROPERTY( double  position             READ position             NOTIFY playbackPositionChanged    )
   Q_PROPERTY( int     duration             READ duration             CONSTANT                          )
   Q_PROPERTY( QString formattedTimeElapsed READ formattedTimeElapsed NOTIFY formattedTimeElapsedChanged)
   Q_PROPERTY( QString formattedDuration    READ formattedDuration    NOTIFY formattedDurationChanged   )
   Q_PROPERTY( QString formattedTimeLeft    READ formattedTimeLeft    NOTIFY formattedTimeLeftChanged   )
   Q_PROPERTY( bool    isCurrent            READ isCurrent            NOTIFY currentStatusChanged       )
   Q_PROPERTY( bool    isPlaying            READ isPlaying            NOTIFY playingStatusChanged       )

   //Constructor
   explicit AVRecording(const Recording::Status status);
   virtual ~AVRecording();

   //Getter
   double     position            () const;
   int        duration            () const;
   QString    formattedTimeElapsed() const;
   QString    formattedDuration   () const;
   QString    formattedTimeLeft   () const;
   bool       isCurrent           () const;
   bool       isPlaying           () const;

   // Attachment properties
   virtual QUrl path          () const override;
   virtual QMimeType* mimeType() const override;
   virtual BuiltInTypes type  () const override;

   virtual QVariant roleData(int role) const override;

   //Setter
   void setPath(const QUrl& path);

private:
   AVRecordingPrivate* d_ptr;
   Q_DECLARE_PRIVATE(AVRecording)

public Q_SLOTS:
   void play (            );
   void stop (            );
   void pause(            );
   void seek ( double pos );
   void reset(            );

Q_SIGNALS:
   /**
    * The recording playback position changed
    * @args pos The position, in percent
    */
   void playbackPositionChanged(double pos);
   ///The recording playback has stopped
   void stopped();
   ///The recording playback has started
   void started();
   ///Emitted when the formatted elapsed time string change
   void formattedTimeElapsedChanged(const QString& formattedValue);
   ///Emitted when the formatted duration string change
   void formattedDurationChanged   (const QString& formattedValue);
   ///Emitted when the formatted time left string change
   void formattedTimeLeftChanged   (const QString& formattedValue);
   ///When this recording is selected as the current one
   void currentStatusChanged(bool);
   ///When this recording is playing
   void playingStatusChanged(bool);
};

}
