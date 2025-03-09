/*****************************************************************************/
/*  topographytools.h - plugin for LibreCAD                                  */
/*                                                                           */
/*  Copyright (C) 2024 qwilvove, qwilvove@protonmail.com                     */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef TOPOGRAPHYTOOLS_H
#define TOPOGRAPHYTOOLS_H

#include "qc_plugininterface.h"
#include "document_interface.h"

/****** LC_Topographytools ******/
class LC_Topographytools : public QObject, QC_PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(QC_PluginInterface)
    Q_PLUGIN_METADATA(IID LC_DocumentInterface_iid FILE  "topographytools.json")

 public:
    virtual PluginCapabilities getCapabilities() const Q_DECL_OVERRIDE;
    virtual QString name() const Q_DECL_OVERRIDE;
    virtual void execComm(Document_Interface *doc,
                          QWidget *parent, QString cmd) Q_DECL_OVERRIDE;
};

#endif // TOPOGRAPHYTOOLS_H
