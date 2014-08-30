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

#ifndef PROJECT_SCHEMATICPAGESDOCK_H
#define PROJECT_SCHEMATICPAGESDOCK_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include <QtWidgets>

/*****************************************************************************************
 *  Forward Declarations
 ****************************************************************************************/

namespace project {
class Project;
class SchematicEditor;
}

namespace Ui {
class SchematicPagesDock;
}

/*****************************************************************************************
 *  Class SchematicPagesDock
 ****************************************************************************************/

namespace project {

/**
 * @brief The SchematicPagesDock class
 */
class SchematicPagesDock final : public QDockWidget
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        explicit SchematicPagesDock(Project& project, SchematicEditor& editor);
        ~SchematicPagesDock();

        // Inherited from QDockWidget
        void resizeEvent(QResizeEvent* event);

    public slots:

        void activeSchematicChanged(int oldIndex, int newIndex);
        void schematicAdded(int newIndex);
        void schematicRemoved(int oldIndex);

    private slots:

        // UI
        void on_btnNewSchematic_clicked();
        void on_btnRemoveSchematic_clicked();
        void on_listWidget_currentRowChanged(int currentRow);

    private:

        // make some methods inaccessible...
        SchematicPagesDock();
        SchematicPagesDock(const SchematicPagesDock& other);
        SchematicPagesDock& operator=(const SchematicPagesDock& rhs);

        // General
        Project& mProject;
        SchematicEditor& mEditor;
        Ui::SchematicPagesDock* mUi;
};

} // namespace project

#endif // PROJECT_SCHEMATICPAGESDOCK_H
