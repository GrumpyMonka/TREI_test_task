#include "xmltable.h"

#include <QDomDocument>
#include <QFile>
#include <QTreeWidget>

#include <QDebug>

#include <QSpacerItem>
#include <QStandardItemModel>

XMlTable::XMlTable( const QString& path_to_file, QWidget* parent )
    : QTreeView( parent )
    , path_to_file( path_to_file )
{
}

XMlTable::~XMlTable()
{
}
