#include "treeitem.h"

TreeItem::TreeItem( const QVector<QVariant>& data, TreeItem* parent )
    : itemData( data )
    , parentItem( parent )
{
}

TreeItem::~TreeItem()
{
    qDeleteAll( childItems );
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data( int column ) const
{
    return itemData.at( column );
}

TreeItem* TreeItem::parent()
{
    return parentItem;
}

void TreeItem::appendChild( TreeItem* child )
{
    childItems.append( child );
}

TreeItem* TreeItem::child( int row )
{
    return childItems.at( row );
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::childNumber() const
{
    if ( nullptr != parentItem )
    {
        return parentItem->childItems.indexOf( const_cast<TreeItem*>( this ) );
    }
    return 0;
}

bool TreeItem::setData( int column, const QVariant& value )
{
    if ( 0 > column || itemData.size() < column )
    {
        return false;
    }
    itemData[column] = value;
    return true;
}
