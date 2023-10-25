#ifndef XMLTABLE_H
#define XMLTABLE_H

#include <QTreeView>

#include <treemodel.h>

class XMlTable : public QTreeView
{
    Q_OBJECT

public:
    explicit XMlTable( const QString& path_to_file, QWidget* parent = nullptr );
    ~XMlTable();

private:
    QString path_to_file;
    TreeModel* model;
};

#endif // XMLTABLE_H
