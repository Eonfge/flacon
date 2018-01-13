/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2017
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


#include "flac.h"
#include "settings.h"
#include "project.h"
#include "inputaudiofile.h"

#include <QDebug>

REGISTER_FORMAT(Format_Flac)


/************************************************
 *
 ************************************************/
QStringList Format_Flac::decoderArgs(const QString &fileName) const
{
    QStringList args;
    args << "-c";
    args << "-d";
    args << "-s";
    args << fileName;
    args << "-";

    return args;
}


/************************************************
 *
 ************************************************/
OutFormat_Flac::OutFormat_Flac()
{
    mId   = "FLAC";
    mExt  = "flac";
    mName = "Flac";
    mSettingsGroup = "Flac";
}



/************************************************

 ************************************************/
bool OutFormat_Flac::check(QStringList *errors) const
{
    bool res = OutFormat::check(errors);

    if (gainType() != GainType::Disable)
    {
        for (int i=0; i<project->count(); ++i)
        {
            if (project->disk(i)->audioFile()->sampleRate() > 48000)
            {
                *errors << QObject::tr("you can't use 'ReplayGain' for files with sample rates above 48kHz. Metaflac doesn't support such files.",
                                       "This string should begin with a lowercase letter. This is a part of the complex sentence.");
                res = false;
                break;
            }
        }
    }

    return res;
}


/************************************************

 ************************************************/
QStringList OutFormat_Flac::encoderArgs(const Tags &tags, const QString &outFile) const
{
    QStringList args;

    args << settings->programName(encoderProgramName());
    args << "--force";      // Force overwriting of output files.
    args << "--silent";     // Suppress progress indicator

    // Settings .................................................
    args << QString("--compression-level-%1").arg(settings->value("Flac/Compression").toString());


    // Tags .....................................................
    if (!tags.artist().isEmpty())   args << "--tag" << QString("artist=%1").arg(tags.artist());
    if (!tags.album().isEmpty())    args << "--tag" << QString("album=%1").arg(tags.album());
    if (!tags.genre().isEmpty())    args << "--tag" << QString("genre=%1").arg(tags.genre());
    if (!tags.date().isEmpty())     args << "--tag" << QString("date=%1").arg(tags.date());
    if (!tags.title().isEmpty())    args << "--tag" << QString("title=%1").arg(tags.title());
    if (!tags.comment().isEmpty())  args << "--tag" << QString("comment=%1").arg(tags.comment());
    if (!tags.diskId().isEmpty())   args << "--tag" << QString("discId=%1").arg(tags.diskId());
    args << "--tag" << QString("TRACKNUMBER=%1").arg(tags.trackNum());
    args << "--tag" << QString("TOTALTRACKS=%1").arg(tags.trackCount());
    args << "--tag" << QString("TRACKTOTAL=%1").arg(tags.trackCount());


    args << "-";
    args << "-o" << outFile;
    return args;
}


/************************************************

 ************************************************/
QStringList OutFormat_Flac::gainArgs(const QStringList &files) const
{
    QStringList args;
    args << settings->programName(gainProgramName());
    args << "--add-replay-gain";
    args << files;

    return args;
}


/************************************************

 ************************************************/
QHash<QString, QVariant> OutFormat_Flac::defaultParameters() const
{
    QHash<QString, QVariant> res;
    res.insert("Flac/Compression",  5);
    res.insert("Flac/ReplayGain",   gainTypeToString(GainType::Disable));
    return res;
}


/************************************************

 ************************************************/
EncoderConfigPage *OutFormat_Flac::configPage(QWidget *parent) const
{
    return new ConfigPage_Flac(parent);
}


/************************************************

 ************************************************/
ConfigPage_Flac::ConfigPage_Flac(QWidget *parent):
    EncoderConfigPage(parent)
{
    setupUi(this);

    setLosslessToolTip(flacCompressionSlider);
    flacCompressionSpin->setToolTip(flacCompressionSlider->toolTip());
    fillReplayGainComboBox(flacGainCbx);
}


/************************************************

 ************************************************/
void ConfigPage_Flac::load()
{
    loadWidget("Flac/Compression",  flacCompressionSlider);
    loadWidget("Flac/ReplayGain", flacGainCbx);
}


/************************************************

 ************************************************/
void ConfigPage_Flac::write()
{
    writeWidget("Flac/Compression",  flacCompressionSlider);
    writeWidget("Flac/ReplayGain", flacGainCbx);
}





