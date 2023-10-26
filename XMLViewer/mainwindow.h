#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSettings>
#include <QStringListModel>
#include <QWidget>

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

private:
    Ui::MainWindow* ui;

    QString path_to_files;
    QSettings* settings;
    QHash<QString, QWidget*> tabs;
    QStringListModel* filesModel;
};
#endif // MAINWINDOW_H
