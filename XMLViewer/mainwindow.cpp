#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include <xmltable.h>

#include <QDebug>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    settings = new QSettings( "settings.ini", QSettings::IniFormat, this );
    setGeometry( settings->value( "geometry" ).toRect() );

    path_to_files = settings->value( "path_to_files", "" ).toString();
    if ( "" == path_to_files )
    {
        QMessageBox::warning( this, "Путь до данных", "Не удалось найти папку с данными, просьба указать." );
        path_to_files = QFileDialog::getExistingDirectory( this, "Выберите папку" );
        settings->setValue( "path_to_files", path_to_files );
    }
    filesModel = new QStringListModel( QDir( path_to_files ).entryList( QDir::Files ), this );
    ui->listView->setModel( filesModel );

    loadTabs();

    connect( ui->listView, &QListView::clicked,
        this, &MainWindow::listViewItemChanged );
    connect( ui->tabWidget, &QTabWidget::tabCloseRequested,
        this, &MainWindow::tabClosed );

    saveOpenTabs();
}

MainWindow::~MainWindow()
{
    settings->setValue( "geometry", geometry() );
    delete ui;
}

void MainWindow::loadTabs()
{
    QStringList tabs_list = settings->value( "tabs" ).toStringList();
    for ( const auto& tab : tabs_list )
    {
        QModelIndexList indexList = filesModel->match( filesModel->index( 0, 0 ), Qt::DisplayRole, tab, -1, Qt::MatchExactly | Qt::MatchCaseSensitive );

        if ( !indexList.isEmpty() )
        {
            listViewItemChanged( indexList.first() );
        }
    }
}

void MainWindow::saveOpenTabs()
{
    settings->setValue( "tabs", openFiles );
}

void MainWindow::listViewItemChanged( const QModelIndex& index )
{
    auto fileName = index.data().toString();
    if ( !openFiles.contains( fileName ) )
    {
        XMlTable* table = new XMlTable( path_to_files + "/" + index.data().toString(), this );
        ui->tabWidget->addTab( table, index.data().toString() );
        ui->tabWidget->setCurrentWidget( table );
        openFiles.push_back( fileName );
        saveOpenTabs();
    }
    ui->tabWidget->setCurrentIndex( openFiles.indexOf( fileName ) );
}

void MainWindow::tabClosed( int index )
{
    if ( -1 == index )
    {
        return;
    }

    QWidget* tabItem = ui->tabWidget->widget( index );
    ui->tabWidget->removeTab( index );
    openFiles.removeAt( index );

    delete ( tabItem );
    tabItem = nullptr;
    saveOpenTabs();
}
