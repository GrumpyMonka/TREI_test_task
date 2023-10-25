#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStringListModel>

#include <xmltable.h>

#include <QDebug>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    settings = new QSettings( "settings.ini", QSettings::IniFormat, this );

    path_to_files = settings->value( "path_to_files", "" ).toString();
    if ( "" == path_to_files )
    {
        QMessageBox::warning( this, "Путь до данных", "Не удалось найти папку с данными, просьба указать." );
        path_to_files = QFileDialog::getExistingDirectory( this, "Выберите папку" );
        settings->setValue( "path_to_files", path_to_files );
    }
    QStringListModel* model = new QStringListModel( QDir( path_to_files ).entryList( QDir::Files ), this );
    ui->listView->setModel( model );

    connect( ui->listView, &QListView::clicked,
        this, &MainWindow::listViewItemChanged );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::listViewItemChanged( const QModelIndex& index )
{
    XMlTable* table = new XMlTable( path_to_files + "/" + index.data().toString(), this );
    ui->tabWidget->addTab( table, index.data().toString() );
    tabs.insert( index, table );
}
