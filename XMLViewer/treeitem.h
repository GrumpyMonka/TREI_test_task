#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QVector>

class TreeItem
{
public:
    explicit TreeItem( const QVector<QVariant>& data, TreeItem* parentItem = nullptr );
    ~TreeItem();

    int columnCount() const;

    void appendChild( TreeItem* child );
    TreeItem* parent();
    TreeItem* child( int row );

    int childCount() const;
    int childNumber() const;
    QVariant data( int column ) const;
    bool setData( int column, const QVariant& value );

private:
    QList<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem* parentItem;
};

#endif // TREEITEM_H
