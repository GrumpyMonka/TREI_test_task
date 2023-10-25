#ifndef XMLTABLE_H
#define XMLTABLE_H

#include <QTreeView>

class XMlTable : public QTreeView
{
    Q_OBJECT

public:
    explicit XMlTable( const QString& path_to_file, QWidget* parent = nullptr );
    ~XMlTable();

private:
    QString path_to_file;
};

#endif // XMLTABLE_H
