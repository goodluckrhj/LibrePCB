/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include <QtWidgets>
#include "netsignal.h"
#include "netclass.h"
#include "../../common/exceptions.h"
#include "circuit.h"

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

NetSignal::NetSignal(Circuit& circuit, const QDomElement& domElement) throw (Exception) :
    QObject(0), mCircuit(circuit), mDomElement(domElement)
{
    mUuid = mDomElement.attribute("uuid");
    if(mUuid.isNull())
    {
        throw RuntimeError(__FILE__, __LINE__, mDomElement.attribute("uuid"),
            QString(tr("Invalid netsignal UUID: \"%1\"")).arg(mDomElement.attribute("uuid")));
    }

    mName = mDomElement.attribute("name");
    if(mName.isEmpty())
    {
        throw RuntimeError(__FILE__, __LINE__, mUuid.toString(),
            QString(tr("Name of netsignal \"%1\" is empty!")).arg(mUuid.toString()));
    }

    mAutoName = (mDomElement.attribute("auto_name") == "true");

    mNetClass = mCircuit.getNetClassByUuid(mDomElement.attribute("netclass"));
    if (!mNetClass)
    {
        throw RuntimeError(__FILE__, __LINE__, mDomElement.attribute("netclass"),
            QString(tr("Invalid netclass UUID: \"%1\""))
            .arg(mDomElement.attribute("netclass")));
    }
}

NetSignal::~NetSignal() noexcept
{
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void NetSignal::setName(const QString& name) throw (Exception)
{
    if(name.isEmpty())
    {
        throw RuntimeError(__FILE__, __LINE__, QString(),
            tr("The new netsignal name must not be empty!"));
    }

    mDomElement.setAttribute("name", name);
    mName = name;

    mDomElement.setAttribute("auto_name", "false");
    mAutoName = false;
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void NetSignal::registerSchematicNetPoint(SchematicNetPoint* netpoint) noexcept
{
    Q_CHECK_PTR(netpoint);
    Q_ASSERT(!mSchematicNetPoints.contains(netpoint));
    mSchematicNetPoints.append(netpoint);
}

void NetSignal::unregisterSchematicNetPoint(SchematicNetPoint* netpoint) noexcept
{
    Q_CHECK_PTR(netpoint);
    Q_ASSERT(mSchematicNetPoints.contains(netpoint));
    mSchematicNetPoints.removeOne(netpoint);
}

void NetSignal::addToCircuit(bool addNode, QDomElement& parent) throw (Exception)
{
    Q_ASSERT(mSchematicNetPoints.isEmpty());

    if (addNode)
    {
        if (parent.nodeName() != "netsignals")
            throw LogicError(__FILE__, __LINE__, parent.nodeName(), tr("Invalid node name!"));

        if (parent.appendChild(mDomElement).isNull())
            throw LogicError(__FILE__, __LINE__, QString(), tr("Could not append DOM node!"));
    }

    mNetClass->registerNetSignal(this);
}

void NetSignal::removeFromCircuit(bool removeNode, QDomElement& parent) throw (Exception)
{
    Q_ASSERT(mSchematicNetPoints.isEmpty());

    if (removeNode)
    {
        if (parent.nodeName() != "netsignals")
            throw LogicError(__FILE__, __LINE__, parent.nodeName(), tr("Invalid node name!"));

        if (parent.removeChild(mDomElement).isNull())
            throw LogicError(__FILE__, __LINE__, QString(), tr("Could not remove node from DOM tree!"));
    }

    mNetClass->unregisterNetSignal(this);
}

/*****************************************************************************************
 *  Static Methods
 ****************************************************************************************/

NetSignal* NetSignal::create(Circuit& circuit, QDomDocument& doc, const QUuid& netclass,
                             const QString& name, bool autoName) throw (Exception)
{
    QDomElement node = doc.createElement("netsignal");
    if (node.isNull())
        throw LogicError(__FILE__, __LINE__, QString(), tr("Could not create DOM node!"));

    // fill the new QDomElement with all the needed content
    node.setAttribute("uuid", QUuid::createUuid().toString()); // generate random UUID
    node.setAttribute("name", name);
    node.setAttribute("auto_name", autoName ? "true" : "false");
    node.setAttribute("netclass", netclass.toString());

    // create and return the new NetSignal object
    return new NetSignal(circuit, node);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
