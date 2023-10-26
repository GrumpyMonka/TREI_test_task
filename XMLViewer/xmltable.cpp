#include "xmltable.h"

#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>

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

    model = new QStandardItemModel( this );
    model->setHorizontalHeaderLabels( QStringList() << "Имя"
                                                    << "Тип"
                                                    << "Значение"
                                                    << "Комментарий" );

    setStyleSheet( "*::item{"
                   "    border-top-width: 0px;"
                   "    border-right-width: 1px;"
                   "    border-bottom-width: 1px;"
                   "    border-left-width: 0px;"
                   "    border-style: solid;"
                   "    border-color: silver;"
                   "}"
                   "*::item:selected{"
                   "    background: palette(Highlight);"
                   "}" );

    LoadDataFromXML( xmlDoc );
    setModel( model );
    header()->setSectionResizeMode( QHeaderView::Stretch );
    setItemDelegate( new CustomItemDelegate( this ) );
    expandAll();
}

XMlTable::~XMlTable()
{
}

void XMlTable::LoadDataFromXML( const QDomDocument& xmlDocument )
{
    model->appendRow( ConvertXmlToRow( xmlDocument.documentElement() ) );
}

QList<QStandardItem*> XMlTable::ConvertXmlToRow( const QDomElement& xmlElement )
{
    QList<QStandardItem*> result_row;
    result_row.push_back( new QStandardItem( xmlElement.attribute( "name" ) ) );
    int size = xmlElement.attribute( "size" ).toInt();
    QString type = "";
    if ( 1 == size )
    {
        type = "булевский";
    }
    else if ( 1 < size )
    {
        type = "целый";
    }
    result_row.push_back( new QStandardItem( type ) );
    result_row.push_back( new QStandardItem( xmlElement.attribute( "value" ) ) );
    result_row.push_back( new QStandardItem( xmlElement.attribute( "comment" ) ) );

    if ( xmlElement.tagName() == elem_tag )
    {
        return result_row;
    }

    for ( const auto& tag : main_tags )
    {
        QDomElement itemElement = xmlElement.firstChildElement( tag );
        while ( !itemElement.isNull() )
        {
            result_row.first()->appendRow( ConvertXmlToRow( itemElement ) );
            itemElement = itemElement.nextSiblingElement( tag );
        }
    }
    return result_row;
}
