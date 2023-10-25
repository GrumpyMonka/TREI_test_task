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
