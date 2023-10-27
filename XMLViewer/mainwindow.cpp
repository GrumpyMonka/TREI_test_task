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

    filesModel = new QStringListModel( this );
    ui->listView->setModel( filesModel );
    settings = new QSettings( "settings.ini", QSettings::IniFormat, this );
    setGeometry( settings->value( "geometry" ).toRect() );

    path_to_files = settings->value( "path_to_files", "" ).toString();
    if ( "" == path_to_files )
    {
        QMessageBox::warning( this, "Путь до данных", "Не удалось найти папку с данными, просьба указать." );
        path_to_files = QFileDialog::getExistingDirectory( this, "Выберите папку" );
        settings->setValue( "path_to_files", path_to_files );
    }
    loadFiles();
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
    auto tabs_list = settings->value( "tabs" ).toStringList();
    for ( const auto& tab : tabs_list )
    {
        auto indexList = filesModel->match( filesModel->index( 0, 0 ), Qt::DisplayRole, tab, -1, Qt::MatchExactly | Qt::MatchCaseSensitive );
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
        auto table = new XMLTable( path_to_files + "/" + index.data().toString(), this );
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

    auto tabItem = ui->tabWidget->widget( index );
    ui->tabWidget->removeTab( index );
    openFiles.removeAt( index );

    delete ( tabItem );
    tabItem = nullptr;
    saveOpenTabs();
}

void MainWindow::on_actionOpenFolder_triggered()
{
    path_to_files = QFileDialog::getExistingDirectory( this, "Выберите папку" );
    settings->setValue( "path_to_files", path_to_files );
    loadFiles();
}

void MainWindow::loadFiles()
{
    filesModel->setStringList( QDir( path_to_files ).entryList( QDir::Files ) );
    while ( ui->tabWidget->count() )
    {
        tabClosed( 0 );
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

XMLTable* MainWindow::currentTable()
{
    auto widget = ui->tabWidget->currentWidget();
    if ( nullptr != widget )
    {
        return static_cast<XMLTable*>( widget );
    }
    return nullptr;
}

QString MainWindow::getPathToFile( const QString& file_format )
{
    QString filePath = QFileDialog::getSaveFileName( this, QString( "Сохранить в %1" ).arg( file_format.toUpper() ),
        "", QString( "%1 Files (*.%2);;All Files (*)" ).arg( file_format.toUpper() ).arg( file_format ) );
    if ( !filePath.endsWith( file_format, Qt::CaseInsensitive ) )
    {
        filePath += "." + file_format;
    }
    return filePath;
}

void MainWindow::on_actionExportFileToCSV_triggered()
{
    if ( nullptr != currentTable() )
    {
        auto filePath = getPathToFile( "csv" );
        if ( !filePath.isEmpty() )
        {
            currentTable()->exportAllToCSV( filePath );
        }
        else
        {
        }
    }
}

void MainWindow::on_actionExportSelectedRowsToCSV_triggered()
{
    if ( nullptr != currentTable() )
    {
        auto filePath = getPathToFile( "csv" );
        if ( !filePath.isEmpty() )
        {
            currentTable()->exportSelectedRowsToCSV( filePath );
        }
        else
        {
        }
    }
}
