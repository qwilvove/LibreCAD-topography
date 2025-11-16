/*****************************************************************************/
/*  topographytools.cpp - plugin for LibreCAD                                */
/*                                                                           */
/*  Copyright (C) 2024 qwilvove, qwilvove@protonmail.com                     */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "topographytools.h"

#include "ui/dialog/main.h"

QString LC_Topographytools::name() const
{
    return (tr("Topography tools"));
}

PluginCapabilities LC_Topographytools::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints << PluginMenuLocation("plugins_menu", tr("Topography tools"));
    return pluginCapabilities;
}

void LC_Topographytools::execComm(Document_Interface *doc, QWidget *parent, QString cmd)
{
    Q_UNUSED(cmd);

    TT_DialogMain mainDialog(parent, doc);
    while (mainDialog.isRunning)
    {
        mainDialog.exec();
    }
}
