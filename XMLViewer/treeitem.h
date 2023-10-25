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

    QVariant data( int column ) const;

    TreeItem* parent();

private:
    QList<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem* parentItem;
};

#endif // TREEITEM_H
