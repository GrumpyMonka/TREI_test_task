#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSettings>
#include <QStringListModel>
#include <QWidget>

#include <xmltable.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

public slots:
    void listViewItemChanged( const QModelIndex& index );
    void tabClosed( int index );
    void loadTabs();
    void saveOpenTabs();

private slots:
    void on_actionOpenFolder_triggered();
    void loadFiles();

    void on_actionExit_triggered();

    void on_actionExportFileToCSV_triggered();

    void on_actionExportSelectedRowsToCSV_triggered();

    XMLTable* currentTable();

private:
    Ui::MainWindow* ui;

    QString path_to_files;
    QSettings* settings;
    QStringList openFiles;
    QStringListModel* filesModel;
};
#endif // MAINWINDOW_H
