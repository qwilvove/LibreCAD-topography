/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2011 Rallaz (rallazz@gmail.com)
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
**
**
** This file is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/
#include "qc_actiongetselect.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include "doc_plugin_interface.h"
#include "rs_actionselectsingle.h"
#include "rs_graphicview.h"
#include "rs_snapper.h"

QC_ActionGetSelect::QC_ActionGetSelect(RS_EntityContainer& container,
                                 RS_GraphicView& graphicView)
    :RS_ActionInterface("Get Select", container, graphicView)
    , completed(false)
    , message(std::make_unique<QString>(tr("Select objects:"))){
    actionType = RS2::ActionGetSelect;
}

QC_ActionGetSelect::QC_ActionGetSelect(RS2::EntityType typeToSelect, RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
    :RS_ActionInterface("Get Select", container, graphicView)
    , completed(false)
    , message(std::make_unique<QString>(tr("Select objects:"))),
     typeToSelect(typeToSelect){
    actionType = RS2::ActionGetSelect;
}

QC_ActionGetSelect::~QC_ActionGetSelect() = default;

void QC_ActionGetSelect::updateMouseButtonHints() {
    switch (getStatus()) {
        case Select:
            updateMouseWidget(*message, tr("Cancel"));
            break;
        default:
            updateMouseWidget();
            break;
    }
}

RS2::CursorType QC_ActionGetSelect::doGetMouseCursor([[maybe_unused]] int status){
    return RS2::SelectCursor;
}

void QC_ActionGetSelect::setMessage(QString msg){
    *message = std::move(msg);
}

void QC_ActionGetSelect::init(int status) {
        RS_ActionInterface::init(status);
        graphicView->setCurrentAction(
                new RS_ActionSelectSingle(typeToSelect, *container, *graphicView, this));
}

void QC_ActionGetSelect::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()==Qt::RightButton) {
        completed = true;
        updateMouseWidget();
        finish();
    }
}

void QC_ActionGetSelect::keyPressEvent(QKeyEvent* e){
    if (e->key()==Qt::Key_Escape || e->key()==Qt::Key_Enter){
        updateMouseWidget();
        finish();
        completed = true;
    }
}

/**
 * Adds all selected entities from 'container' to the selection.
 */
void QC_ActionGetSelect::getSelected(QList<Plug_Entity *> *se, Doc_plugin_interface *d) const{
    for (auto e: *container) {
        if (e->isSelected()) {
            Plugin_Entity *pe = new Plugin_Entity(e, d);
            se->append(reinterpret_cast<Plug_Entity *>(pe));
        }
    }
}

void QC_ActionGetSelect::unselectEntities(){
    for(auto e: *container){ // fixme - iterating all entities for selection
        if (e->isSelected()) {
            e->setSelected(false);
        }
    }
    updateSelectionWidget();
}
