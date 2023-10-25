#include "treemodel.h"

TreeModel::TreeModel( const QStringList& headers, const QDomElement& xmlData, QObject* parent )
{
    QVector<QVariant> rootData;
    for ( const auto& header : headers )
    {
        rootData.push_back( header );
    }
    rootItem = new TreeItem( rootData );
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

void TreeModel::setXmlData( const QDomElement& xmlData, TreeItem* parent )
{
}

QVariant TreeModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() || ( role != Qt::DisplayRole && role != Qt::EditRole ) )
    {
        return {};
    }
    TreeItem* item = getItem( index );
    if ( nullptr != item )
    {
        return item->data( index.column() );
    }
    else
    {
        return {};
    }
}

TreeItem* TreeModel::getItem( const QModelIndex& index ) const
{
    if ( index.isValid() )
    {
        TreeItem* item = static_cast<TreeItem*>( index.internalPointer() );
        if ( nullptr != item )
        {
            return item;
        }
    }
    return nullptr;
}

QModelIndex TreeModel::index( int row, int column, const QModelIndex& parent ) const
{
}

QModelIndex TreeModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
    {
        return QModelIndex();
    }

    TreeItem* item = getItem( index );
    if ( nullptr != item )
    {
        TreeItem* parentItem = item->parent();
        if ( nullptr == parentItem || rootItem == parentItem )
        {
            return {};
        }
        return createIndex( parentItem->childNumber(), 0, parentItem );
    }
}

int TreeModel::rowCount( const QModelIndex& parent ) const
{
}

int TreeModel::columnCount( const QModelIndex& parent ) const
{
    return rootItem->columnCount();
}
