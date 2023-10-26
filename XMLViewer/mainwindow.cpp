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
    QStringList tabs_list;
    for ( auto it = tabs.begin(); it != tabs.end(); ++it )
    {
        tabs_list.push_back( it.key() );
    }
    settings->setValue( "tabs", tabs_list );
}

void MainWindow::listViewItemChanged( const QModelIndex& index )
{
    if ( !tabs.contains( index.data().toString() ) )
    {
        XMlTable* table = new XMlTable( path_to_files + "/" + index.data().toString(), this );
        ui->tabWidget->addTab( table, index.data().toString() );
        ui->tabWidget->setCurrentWidget( table );
        tabs.insert( index.data().toString(), table );
    }
    saveOpenTabs();
}

void MainWindow::tabClosed( int index )
{
    if ( -1 == index )
    {
        return;
    }

    QWidget* tabItem = ui->tabWidget->widget( index );
    ui->tabWidget->removeTab( index );
    tabs.remove( tabs.key( tabItem ) );

    delete ( tabItem );
    tabItem = nullptr;
    saveOpenTabs();
}
