#ifndef XMLTABLE_H
#define XMLTABLE_H

#include <QDomDocument>
#include <QPainter>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTreeView>

#include <QDebug>

class CustomItemDelegate : public QStyledItemDelegate
{
public:
    CustomItemDelegate( QObject* parent = nullptr )
        : QStyledItemDelegate( parent )
    {
    }

    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override
    {
        QModelIndex newIndex = index.sibling( index.row(), 1 );
        auto data = newIndex.data( Qt::DisplayRole ).toString();
        if ( data.isEmpty() )
        {
            painter->fillRect( option.rect, QColor( "#A8E4A0" ) );
        }
        QStyledItemDelegate::paint( painter, option, index );
    }
};

class XMlTable : public QTreeView
{
    Q_OBJECT

public:
    explicit XMlTable( const QString& path_to_file, QWidget* parent = nullptr );
    ~XMlTable();

    void LoadDataFromXML( const QDomDocument& xmlDocument );
    QList<QStandardItem*> ConvertXmlToRow( const QDomElement& xmlElement );

private:
    QString path_to_file;
    QStandardItemModel* model;

    const QVector<QString> main_tags = { "CONF", "PARAM", "BLK", "SUBBLK", "VAR" };
    const QString elem_tag = "VAR";
};

#endif // XMLTABLE_H
