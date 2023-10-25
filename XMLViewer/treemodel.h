#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QDomElement>
#include <QModelIndex>

#include <treeitem.h>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel( const QStringList& headers, const QDomElement& xmlData, QObject* parent = 0 );
    ~TreeModel();

    QVariant data( const QModelIndex& index, int role ) const override;
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex& index ) const override;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

private:
    void setXmlData( const QDomElement& xmlData, TreeItem* parent );

    TreeItem* getItem( const QModelIndex& index ) const;

private:
    TreeItem* rootItem;
};

#endif // TREEMODEL_H
