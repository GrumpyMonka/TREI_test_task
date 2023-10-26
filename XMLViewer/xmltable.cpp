#include "xmltable.h"

#include <QDomDocument>
#include <QFile>
#include <QMessageBox>

#include <QDebug>

XMlTable::XMlTable( const QString& path_to_file, QWidget* parent )
    : QTreeView( parent )
    , path_to_file( path_to_file )
{
    QFile file( path_to_file );
    if ( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::warning( this, "Ошибка", "Ошибка открытия файла: " + path_to_file );
        return;
    }

    QDomDocument xmlDoc;
    if ( !xmlDoc.setContent( &file ) )
    {
        QMessageBox::warning( this, "Ошибка", "Ошибка парсинга файла: " + path_to_file );
        file.close();
        return;
    }
    file.close();

    model = new TreeModel( QStringList() << "lol"
                                         << "kek",
        xmlDoc.documentElement(), this );
    for ( int column = 0; column < model->columnCount(); ++column )
    {
        resizeColumnToContents( column );
    }
}

XMlTable::~XMlTable()
{
}
