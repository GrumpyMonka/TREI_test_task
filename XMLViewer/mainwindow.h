#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSettings>
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

private:
    Ui::MainWindow* ui;

    QString path_to_files;
    QSettings* settings;
    QMap<QModelIndex, QWidget*> tabs;
};
#endif // MAINWINDOW_H