#include "xmltable.h"

#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QStack>
#include <QStandardItemModel>

#include <QDebug>

XMLTable::XMLTable( const QString& path_to_file, QWidget* parent )
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

    loadDataFromXML( xmlDoc );
    setModel( model );
    header()->setSectionResizeMode( QHeaderView::Stretch );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setItemDelegate( new CustomItemDelegate( this ) );
    expandAll();
}

XMLTable::~XMLTable()
{
}

void XMLTable::loadDataFromXML( const QDomDocument& xmlDocument )
{
    model->appendRow( convertXmlToRow( xmlDocument.documentElement() ) );
}

QList<QStandardItem*> XMLTable::convertXmlToRow( const QDomElement& xmlElement )
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
            result_row.first()->appendRow( convertXmlToRow( itemElement ) );
            itemElement = itemElement.nextSiblingElement( tag );
        }
    }
    return result_row;
}

QList<QList<QStandardItem*>> XMLTable::getRowsItems( bool using_indexs, const QModelIndexList& indexs )
{
    QList<QList<QStandardItem*>> rows_items;
    QList<QList<QStandardItem*>> stack;
    if ( model->rowCount() > 0 )
    {
        QList<QStandardItem*> rootRow;
        for ( int i = 0; i < model->columnCount(); ++i )
        {
            rootRow.push_back( model->item( 0, i ) );
        }
        stack.push_back( rootRow );
    }

    while ( !stack.isEmpty() )
    {
        if ( !using_indexs || ( using_indexs && indexs.contains( stack.back().at( 0 )->index() ) ) )
        {
            rows_items.push_back( stack.back() );
        }
        auto parentItem = stack.back().at( 0 );
        stack.pop_back();

        if ( parentItem->hasChildren() )
        {
            for ( int i = parentItem->rowCount() - 1; i >= 0; --i )
            {
                QList<QStandardItem*> row;
                for ( int k = 0; k < parentItem->columnCount(); ++k )
                {
                    row.push_back( parentItem->child( i, k ) );
                }
                stack.push_back( row );
            }
        }
    }

    return rows_items;
}

void XMLTable::exportAllToCSV( const QString& path )
{
    exportToCSVFile( getRowsItems(), path );
}

void XMLTable::exportSelectedRowsToCSV( const QString& path )
{
    auto index_list = selectionModel()->selectedRows();
    exportToCSVFile( getRowsItems( true, index_list ), path );
}

void XMLTable::exportToCSVFile( const QList<QList<QStandardItem*>>& rows_for_export, const QString& path )
{
    QFile file( path );
    if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        for ( int i = 0; i < model->columnCount(); ++i )
        {
            QStandardItem* item = model->horizontalHeaderItem( i );
            if ( nullptr != item )
            {
                file.write( item->text().toUtf8() + ";" );
            }
        }
        file.write( "\n" );

        for ( const auto& row : rows_for_export )
        {
            for ( const auto& item : row )
            {
                file.write( item->text().toUtf8() + ";" );
            }
            file.write( "\n" );
        }
    }
    else
    {
    }
    file.close();
}
