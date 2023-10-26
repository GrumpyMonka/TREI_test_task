#include "treemodel.h"

TreeModel::TreeModel( const QStringList& headers, const QDomElement& xmlData, QObject* parent )
{
    QVector<QVariant> rootData;
    for ( const auto& header : headers )
    {
        rootData.push_back( header );
    }
    rootItem = new TreeItem( rootData );
    setXmlData( xmlData, rootItem );
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

Qt::ItemFlags TreeModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return 0;
    return Qt::ItemIsEditable | QAbstractItemModel::flags( index );
}

void TreeModel::setXmlData( const QDomElement& xmlData, TreeItem* parent )
{
    QVector<QVariant> itemData;
    itemData.push_back( "vwvwe" );
    itemData.push_back( "vewrvewrv" );
    rootItem->appendChild( new TreeItem( itemData, rootItem ) );
}

QVariant TreeModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() || ( role != Qt::DisplayRole && role != Qt::EditRole ) )
    {
        return {};
    }
    TreeItem* item = getItem( index );
    return item->data( index.column() );
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
    return rootItem;
}

QModelIndex TreeModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( parent.isValid() && 0 != parent.column() )
    {
        return QModelIndex();
    }
    TreeItem* parentItem = getItem( parent );
    TreeItem* childItem = parentItem->child( row );
    if ( nullptr != childItem )
    {
        return createIndex( row, column, childItem );
    }
    return QModelIndex();
}

QModelIndex TreeModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
    {
        return QModelIndex();
    }

    TreeItem* item = getItem( index );
    TreeItem* parentItem = item->parent();
    if ( nullptr == parentItem || rootItem == parentItem )
    {
        return {};
    }
    return createIndex( parentItem->childNumber(), 0, parentItem );
}

int TreeModel::rowCount( const QModelIndex& parent ) const
{
    TreeItem* parentItem = getItem( parent );
    return parentItem->childCount();
}

int TreeModel::columnCount( const QModelIndex& parent ) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );
    return QVariant();
}

bool TreeModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role )
{
    if ( role != Qt::EditRole || orientation != Qt::Horizontal )
        return false;
    bool result = rootItem->setData( section, value );
    if ( result )
    {
        emit headerDataChanged( orientation, section, section );
    }
    return result;
}
