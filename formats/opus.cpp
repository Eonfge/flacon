/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2013
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


#include "opus.h"
#include "disk.h"
#include "settings.h"
#include <QDebug>

#define CONF_OPUS_BITRATETYPE "Opus/BitrateType"
#define CONF_OPUS_BITRATE "Opus/Bitrate"

/************************************************

 ************************************************/
OutFormat_Opus::OutFormat_Opus()
{
    mId   = "OPUS";
    mExt  = "opus";
    mName = "Opus";
    mSettingsGroup = "Opus";
}


/************************************************

 ************************************************/
QStringList OutFormat_Opus::encoderArgs(const Tags &tags, const QString &outFile) const
{
    QStringList args;

    args << settings->programName(encoderProgramName());

    args << "--quiet";

    QString type = settings->value(CONF_OPUS_BITRATETYPE).toString();
    if (type == "VBR")
        args << "--vbr";

    if (type == "CBR")
        args << "--cvbr";

    args << "--bitrate" << settings->value(CONF_OPUS_BITRATE).toString();

    // Tags .....................................................
    if (!tags.artist().isEmpty())   args << "--artist"  << tags.artist();
    if (!tags.album().isEmpty())    args << "--album"   << tags.album();
    if (!tags.genre().isEmpty())    args << "--genre"   << tags.genre();
    if (!tags.date().isEmpty())     args << "--date"    << tags.date();
    if (!tags.title().isEmpty())    args << "--title"   << tags.title();
    if (!tags.comment().isEmpty())  args << "--comment" << QString("COMMENT=%1").arg(tags.comment());
    if (!tags.diskId().isEmpty())   args << "--comment" << QString("DISCID=%1").arg(tags.diskId());
    args << "--comment" << QString("TRACKNUMBER=%1").arg(tags.trackNum());
    args << "--comment" << QString("TRACKTOTAL=%1").arg(tags.trackCount());

    // Files ....................................................
    args << "-";
    args << outFile;

    return args;
}


/************************************************

 ************************************************/
QStringList OutFormat_Opus::gainArgs(const QStringList &files) const
{
    Q_UNUSED(files);
    return QStringList();
}


/************************************************

 ************************************************/
QHash<QString, QVariant> OutFormat_Opus::defaultParameters() const
{
    QHash<QString, QVariant> res;
    res.insert("Opus/BitrateType",      "VBR");
    res.insert("Opus/Bitrate",          96);
    return res;
}


/************************************************

 ************************************************/
EncoderConfigPage *OutFormat_Opus::configPage(QWidget *parent) const
{
    return new ConfigPage_Opus(parent);
}


/************************************************

 ************************************************/
ConfigPage_Opus::ConfigPage_Opus(QWidget *parent):
    EncoderConfigPage(parent)
{
    setupUi(this);

    opusBitrateTypeCbx->addItem(tr("VBR - variable bitrate"),    "VBR");
    opusBitrateTypeCbx->addItem(tr("CBR - constrained bitrate"), "CBR");

    opusBitrateTypeCbx->setToolTip(toolTipCss() + opusBitrateTypeCbx->toolTip());
    opusBitrateTypeLabel->setToolTip(opusBitrateTypeCbx->toolTip());

    opusBitrateSlider->setToolTip(toolTipCss() + opusBitrateSlider->toolTip());
    opusBitrateSpin->setToolTip(opusBitrateSlider->toolTip());
    opusBitrateLabel->setToolTip(opusBitrateSlider->toolTip());
}


/************************************************

 ************************************************/
void ConfigPage_Opus::load()
{
    loadWidget("Opus/BitrateType",  opusBitrateTypeCbx);
    loadWidget("Opus/Bitrate",      opusBitrateSlider);
}


/************************************************

 ************************************************/
void ConfigPage_Opus::write()
{
    writeWidget("Opus/BitrateType",  opusBitrateTypeCbx);
    writeWidget("Opus/Bitrate",      opusBitrateSlider);
}






